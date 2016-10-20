#include "ble_central_event_handler.h"
#include "advertising.h"
#include "app_error.h"
#include "app_scheduler.h"
#include "app_timer.h"
#include "app_trace.h"
#include "ble.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "ble_db_discovery.h"
#include "ble_gap.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_types.h"
#include "led_sequence.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "scan.h"
#include "sdk_config.h"
#include "sdk_errors.h"
#include "softdevice_handler_appsh.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "dfu_handler.h"

/**@brief Function for handling BLE Stack events concerning central
 * applications.
 *
 * @details This function keeps the connection handles of central applications
 * up-to-date. It
 * parses scanning reports, initiating a connection attempt to peripherals when
 * a target UUID
 * is found, and manages connection parameter update requests. Additionally, it
 * updates the status
 * of LEDs used to report central applications activity.
 *
 * @note        Since this function updates connection handles, @ref
 * BLE_GAP_EVT_DISCONNECTED events
 *              should be dispatched to the target application before invoking
 * this function.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */

#define CENTRAL_LINK_COUNT 2
#define PERIPHERAL_LINK_COUNT 1

#define MAX_CONNECTED_CENTRALS  CENTRAL_LINK_COUNT
 /**< Maximum number of central applications which can be
       connected at any time. */
#define UUID16_SIZE 2
#define MIN_CONNECTION_INTERVAL                                                \
  MSEC_TO_UNITS(40,                                                           \
                UNIT_1_25_MS) /**< Determines minimum connection interval in \
                                 milliseconds. */
#define MAX_CONNECTION_INTERVAL                                                \
  MSEC_TO_UNITS(80,                                                            \
                UNIT_1_25_MS) /**< Determines maximum connection interval in \
                                 milliseconds. */
#define APP_COMPANY_IDENTIFIER 0x123
#define DEVICE_NAME "JWB_TEST" /**< Name of device used for advertising. */
#define SUPERVISION_TIMEOUT                                                    \
  MSEC_TO_UNITS(2000, UNIT_10_MS) /**< Determines supervision time-out in \
                                     units of 10 milliseconds. */
#define APPL_LOG                                                               \
  app_trace_log /**< Macro used to log debug information over UART. */
#define SLAVE_LATENCY                                                          \
  6 /**< Determines slave latency in terms of connection events. */
#define ADV_TYPE_LEN 2
#define BEACON_DATA_LEN 21

#define FIRST_CONN_PARAMS_UPDATE_DELAY                                         \
  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER) /**< Time from initiating event \                                                                             \
                                                (connect or start of \                                                                             \
                                                notification) to first time \                                                                             \
                                                sd_ble_gap_conn_param_update \                                                                             \
                                                is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY                                          \
  APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to \                                                                             \
                                          sd_ble_gap_conn_param_update after \                                                                             \
                                          the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT   3                                        \
	/**< Number of attempts before giving up the connection parameter \                                                                             \
       negotiation. */

/**@brief Macro to unpack 16bit unsigned UUID from an octet stream. */
#define UUID16_EXTRACT(DST, SRC)                                               \
  do {                                                                         \
    (*(DST)) = (SRC)[1];                                                       \
    (*(DST)) <<= 8;                                                            \
    (*(DST)) |= (SRC)[0];                                                      \
  } while (0)


// static char * target_device_name = "JWB_099";
static uint16_t m_conn_handle =
    BLE_CONN_HANDLE_INVALID; /**< Connection handle for the HRS central
                                application */
static ble_db_discovery_t m_ble_db_discovery_hrs; /**< HR service DB structure
                                                     used by the database
                                                     discovery module. */

void disconnect(void) {
  if (m_conn_handle != BLE_CONN_HANDLE_INVALID) {
    uint32_t err_code;
    err_code = sd_ble_gap_disconnect(m_conn_handle,
                                     BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
  }
}

uint16_t get_current_connection_handle(void) { return m_conn_handle; }
/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went
 * wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error) {
  APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for handling BLE Stack events involving peripheral
 * applications. Manages the
 * LEDs used to report the status of the peripheral applications.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
static void on_ble_peripheral_evt(ble_evt_t *p_ble_evt) {

  switch (p_ble_evt->header.evt_id) {

  case BLE_GAP_EVT_CONNECTED: {
    break;
  }

  case BLE_GAP_EVT_DISCONNECTED:
    advertising_start();
    break;

  default:
    // No implementation needed.
    break;
  }
}

void on_ble_central_evt(const ble_evt_t *const p_ble_evt) {
  // For readability.
  const ble_gap_evt_t *const p_gap_evt = &p_ble_evt->evt.gap_evt;
  switch (p_ble_evt->header.evt_id) {
  /** Upon connection, check which peripheral has connected (HR or RSC),
   * initiate DB
   *  discovery, update LEDs status and resume scanning if necessary. */
  case BLE_GAP_EVT_CONNECTED: {
    if (ble_conn_state_n_centrals() == MAX_CONNECTED_CENTRALS) {
      disconnect();
    } else {
      // Resume scanning.
      // LEDS_ON(CENTRAL_SCANNING_LED);
      // scan_start();
    }
    break; // BLE_GAP_EVT_CONNECTED
  }

  /** Upon disconnection, reset the connection handle of the peer which
   * disconnected, update
   * the LEDs status and start scanning again. */
  case BLE_GAP_EVT_DISCONNECTED: {
    uint8_t n_centrals;
    if (p_gap_evt->conn_handle == m_conn_handle) {
      NRF_LOG_PRINTF("HRS central disconnected (reason: %d)\r\n",
                     p_gap_evt->params.disconnected.reason);

      m_conn_handle = BLE_CONN_HANDLE_INVALID;
    }

    // Start scanning
    scan_start();
    advertising_start();
    // Update LEDs status.
    // LEDS_ON(CENTRAL_SCANNING_LED);
    n_centrals = ble_conn_state_n_centrals();
    if (n_centrals == 0) {
      // LEDS_OFF(CENTRAL_CONNECTED_LED);
    }

    break;
  } // BLE_GAP_EVT_DISCONNECTED

  case BLE_GAP_EVT_ADV_REPORT: {
    break;
  }

  // BLE_GAP_ADV_REPORT

  case BLE_GAP_EVT_TIMEOUT: {
    // We have not specified a timeout for scanning, so only connection attemps
    // can timeout.
    if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN) {
      APPL_LOG("[APPL]: Connection Request timed out.\r\n");
    }
  } break; // BLE_GAP_EVT_TIMEOUT

  case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST: {
    // Accept parameters requested by peer.
    ret_code_t err_code;
    err_code = sd_ble_gap_conn_param_update(
        p_gap_evt->conn_handle,
        &p_gap_evt->params.conn_param_update_request.conn_params);
    APP_ERROR_CHECK(err_code);
  } break; // BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST

  default:
    // No implementation needed.
    break;
  }
}
/**@brief Function for dispatching a BLE stack event to all modules with a BLE
 * stack event handler.
 *
 * @details This function is called from the scheduler in the main loop after a
 * BLE stack event has
 * been received.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void ble_evt_dispatch(ble_evt_t *p_ble_evt) {
  uint16_t conn_handle;
  uint16_t role;

  /** The Connection state module has to be fed BLE events in order to function
   * correctly
   * Remember to call ble_conn_state_on_ble_evt before calling any
   * ble_conns_state_* functions. */
  ble_conn_state_on_ble_evt(p_ble_evt);

  notify_dfu_ble_evt(p_ble_evt);
  // The connection handle should really be retrievable for any event type.
  conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
  role = ble_conn_state_role(conn_handle);

  // Based on the role this device plays in the connection, dispatch to the
  // right applications.
  if (role == BLE_GAP_ROLE_PERIPH) {
    // Manages peripheral LEDs.
    on_ble_peripheral_evt(p_ble_evt);

    ble_advertising_on_ble_evt(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);

    // Dispatch to peripheral applications.
    // ble_hrs_on_ble_evt (&m_hrs, p_ble_evt);
    // ble_rscs_on_ble_evt(&m_rscs, p_ble_evt);
  } else if ((role == BLE_GAP_ROLE_CENTRAL) ||
             (p_ble_evt->header.evt_id == BLE_GAP_EVT_ADV_REPORT)) {
    /** on_ble_central_evt will update the connection handles, so we want to
     * execute it
     * after dispatching to the central applications upon disconnection. */
    if (p_ble_evt->header.evt_id != BLE_GAP_EVT_DISCONNECTED) {
      on_ble_central_evt(p_ble_evt);
    }

    if (conn_handle == m_conn_handle) {
      // ble_hrs_c_on_ble_evt(&m_ble_hrs_c, p_ble_evt);
      ble_db_discovery_on_ble_evt(&m_ble_db_discovery_hrs, p_ble_evt);
    }
    // If the peer disconnected, we update the connection handles last.
    if (p_ble_evt->header.evt_id == BLE_GAP_EVT_DISCONNECTED) {
      on_ble_central_evt(p_ble_evt);
    }
  }
}

static void sys_evt_dispatch(uint32_t sys_evt) {
  ble_advertising_on_sys_evt(sys_evt);
  /** Dispatch the system event to the Flash Storage module, where it will be
   *  dispatched to the Flash Data Storage module and from there to the Peer
   * Manager. */
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupts.
 */
void ble_stack_init(void) {
  ret_code_t err_code;
  nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
  // Initialize the SoftDevice handler module.
  // SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, NULL);
  SOFTDEVICE_HANDLER_APPSH_INIT(&clock_lf_cfg, true);

  ble_enable_params_t ble_enable_params;
  memset(&ble_enable_params, 0, sizeof(ble_enable_params));
#ifdef S130
  ble_enable_params.gatts_enable_params.attr_tab_size =
      BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
#endif
  ble_enable_params.gatts_enable_params.service_changed = false;
  err_code = softdevice_enable_get_default_config(
      CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT, &ble_enable_params);
  APP_ERROR_CHECK(err_code);
  CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT);
  err_code = softdevice_enable(&ble_enable_params);
  APP_ERROR_CHECK(err_code);
  // Register with the SoftDevice handler module for BLE events.
  err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
  APP_ERROR_CHECK(err_code);

  // Register with the SoftDevice handler module for System events.
  err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
  APP_ERROR_CHECK(err_code);
}
void conn_params_init(void) {
  uint32_t err_code;
  ble_conn_params_init_t cp_init;

  memset(&cp_init, 0, sizeof(cp_init));

  cp_init.p_conn_params = NULL;
  cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
  cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
  cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
  cp_init.start_on_notify_cccd_handle =
      BLE_CONN_HANDLE_INVALID; // Start upon connection.
  cp_init.disconnect_on_fail = true;
  cp_init.evt_handler = NULL; // Ignore events.
  cp_init.error_handler = conn_params_error_handler;

  err_code = ble_conn_params_init(&cp_init);
  APP_ERROR_CHECK(err_code);
}

void gap_params_init(void) {
  uint32_t err_code;
  ble_gap_conn_params_t gap_conn_params;
  ble_gap_conn_sec_mode_t sec_mode;

  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
  err_code = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)DEVICE_NAME,
                                        strlen(DEVICE_NAME));
  APP_ERROR_CHECK(err_code);

  memset(&gap_conn_params, 0, sizeof(gap_conn_params));

  gap_conn_params.min_conn_interval = MIN_CONNECTION_INTERVAL;
  gap_conn_params.max_conn_interval = MAX_CONNECTION_INTERVAL;
  gap_conn_params.slave_latency = SLAVE_LATENCY;
  gap_conn_params.conn_sup_timeout = SUPERVISION_TIMEOUT;

  err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
  APP_ERROR_CHECK(err_code);
}

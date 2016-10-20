#include "app_error.h"
#include "ble_db_discovery.h"

/**@brief Function for handling database discovery events.
 *
 * @details This function is callback function to handle events from the
 * database discovery module.
 *          Depending on the UUIDs that are discovered, this function should
 * forward the events
 *          to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t *p_evt) {
  //    ble_rscs_on_db_disc_evt(&m_ble_rsc_c, p_evt);
  //    ble_hrs_on_db_disc_evt(&m_ble_hrs_c, p_evt);
}

void db_discovery_init(void) {
  ret_code_t err_code = ble_db_discovery_init(db_disc_handler);
  APP_ERROR_CHECK(err_code);
}

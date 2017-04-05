#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include "Arduino.h"
#include "JWB_Serial.h"
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
#include "app_uart.h"
#include "nrf.h"
#include "nrf_log.h"
#include "nrf_error.h"
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

__INLINE void jwb_uart_write(const char * msg)
{
  while( *msg )
  {
      NRF_UART0->TXD = *msg++;
      while( NRF_UART0->EVENTS_TXDRDY != 1 );
      NRF_UART0->EVENTS_TXDRDY = 0;
  }
}

void jwb_uart_write_many(int num_args, ...)
{
  const char* msg;
  va_list p_args;
  va_start(p_args, num_args);

  for (int i = 0; i < num_args; i++)
  {
      msg = va_arg(p_args, const char*);
      jwb_uart_write(msg);
  }
  va_end(p_args);
}

void jwb_uart_printf(const char * format_msg, ...)
{
  static char buffer[256];
  va_list p_args;
  va_start(p_args, format_msg);
  vsprintf(buffer, format_msg, p_args);
  va_end(p_args);
  jwb_uart_write(buffer);
}

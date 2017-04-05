#ifndef __UART_C__
#define __UART_C__

void uart_putstring(const char *str);
void uart_puthexbyte(uint8_t hexnum);
void uart_puthexquad(uint32_t hexquad);
void uart_puthexword(uint16_t hexword);

void uart_config(uint8_t txd_pin_number, uint8_t rxd_pin_number);
#endif

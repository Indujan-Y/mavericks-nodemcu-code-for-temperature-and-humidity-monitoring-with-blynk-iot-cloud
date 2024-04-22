#include <stdint.h>
void initUART() {
    TXSTA = 0x24;
    RCSTA = 0x90;
    SPBRG = 129;
}
void UART_Write_Text(const char* text) {
    while (*text) {
        while (!TXSTA.TRMT);
        TXREG = *text++;
    }
}

void main() {
    TRISB = 0x01;
    initUART();
    while (1) {
        if (PORTB & 0x01) {
            PORTB |= 0x02;
            UART_Write_Text("Alert! Ammonia is Leaking \r\n");
        } else {
            PORTB &= ~0x02;
        }
    }
}
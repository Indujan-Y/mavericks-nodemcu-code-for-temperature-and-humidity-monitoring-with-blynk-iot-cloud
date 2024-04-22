#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF
#define _XTAL_FREQ 20000000

#include <stdio.h>
#include <stdint.h>

void initADC() {
    ADCON1 = 0x0E;
    ADCON0 = 0x01;
}

void initUART() {
    TXSTA = 0x24;
    RCSTA = 0x90;
    SPBRG = 129;
}

uint16_t readADC() {
    ADCON0.GO = 1;
    while (ADCON0.GO);
    return (ADRESH << 8) | ADRESL;
}

void UART_Write_Text(char* text) {
    while (*text) {
        while (!TXSTA.TRMT);
        TXREG = *text++;
    }
}

void main() {
    char buffer[16];
    initADC();
    initUART();
    while (1) {
        uint16_t adc_value = readADC();
        sprintf(buffer, "ADC: %u\r\n", adc_value);
        UART_Write_Text(buffer);
        __delay_ms(1000);
    }
}
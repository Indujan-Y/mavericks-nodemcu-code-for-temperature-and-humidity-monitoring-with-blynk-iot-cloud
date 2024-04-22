sbit DHT11_PIN at RB1_bit;
sbit LED1 at RB0_bit;
sbit LED2 at RB3_bit;
sbit SWITCH at RB2_bit;

unsigned char temperature, humidity;

void DHT11_Start() {
    TRISB1_bit = 0;
    DHT11_PIN = 0;
    Delay_ms(18);
    DHT11_PIN = 1;
    TRISB1_bit = 1;
    Delay_us(40);
}
bit DHT11_CheckResponse() {
    if (DHT11_PIN == 0) {
        Delay_us(80);
        if (DHT11_PIN == 1) {
            Delay_us(80);
            return 1;
        }
    }
    return 0;
}
unsigned char DHT11_ReadByte() {
    unsigned char i, result = 0;

    for (i = 0; i < 8; i++) {
        while (DHT11_PIN == 0);
        Delay_us(40);
        if (DHT11_PIN == 1) {
            result |= (1 << (7 - i));
            while (DHT11_PIN == 1);
        }
    }
    return result;
}

void main() {
    TRISB2_bit = 1;
    TRISB0_bit = 0;
    TRISB3_bit = 0;
    while (1) {
        if (PORTB.F2 == 1) {
            LED1 = 1;
            LED2 = 1;
        } else {
            LED1 = 0;
            LED2 = 0;
        }
        DHT11_Start();
        if (DHT11_CheckResponse()) {
            humidity = DHT11_ReadByte();
            DHT11_ReadByte();
            temperature = DHT11_ReadByte();
            DHT11_ReadByte();
            DHT11_ReadByte();
        }
        Delay_ms(2000);
    }
}
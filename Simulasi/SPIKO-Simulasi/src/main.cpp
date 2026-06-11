/*
   SISTEM MONITORING DAN PEMBERI PAKAN IKAN OTOMATIS
   AVR Bare Metal - ATmega2560 / Arduino Mega 2560
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h> // WAJIB untuk Interupsi Serial

// =====================================================
// KONFIGURASI PIN
// =====================================================
#define DS18B20_DDR   DDRA
#define DS18B20_PORT  PORTA
#define DS18B20_PINR  PINA
#define DS18B20_PIN   PA0

#define SERVO_DDR   DDRH
#define SERVO_PORT  PORTH
#define SERVO_PIN   PH6

#define DS1307_ADDR 0x68

// =====================================================
// VARIABEL GLOBAL
// =====================================================
uint8_t jamPakan1 = 20, menitPakan1 = 34;
uint8_t jamPakan2 = 21, menitPakan2 = 0;
uint8_t sudahPakan1 = 0, sudahPakan2 = 0;

// Ring Buffer untuk Interupsi UART RX
#define RX_BUF_SIZE 64
volatile char rx_buffer[RX_BUF_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

// =====================================================
// INTERUPSI SERIAL RX (Menangkap data di balik layar)
// =====================================================
ISR(USART0_RX_vect)
{
    char c = UDR0;
    uint8_t next_head = (rx_head + 1) % RX_BUF_SIZE;
    if (next_head != rx_tail)
    {
        rx_buffer[rx_head] = c;
        rx_head = next_head;
    }
}

// =====================================================
// DELAY VARIABEL
// =====================================================
void delay_ms_var(uint16_t ms) { while (ms--) { _delay_ms(1); } }
void delay_us_var(uint16_t us) { while (us--) { _delay_us(1); } }

// =====================================================
// UART0 — TX + RX
// =====================================================
void UART0_Init(uint32_t baud)
{
    uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    // Enable TX, RX, dan Interupsi RX
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    sei(); // Aktifkan Global Interrupt
}

void UART0_SendChar(char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void UART0_SendString(const char *str)
{
    while (*str) UART0_SendChar(*str++);
}

void UART0_SendInt(int16_t data)
{
    char buffer[10];
    itoa(data, buffer, 10);
    UART0_SendString(buffer);
}

uint8_t UART0_Available(void)
{
    return (rx_head != rx_tail);
}

char UART0_ReadChar(void)
{
    if (rx_head == rx_tail) return 0;
    char c = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUF_SIZE;
    return c;
}

uint8_t UART0_ReadLine(char *buf, uint8_t maxLen)
{
    static uint8_t i = 0;
    while (UART0_Available())
    {
        char c = UART0_ReadChar();
        if (c == '\n' || c == '\r')
        {
            if (i == 0) continue; // Abaikan karakter kosong berturut-turut
            buf[i] = '\0';
            i = 0;
            return 1;
        }
        else if (i < maxLen - 1)
        {
            buf[i++] = c;
        }
    }
    return 0;
}

// =====================================================
// ADC UNTUK PH SENSOR
// =====================================================
void ADC_Init(void)
{
    ADMUX  = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_Read(uint8_t channel)
{
    ADMUX = (ADMUX & 0xE0) | (channel & 0x1F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

float bacaPH(void)
{
    uint16_t adcValue = ADC_Read(0);
    return ((float)adcValue * 14.0) / 1023.0;
}

// =====================================================
// TWI / I2C UNTUK RTC DS1307
// =====================================================
void TWI_Init(void) { TWBR = 72; TWSR = 0x00; }
void TWI_Start(void) { TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); while (!(TWCR & (1 << TWINT))); }
void TWI_Stop(void) { TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); }
void TWI_Write(uint8_t data) { TWDR = data; TWCR = (1 << TWINT) | (1 << TWEN); while (!(TWCR & (1 << TWINT))); }
uint8_t TWI_Read_ACK(void) { TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); while (!(TWCR & (1 << TWINT))); return TWDR; }
uint8_t TWI_Read_NACK(void) { TWCR = (1 << TWINT) | (1 << TWEN); while (!(TWCR & (1 << TWINT))); return TWDR; }

uint8_t bcdToDec(uint8_t val) { return ((val >> 4) * 10) + (val & 0x0F); }
uint8_t decToBcd(uint8_t val) { return ((val / 10) << 4) | (val % 10); }

void RTC_ReadTime(uint8_t *jam, uint8_t *menit, uint8_t *detik)
{
    TWI_Start();
    TWI_Write(DS1307_ADDR << 1);
    TWI_Write(0x00);
    TWI_Start();
    TWI_Write((DS1307_ADDR << 1) | 1);
    uint8_t rawDetik = TWI_Read_ACK();
    uint8_t rawMenit = TWI_Read_ACK();
    uint8_t rawJam   = TWI_Read_NACK();
    TWI_Stop();
    *detik = bcdToDec(rawDetik & 0x7F);
    *menit = bcdToDec(rawMenit);
    *jam   = bcdToDec(rawJam & 0x3F);
}

// =====================================================
// ONE WIRE UNTUK DS18B20 (Ditambahkan Proteksi CLI/SEI)
// =====================================================
void OneWire_Low(void) { DS18B20_DDR |= (1 << DS18B20_PIN); DS18B20_PORT &= ~(1 << DS18B20_PIN); }
void OneWire_Release(void) { DS18B20_DDR &= ~(1 << DS18B20_PIN); DS18B20_PORT |= (1 << DS18B20_PIN); }

uint8_t OneWire_Reset(void)
{
    uint8_t presence;
    OneWire_Low(); _delay_us(480);
    OneWire_Release(); _delay_us(70);
    presence = !(DS18B20_PINR & (1 << DS18B20_PIN));
    _delay_us(410);
    return presence;
}

void OneWire_WriteBit(uint8_t bit)
{
    cli(); // Matikan interupsi sejenak agar timing sensor tidak rusak
    if (bit) { OneWire_Low(); _delay_us(6);  OneWire_Release(); _delay_us(64); }
    else     { OneWire_Low(); _delay_us(60); OneWire_Release(); _delay_us(10); }
    sei();
}

uint8_t OneWire_ReadBit(void)
{
    uint8_t bit;
    cli();
    OneWire_Low(); _delay_us(6);
    OneWire_Release(); _delay_us(9);
    bit = (DS18B20_PINR & (1 << DS18B20_PIN)) ? 1 : 0;
    sei();
    _delay_us(55);
    return bit;
}

void OneWire_WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++) { OneWire_WriteBit(data & 0x01); data >>= 1; }
}

uint8_t OneWire_ReadByte(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++) { if (OneWire_ReadBit()) data |= (1 << i); }
    return data;
}

float DS18B20_ReadTemperature(void)
{
    uint8_t tempL, tempH;
    int16_t rawTemp;
    if (!OneWire_Reset()) return -127.0;
    OneWire_WriteByte(0xCC);
    OneWire_WriteByte(0x44);
    delay_ms_var(750);
    if (!OneWire_Reset()) return -127.0;
    OneWire_WriteByte(0xCC);
    OneWire_WriteByte(0xBE);
    tempL = OneWire_ReadByte();
    tempH = OneWire_ReadByte();
    rawTemp = (tempH << 8) | tempL;
    return (float)rawTemp / 16.0;
}

// =====================================================
// SERVO SOFTWARE PWM
// =====================================================
void Servo_Init(void)
{
    SERVO_DDR  |=  (1 << SERVO_PIN);
    SERVO_PORT &= ~(1 << SERVO_PIN);
}

void Servo_Write(uint8_t angle)
{
    uint16_t pulseWidth = 1000 + ((uint32_t)angle * 1000) / 180;
    SERVO_PORT |=  (1 << SERVO_PIN); delay_us_var(pulseWidth);
    SERVO_PORT &= ~(1 << SERVO_PIN); delay_us_var(20000 - pulseWidth);
}

void Servo_Hold(uint8_t angle, uint16_t duration_ms)
{
    uint16_t cycle = duration_ms / 20;
    for (uint16_t i = 0; i < cycle; i++) Servo_Write(angle);
}

// =====================================================
// FUNGSI BERI PAKAN (Disinkronkan dengan JSON Web)
// =====================================================
void beriPakan(void)
{
    UART0_SendString("{\"status\":\"Katup Dibuka\"}\r\n");
    Servo_Hold(90, 1500); // Bergerak ke 90°
    
    UART0_SendString("{\"status\":\"Katup Tertutup\"}\r\n");
    Servo_Hold(0,  1000); // Kembali ke 0°
}

// =====================================================
// PROSES PERINTAH
// =====================================================
void prosesPerintah(const char *cmd)
{
    if (strcmp(cmd, "PAKAN") == 0)
    {
        beriPakan();
    }
}

// =====================================================
// KIRIM DATA JSON
// =====================================================
void kirimJSON(uint8_t jam, uint8_t menit, uint8_t detik, float suhu, float ph)
{
    char buf[10];
    UART0_SendString("{");
    UART0_SendString("\"jam\":");    UART0_SendInt(jam);
    UART0_SendString(",\"menit\":"); UART0_SendInt(menit);
    UART0_SendString(",\"detik\":"); UART0_SendInt(detik);

    UART0_SendString(",\"suhu\":");
    dtostrf(suhu, 5, 2, buf);
    UART0_SendString(buf);

    UART0_SendString(",\"ph\":");
    dtostrf(ph, 4, 2, buf);
    UART0_SendString(buf);
    UART0_SendString("}\r\n");
}

// =====================================================
// MAIN PROGRAM
// =====================================================
int main(void)
{
    float suhuAir, nilaiPH;
    uint8_t jam, menit, detik;
    char cmdBuf[32];

    UART0_Init(9600);
    ADC_Init();
    TWI_Init();
    Servo_Init();

    OneWire_Release();
    Servo_Hold(0, 500);

    UART0_SendString("{\"event\":\"boot\",\"msg\":\"Sistem Monitoring Pakan Ikan\"}\r\n");

    while (1)
    {
        // Berkat Interrupt, kita bisa memproses perintah bertumpuk di sini
        while (UART0_ReadLine(cmdBuf, sizeof(cmdBuf)))
        {
            prosesPerintah(cmdBuf);
        }

        RTC_ReadTime(&jam, &menit, &detik);
        suhuAir  = DS18B20_ReadTemperature();
        nilaiPH  = bacaPH();

        kirimJSON(jam, menit, detik, suhuAir, nilaiPH);

        if (jam == jamPakan1 && menit == menitPakan1 && sudahPakan1 == 0) { beriPakan(); sudahPakan1 = 1; }
        if (jam == jamPakan2 && menit == menitPakan2 && sudahPakan2 == 0) { beriPakan(); sudahPakan2 = 1; }
        if (jam == 0 && menit == 0) { sudahPakan1 = 0; sudahPakan2 = 0; }

        delay_ms_var(1000);
    }
}

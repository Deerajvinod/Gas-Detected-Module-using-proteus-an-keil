#include <reg51.h>  // Include 8051 register definitions

// ======================= LCD CONNECTIONS ========================
#define lcd_data P1        // LCD data bus connected to Port 1

sbit rs = P2^0;            // RS pin connected to P2.0
sbit rw = P2^1;            // RW pin connected to P2.1
sbit en = P2^2;            // EN pin connected to P2.2

// ======================= SENSOR ========================
sbit gas_sensor = P3^2;    // Gas sensor output connected to P3.2

// ======================= FUNCTION DECLARATIONS ========================
void lcd_cmd(unsigned char cmd);
void lcd_data_write(unsigned char dat);
void lcd_init();
void lcd_string(char *str);
void lcd_clear();
void delay_ms(unsigned int ms);
void lcd_gotoxy(unsigned char row, unsigned char col);

// ======================= MAIN FUNCTION ========================
void main() {
    lcd_init();                   // Initialize LCD
    lcd_string("   Gas Detector  ");
    delay_ms(2000);
    lcd_clear();

    while(1) {
        if (gas_sensor == 1) {    // Gas detected (HIGH)
            lcd_clear();
            lcd_string("Gas Detected!");
            lcd_gotoxy(2, 1);
            lcd_string("Take Action!");
        } else {
            lcd_clear();
            lcd_string("No Gas Present");
            lcd_gotoxy(2, 1);
            lcd_string("System Normal");
        }
        delay_ms(500);           // Small delay before checking again
    }
}

// ======================= LCD FUNCTIONS ========================
void lcd_cmd(unsigned char cmd) {
    lcd_data = cmd;    // Put command on data bus
    rs = 0;            // RS = 0 -> command register
    rw = 0;            // RW = 0 -> write operation
    en = 1;            // Enable pulse
    delay_ms(2);
    en = 0;
}

void lcd_data_write(unsigned char dat) {
    lcd_data = dat;    // Put data on data bus
    rs = 1;            // RS = 1 -> data register
    rw = 0;            // RW = 0 -> write operation
    en = 1;            // Enable pulse
    delay_ms(2);
    en = 0;
}

void lcd_init() {
    lcd_cmd(0x38); // 8-bit mode, 2 lines, 5x7 font
    lcd_cmd(0x0C); // Display ON, cursor OFF
    lcd_cmd(0x06); // Entry mode, increment cursor
    lcd_cmd(0x01); // Clear display
    lcd_cmd(0x80); // Cursor to first line, first position
}

void lcd_string(char *str) {
    while(*str) {
        lcd_data_write(*str++);
    }
}

void lcd_clear() {
    lcd_cmd(0x01);    // Clear display
    delay_ms(2);
}

void lcd_gotoxy(unsigned char row, unsigned char col) {
    unsigned char pos;
    if(row == 1)
        pos = 0x80 + (col - 1); // First line
    else if(row == 2)
        pos = 0xC0 + (col - 1); // Second line
    lcd_cmd(pos);
}

// ======================= DELAY FUNCTION ========================
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 127; j++); // Approximate delay
}

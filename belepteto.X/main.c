#include <xc.h>

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT Oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RA5/MCLR pin function select bit (MCLR pin enabled)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (Disabled)
#pragma config CPD = OFF        // Data EEPROM Code Protection (Disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Disabled)

#define INPUT_PIN1 PORTBbits.RB0 // Signal from LM32/7/b
#define INPUT_PIN2 PORTBbits.RB1 // OI from HEF40/A
#define INPUT_PIN3 PORTBbits.RB2 // STR from HEF40/A
#define SPEAKER_PIN PORTAbits.RA2 // Output for speaker
#define RELAY_PIN PORTAbits.RA1   // Output for relay (choose exact pin later)

void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_ms(1);
    }
}

void initialize_pins() {
    TRISBbits.TRISB0 = 1; // Set RB0 as input
    TRISBbits.TRISB1 = 0; // Set RB1 as input
    TRISBbits.TRISB2 = 1; // Set RB2 as input
    TRISAbits.TRISA2 = 0; // Set RA2 as output (Speaker)
    TRISAbits.TRISA1 = 0; // Set RA1 as output (Relay)
}

void beep() {
    SPEAKER_PIN = 1;    // Turn on speaker
    delay_ms(100);      // Beep for 100 ms
    SPEAKER_PIN = 0;    // Turn off speaker
    delay_ms(100);      // Pause for 100 ms
}

void main() {
    initialize_pins(); // Initialize I/O pins
    
    while (1) {
        // Example logic: Trigger a beep when all inputs are high
        if (INPUT_PIN1 && INPUT_PIN2 && INPUT_PIN3) {
            beep();
            RELAY_PIN = 1; // Activate relay (adjust condition as needed)
        } else {
            RELAY_PIN = 0; // Deactivate relay
        }
    }
}

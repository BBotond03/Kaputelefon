#include <xc.h>

#define _XTAL_FREQ 20000000
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (XT Oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RA5/MCLR pin function select bit (MCLR pin enabled)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (Disabled)
#pragma config CPD = OFF        // Data EEPROM Code Protection (Disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Disabled)

#define INPUT_PIN1 PORTAbits.RA1 // Signal from LM32/7/b
#define INPUT_PIN2 PORTAbits.RA3 // OE from HEF40/A
#define INPUT_PIN3 PORTBbits.RB2 // STR from HEF40/A
#define BUTTON PORTAbits.RA2 //Button

#define SPEAKER PORTBbits.RB1 // Output for speaker
#define RELAY PORTAbits.RA0   // Output for relay
#define LED PORTBbits.RB0 // Output LED



void initialize_pins() {
    TRISBbits.TRISB0 = 0; // LED
    TRISAbits.TRISA3 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB1 = 0; // Speaker
    TRISAbits.TRISA0 = 0; // Relay
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1; // Button
    

}
/*
void beep() {
    SPEAKER_PIN = 1;    // Turn on speaker
    delay_ms(100);      // Beep for 100 ms
    SPEAKER_PIN = 0;    // Turn off speaker
    delay_ms(100);      // Pause for 100 ms
}
  */
int open;

void main() {
    initialize_pins(); // Initialize I/O pins
    LED = 0;

    while (1) {
        if (BUTTON == 1) {    
        __delay_ms(50); 
        }
        if (BUTTON == 0) {   
            LED = !LED;
            __delay_ms(50);
        }
        __delay_ms(50);

        


    }
}
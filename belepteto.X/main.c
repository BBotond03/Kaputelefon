#include <xc.h>

#define _XTAL_FREQ 20000000
#define MAX_VALUES 12


// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (XT Oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RA5/MCLR pin function select bit (MCLR pin enabled)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (Disabled)
#pragma config CPD = OFF        // Data EEPROM Code Protection (Disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Disabled)

#define PADIN PORTAbits.RA1 // Signal from LM32/7/b
#define SRAOE PORTAbits.RA3 // OE from HEF40/A
#define SRASTR PORTBbits.RB2 // STR from HEF40/A
#define BUTTON PORTAbits.RA2 //Button

#define SPEAKER PORTBbits.RB1 // Output for speaker
#define RELAY PORTAbits.RA0   // Output for relay
#define LED PORTBbits.RB0 // Output LED

int pinPad[12];
int index = 0; 
int outputValue;

void beep() {
    SPEAKER = 1;    // Turn on speaker
    __delay_ms(100);      // Beep for 100 ms
    SPEAKER = 0;    // Turn off speaker
    __delay_ms(100);      // Pause for 100 ms
}

void __interrupt() isr() {
    if (TMR1IF) { 
        LED = PADIN;
        if(PADIN = 1)
            beep();
        TMR1IF = 0;
        TMR1 = 0;

        pinPad[index] = PADIN;
        
        index++;

        if (index >= MAX_VALUES) {
            outputValue = 0; 
            for (int i = 0; i < MAX_VALUES; i++) {
                outputValue |= (pinPad[i] << i); 
            }
            for (int i = 0; i < MAX_VALUES; i++) {
                if(pinPad[i] == 1)
                {
                    LED = !LED;
                    __delay_ms(300);
                    LED = !LED;
                }
                
            }
            index = 0;
        }
    }
}

void setup_timer1()
{
    T1CONbits.TMR1CS = 0;
    T1CONbits.T1CKPS = 3;
    T1CONbits.nT1SYNC = 1;
    TMR1H = 0x00;
    TMR1L = 0x00;
    T1CONbits.TMR1ON = 1;
    
}

void initialize_pins() {
    CMCON = 0x07;
    TRISBbits.TRISB0 = 0; // LED
    TRISAbits.TRISA3 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB1 = 0; // Speaker
    TRISAbits.TRISA0 = 0; // Relay
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1; // Button
    SRAOE = 1;
    SRASTR = 1;
    
    

}


int open;

void main() {
    initialize_pins(); // Initialize I/O pins
    

    
    LED = 0;
    while (1) {
    }
}
#include <xc.h>

#define _XTAL_FREQ 20000000
#define MAX_VALUES 12
#define NUICANCE 6000

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
#define SRCLK PORTBbits.RB6  //data output
#define DATA PORTBbits.RB7  //data output 
#define BUTTON PORTAbits.RA2 //Button

#define SPEAKER PORTBbits.RB1 // Output for speaker
#define RELAY PORTAbits.RA0   // Output for relay
#define LED PORTBbits.RB0 // Output LED

#define NUMSENSOR 7
#define NUMTRACKER

int sensor_index = 0;
int output_index = 0;
int output_pinpad[NUMSENSOR];
int input_pinpad[NUMSENSOR];
int USER_INPUT[7];

void delay_us(int time)
{
    for(int i =0; i< time; i++)
    {
        __delay_us(1);
    }
}

void beep(int duration_ms) 
{
    int half_period_us = 1000000 / (2 * NUICANCE);
    int total_cycles = (duration_ms * 1000) / (2 * half_period_us);

    for (int i = 0; i < total_cycles; i++) {
        SPEAKER = 1;              
        delay_us((int)half_period_us);  
        SPEAKER = 0;                
        delay_us((int)half_period_us);  
    }
}

void cycleOutput(int list[], int size) {
    int currentIndex = -1;

    for (int i = 0; i < size; i++) {
        if (list[i] == 1) {
            currentIndex = i;
            list[i] = 0;
            break;
        }
    }

    int nextIndex = (currentIndex + 1) % size;
    list[nextIndex] = 1;
}
void decodeValue(int code[])
{
    for(int i =0; i<7; i++)
    {
        if(code[i] == 0)
        {
            //beep(1000); 

        }
    }
}
/*
void __interrupt() isr() {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;
        TMR1H = 0x00; 
        TMR1L = 0x00;
        SRCLK = 1;
        __delay_ms(100);
        if (sensor_index < 2 * NUMSENSOR) {
            if (sensor_index < NUMSENSOR) {
                SRASTR = 0;
                DATA = (int)input_pinpad[sensor_index];
                sensor_index++;
            } else {
                SRASTR = 1; 
                DATA = (int)input_pinpad[sensor_index - NUMSENSOR];
                output_pinpad[sensor_index - NUMSENSOR] = PADIN;
                sensor_index++;
            }
        } else {
            sensor_index = 0;
            decodeValue(output_pinpad); 
            cycleOutput(input_pinpad, NUMSENSOR);
        }
        SRCLK = 0;

    }
}
*/


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
    TRISBbits.TRISB7 = 0;  //data output 
    TRISBbits.TRISB6 = 0;  //shift register clk
    SRAOE = 1;
    for(int i =0; i<NUMSENSOR; i++)
    {
        if(i == 0)
        {
            output_pinpad[i] = 1;
            input_pinpad[i] = 1;
            
        }
        else
        {
            output_pinpad[i] = 1;
            input_pinpad[i] = 0;  
        }
    }   
    INTCONbits.GIE = 1;  
    INTCONbits.PEIE = 1;

}


int open;

void main() {
    initialize_pins(); 
    setup_timer1(); 

    LED = 0;
    
    while (1) {
        
        if (BUTTON == 0) {
            beep(1000); 
            __delay_ms(50);
            
        }
        SRCLK = 1;
        __delay_ms(50);
        
        if (sensor_index < 2 * NUMSENSOR) {
            if (sensor_index < NUMSENSOR) {
                SRASTR = 0;
                DATA = input_pinpad[sensor_index];
                sensor_index++;
                if(DATA == 0)
                {
                    
                }
                if(DATA == 1)
                {
                   LED=!LED;
                    __delay_ms(50);
                    LED=!LED; 
                }
            } else {
                SRASTR = 1; 
                DATA = input_pinpad[sensor_index - NUMSENSOR];
                output_pinpad[sensor_index - NUMSENSOR] = PADIN;
                sensor_index++;
            }
        } else {
            sensor_index = 0;
            decodeValue(output_pinpad); 
            cycleOutput(input_pinpad, NUMSENSOR);
        }
        SRCLK = 0;
        __delay_ms(50);


    
    }
}


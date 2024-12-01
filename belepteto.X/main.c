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


//relay????????????!!!!!!!!!!!
#define OUTPUT PORTAbits.RA1 // Signal from LM32/7/b
#define SRAOE PORTAbits.RA3 // OE from HEF40/A
#define STR PORTBbits.RB2 // STR from HEF40/A
#define SRCLK PORTBbits.RB6  //CLOCK A
#define DATA PORTBbits.RB7  //data output 
#define BUTTON PORTAbits.RA2 //Button

#define SPEAKER PORTBbits.RB1 // Output for speaker
//#define RELAY PORTAbits.RA0   // Output for relay
#define LED PORTBbits.RB0 // Output LED

#define NUMSENSOR 7

int sensorIndex = 0;
int outputIndex = 0;
int outputPinpad[NUMSENSOR];
int inputPinpad[NUMSENSOR];
int USER_INPUT[8];

void initialize_pins() {
    CMCON = 0x07;
    TRISBbits.TRISB0 = 0; // LED
    TRISAbits.TRISA3 = 0; //output enable
    TRISBbits.TRISB2 = 0; // STR
    TRISBbits.TRISB1 = 0; // Speaker
   // TRISAbits.TRISA0 = 1; // Relay
    TRISAbits.TRISA1 = 1; // input
    TRISAbits.TRISA2 = 1; // Button
    TRISBbits.TRISB7 = 0;  // data output 
    TRISBbits.TRISB6 = 0;  // shift register clk
    SRAOE = 1;
    for(int i =0; i<NUMSENSOR; i++)
    {
        if(i == 0)
        {
            outputPinpad[i] = 1;
            inputPinpad[i] = 1;
            
        }
        else
        {
            outputPinpad[i] = 1;
            inputPinpad[i] = 0;  
        }
    }   
    //INTCONbits.GIE = 1;  
    //INTCONbits.PEIE = 1;

}



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
void flash()
{
    LED = !LED;
    __delay_ms(50);
    LED = !LED;
}

void cycle_output(int list[], int size) {
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
void decode_value(int code[])
{
    for(int i =0; i<7; i++)
    {
        if(code[i] == 1)
        {
            beep(500);
        }
        else 
            flash();
    }
}
void clock_SR()
{
    SRCLK = 1;  
    __delay_us(10);
    SRCLK = 0;
}
void push_output()
{
    STR = 0;
    __delay_us(10);
    STR = 1;
}

int open = 1;

void main() {
    initialize_pins(); 
    STR = 1;
    LED = 0;
    int itter = 0;
    int cycle = 0;
    while (1) {
        if (BUTTON == 0) {
            beep(1000);
            __delay_ms(50);
        }

        DATA = 0;

        for (int i = 0; i < (7 - itter); i++) {
            clock_SR();
        }

        DATA = 1;
        clock_SR();

        DATA = 0;
        for (int i = (7 - itter); i < 7; i++) {
            clock_SR();
        }
        for (int i = 0; i < (7 - itter); i++) {
            clock_SR();
        }

        DATA = 1;
        clock_SR();

        DATA = 0;
        for (int i = (7 - itter); i < 7; i++) {
            clock_SR();
        }

        push_output();
        USER_INPUT[itter] = OUTPUT;
        itter++;

        if (itter == 8) {
            itter = 0;
            decode_value(USER_INPUT);

        }
        
        
    }
}

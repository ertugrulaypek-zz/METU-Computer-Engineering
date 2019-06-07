// Ertugrul Aypek, 2171270
// Mustafa Badıllı, 2171296


#include <p18cxxx.h>
#include <xc.h> //#include <p18f8722.h>
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#define _XTAL_FREQ   40000000

#include "Includes.h"
#include "LCD.h"
#include<stdio.h>


void sevenDisplay(int ,int ,int ,int);

char * adcReturn(int);

int waitRe1Flag = 0;
int voters[2] = {0,0}; // displaying voters' amount
int timer1Counter=90; // voting time
unsigned int tmr1default;
unsigned int tmr0default;
unsigned int timer0counterDefault;
unsigned int timer0counter;

 

int sevenSegment[]={
0b00111111,                     // 0
0b00000110,                     // 1
0b01011011,                     // 2
0b01001111,                     // 3
0b01100110,                     // 4
0b01101101,                     // 5
0b01111101,                     // 6
0b00000111,                     // 7
0b01111111,                     // 8
0b01101111,                     // 9
};

const char* adaylar[8] = {
    "Poe", // 0-127
    "Galib",// 128-255
    "Selimi",// 256-383
    "Nesimi",// 384-511
    "Hatayi",// 512-639
    "Zweig",// 640-767
    "Nabi",// 768-895
    "Austen"// 896-1023
};

void initConfiguration(){ 
  /* sonu default ile biten variabları her bi isr'den sonra tekrar default hale getirmek icin*/

    TRISH4 = 1;                     // A/D port input setting
    ADCON0 = 0b00110000;            // set chanel 1100 => chanel12
    ADCON1 = 0b00000000;            // set voltage limits and set analog all.
    ADCON2 = 0b10001111;            // right justified
    PIE1bits.ADIE = 1;              // A/D interrupt enable
    ADON=1;                         // 

    TRISE = 0x02;                   // RE1 input
    INTCONbits.TMR0IE = 1;          // enable TMR0 interrupts
    INTCONbits.TMR0IF = 0;          // clear timer0 interrupt flag

    TMR1IE=1;

    
    

    TRISB6 = 1;                     // set RB6 as input pin to use PORTB interrupt
    TRISB7 = 1;                     // set RB7 as input pin to use PORTB interrupt
    PORTB = 0;                      // clear PORTB in order to avoid unexpected situations
    LATB = 0;                       // clear LATB in order to avoid unexpected situations
    INTCONbits.RBIE = 1;            // enable PORTB change interrupts
    INTCONbits.RBIF = 0;            // clear PORTB interrupt flag
    INTCON2bits.RBPU = 0 ;           // PORTB pull-ups are enabled by individual port latch values

    T1CON = 0b10110000;
    tmr1default = 0;                // timer1 initial value configuration AYARLANACAK!!
    TMR1 = tmr1default;
    
    T0CON = 0b01000111;             // set pre-scaler 1:256, use timer0 as 8-bit
    timer0counterDefault = 20;       // counter for timer0 100ms (1000000 instruction => 256 * 20 *(256-60) )
    timer0counter = timer0counterDefault;     // setTimer0Counter
    tmr0default = 0;                // timer0 initial value configuration for 100ms
    TMR0 = tmr0default;              // set timer0's initial value

   

    INTCONbits.GIE_GIEH = 1;        // enable global interrupts
    INTCONbits.PEIE_GIEL = 1;       // enable peripheral interrupts

    ADON = 1;                       // enable A/D conversion module
    ADIF = 0;                       // clear A/D interrupt flag
    ADIE = 1;                       // enable A/D interrupts

    TRISA = 0x00;
    LATA2 = 1;

    TMR0ON = 1;
    TMR1ON = 1;

    TRISJ = 0;                      // Seven segment as output
    LATJ = 0;                       // clear 
    LATH = 0;                       // clear
    TRISH = TRISH & 0b11110000;     // PORTH<3:0> as output

    LATH = LATH & 0xF0;

}

void init() {

    initConfiguration(); // conf. timers and other things..
    WriteCommandToLCD(0x80);   // First line
    WriteStringToLCD(" #Electro Vote#");    // Write Hello World on LCD
    WriteCommandToLCD(0xC0); // Second line
    WriteStringToLCD(" ############## ");   // Write Hello World on LCD
    while(!waitRe1Flag){
        if(PORTEbits.RE1 == 0){ // pressed
            while(1){
                if(PORTEbits.RE1 == 1){
                    waitRe1Flag = 1;
                    break;
                }
            }
            // 3 sec. No built-in func used
            for(int i = 0; i < 545; i++){
                for(int j = 0; j < 5000; j++){
                    //__delay_ms(10);
                }
            }
        }
    }
              

}


// Design of seven segment display in a cyclic manner: (Unluckily, could not be used)
void sevenSegmentDisplay(int d0,int d1,int d2,int d3){
    LATH = LATH & 0xF0;

    LATJ = sevenSegment[d0];                     // Set LATJ to d0 parameter
    LATH0 = 1;                          // Turn on D0 of 7-segment display
    __delay_us(50);                    // wait for shortly
    LATH0 = 0;

    LATJ = sevenSegment[d1];                     // Set LATJ to d1 parameter
    LATH1 = 1;                          // Turn on D1 of 7-segment display
    __delay_us(50);                    // wait for shortly
    LATH1 = 0;

    LATJ = sevenSegment[d2];                     // Set LATJ to d2 parameter
    LATH2 = 1;                          // Turn on D2 of 7-segment display
    __delay_us(50);                    // wait for shortly
    LATH2 = 0;

    LATJ = sevenSegment[d3];                     // Set LATJ to d3 parameter
    LATH3 = 1;                          // Turn on D3 of 7-segment display
    __delay_us(50);                    // wait for shortly
    LATH3 = 0;
}


char * adcReturn(int val){
    if(val>=0 && val<128 ){
        return adaylar[0] ;
    }
    if(val>=128 && val<256 ){
        return adaylar[1];
    }
    if(val>=256 && val<384 ){
        return  adaylar[2];
    }
    if(val>=384 && val<512 ){
        return adaylar[3];
    }
    if(val>=512 && val<640 ){
        return adaylar[4];
    }
    if(val>=640 && val<768 ){
        return adaylar[5];
    }
    if(val>=768 && val<896 ){
        return adaylar[6];
    }
    if(val>=896 && val<1024 ){
        return adaylar[7];
    }
}

void enterVoters(){
    ClearLCDScreen();
    char nofvoters[14];
    ClearLCDScreen(); 
    WriteCommandToLCD(0x80);
    WriteStringToLCD(" #Enter Voters#");
    WriteCommandToLCD(0xC0);
    
    sprintf(nofvoters," %d%d          ",voters[0],voters[1]);
    WriteStringToLCD(nofvoters);

    // RB6 on change ..
}

void beginVote(){

    char time90left[14];
    ClearLCDScreen(); 

    WriteCommandToLCD(0x80);
    if(timer1Counter>10){
        sprintf(time90left,"  Time left :%d",timer1Counter);
    }
    else{
        sprintf(time90left,"  Time left :%d%d",0,timer1Counter);
    }
    WriteStringToLCD(time90left);

    // must be filled

}

void __interrupt() my_isr(void){

    return;

}

void main(void){
    
    InitLCD();
    ClearLCDScreen();
    init();
    enterVoters();
    return;
}
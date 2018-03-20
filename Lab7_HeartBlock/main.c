// 0.Documentation Section 
// Lab7_HeartBlock, main.c

// Runs on LM4F120 or TM4C123 LaunchPad
// Input from PF4(SW1) is AS (atrial sensor), 
// Output to PF3, Green LED, is Ready,
// Output to PF1, Red LED, is VT (ventricular trigger) 
// Make PF4 input, PF3,PF1 output
// Initialize Ready to high and VT to low
// Repeat this sequence of operation over and over
// 1) Wait for AS to fall (touch SW1 switch)
// 2) Clear Ready low
// 3) Wait 10ms (debounces the switch)
// 4) Wait for AS to rise (release SW1)
// 5) Wait 250ms (simulates the time between atrial and ventricular contraction)
// 6) set VT high, which will pulse the ventricles 
// 7) Wait 250ms
// 8) clear VT low 
// 9) set Ready high

// Date: January 15, 2016

// 1. Pre-processor Directives Section
#include "TExaS.h"
#include<stdio.h>
#include<stdlib.h>

// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
// 2. Declarations Section
//   Global Variables

//   Function Prototypes
void PortFInit(void);             // port F initial function
void Delay1ms(unsigned long msec); // delay function
void EnableInterrupts(void);       // enable interrupts
void WaitForAsLow(void);           // sensor-off waiting function
void WaitForAsHigh(void);          // sensor-on waiting function
void SetVt(void);                  // trigger setting function
void ClearVt(void);                // trigger clearing function
void SetReady(void);               // indicator setting function
void ClearReady(void);             // indicator clearing function
 

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  TExaS_Init(SW_PIN_PF40, LED_PIN_PF31,ScopeOn);  // activate grader and set system clock to 80 MHz
  PortFInit();                            // Init port PF4 PF3 PF1    
  EnableInterrupts();                      // enable interrupts for the grader  
   while (1) {
        SetReady();      
        WaitForAsHigh(); 
        ClearReady();    
        Delay1ms(10);    
        WaitForAsLow();  
        Delay1ms(250);   
        SetVt();         
        Delay1ms(250);   
        ClearVt();       
    }
  }

void PortFInit(void) {
    volatile unsigned long delay;
 
    SYSCTL_RCGC2_R    |= 0x00000020;     // 1) F clock
    delay              = SYSCTL_RCGC2_R; // delay to allow clock to stabilize
    GPIO_PORTF_LOCK_R  = 0x4C4F434B;     // 2) unlock Port F
    GPIO_PORTF_CR_R   |= 0x1A;           // allow changes to PF4, PF3, PF1
    GPIO_PORTF_AMSEL_R = 0x00;           // 3) disable analog function
    GPIO_PORTF_PCTL_R  = 0x00;           // 4) GPIO clear bit PCTL
    GPIO_PORTF_DIR_R  |= 0x0A;           // 5) PF4 is input. PF3, PF1 are outputs
    GPIO_PORTF_AFSEL_R = 0x00;           // 6) no alternate function
    GPIO_PORTF_PUR_R  |= 0x10;           // enable pullup resistor on PF4
    GPIO_PORTF_DEN_R  |= 0x1A;           // 7) enable digital pins PF4, PF3, PF1
}
 

void WaitForAsLow(void) {
    unsigned long as;                 
 
    do {
        as = GPIO_PORTF_DATA_R & (1<<4); 
    } while (!as);                    
}
 
void WaitForAsHigh(void) {
    unsigned long as;                  
 
    do {
        as = GPIO_PORTF_DATA_R & (1<<4); 
    } while (as);                      
}
void SetVt(void) {
    GPIO_PORTF_DATA_R |= 0x02; 
}
void ClearVt(void) {
    GPIO_PORTF_DATA_R &= (0<<1);
}
 
void SetReady(void) {
    GPIO_PORTF_DATA_R |= 0x08; 
}
 
void ClearReady(void)
	{
    GPIO_PORTF_DATA_R &= (0<<3); 
}
 

void Delay1ms(unsigned long msec) {
    unsigned long i; 
 
    while (msec) {
        i = 13333 ;  
 
        while (i) {
            i--;     
        }
        msec--;      
    }
}
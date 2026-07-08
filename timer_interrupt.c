#include "timer.h"

// global variables
volatile u8 timer_fired = 0;
volatile u32 seconds_count = 0;



void Timer0_ISR(void) __irq {
    
    seconds_count++;
    
    // 900 seconds = 15 minutes
    if (seconds_count >= 30) {  //use 900
        timer_fired = 1;   
	   T0TCR = (1 << 1);    
    }
    
    T0IR = (1 << 0);  
     VICVectAddr = 0;         
}


void Init_Timer0_Interrupt(void) {
    T0TCR = (1 << 1);         
    T0PR  = 14999;            
    
    
    T0MR0 = 1000;             
    
   
    T0MCR = (1 << 0) | (1 << 1); 
    
    
    VICVectAddr2 = (unsigned long)Timer0_ISR; // Assign ISR to Slot 0
    VICVectCntl2 = (1 << 5) | 4;              // Enable Slot 0 & assign to Timer 0 (Channel 4)
    VICIntEnable = (1 << 4);                  // Globally enable Timer 0 Interrupt
}

// Start the countdown
void Start_Scroll_Timer(void) {
    timer_fired = 0;          // Reset flag
    seconds_count = 0;        // Reset second counter
    T0TCR = (1 << 1);         // Clear physical timer registers
    T0TCR = (1 << 0);         // Start Timer 0 counting
}


void Stop_Scroll_Timer(void) {
    T0TCR = (1 << 1);         // Disable and reset
}

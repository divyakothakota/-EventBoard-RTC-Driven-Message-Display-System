#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "rtc.h"
#include "lcd.h"

void rtc_edit(void);
	
#define EINT0 14
void eint0_isr(void)__irq{
	
	rtc_edit();
	
	EXTINT=1<<0;
	
	VICVectAddr=0;
	
	
}

void enable_eint0(void){

		PINSEL0|=0x0000000C;
		VICIntSelect=0<<EINT0;
	  VICIntEnable=1<<EINT0;
	  VICVectCntl0=(1<<5)|EINT0;
		VICVectAddr0=(unsigned int)(eint0_isr);
	
		EXTMODE |= (1 << 0);    // Set EINT0 to Edge Sensitive (1)
    EXTPOLAR &= ~(1 << 0); // Set to Falling Edge (0) because it's Active Low
  
}


#include <lpc21xx.h>
#include "types.h"
#include "lcd.h"
#include "rtc.h"
#include "global.h"
#include "msg.h"
#include "timer.h"

s32 i=0;

volatile int startScroll = 0; 

void check_enable(void){
	for(i=0;i<TOTAL_MESSAGES;i++){
				if(((messageList[i].hour) ==HOUR) && ((messageList[i].minute)==MIN) && (SEC==0)){
						
					if(messageList[i].enabled){ 
							  startScroll=1;
							  Start_Scroll_Timer();
								break;
				}
			}
		}
	}


void rtc_alarm_isr(void) __irq {
   
			check_enable();
	
			ILR=0X03;
			VICVectAddr = 0;           
}


void enable_rtc_alarm(void) {
   
			CIIR=1<<1;
			ILR=0x03;

    
    VICIntSelect &= ~(1 << 13);           // RTC as IRQ
    VICVectAddr1 = (unsigned int)rtc_alarm_isr; 
    VICVectCntl1 = (1 << 5) | 13;         // Enable Slot 1 and link to RTC
    VICIntEnable = (1 << 13);             // Enable RTC Interrupt
}

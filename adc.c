#include <lpc21xx.h>
#include "types.h"
#include "adc_defines.h"
#include "delay.h"
#include "lcd.h"

void InitADC(void){
	PINSEL1|=1<<24;
	ADCR=PDN_BIT|CLKDIV;
}


void ReadADC(u32 chno,f32* eAR,u32 *AdcDval){
		ADCR&=~(255<<0);
	  ADCR|=chno|STA_CONV;
		
		delay_us(3);
	
		while(((ADDR>>DONE_BIT)&1)==0);
	
		ADCR&=~(STA_CONV);
	
		*AdcDval=((ADDR>>RESULT)&1023);
	  *eAR=((3.3/1023)*(*AdcDval));
}

void DisplayADC(float *eAR){
		float temp;
		//temp=0.25*100;
		temp= (*eAR * 100.0);
		CmdLCD(0xC0+11);
		U32LCD(temp);
		CharLCD(223);
	 CharLCD('C');
}

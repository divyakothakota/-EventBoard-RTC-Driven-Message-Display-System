#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "defines.h"
#include "lcd_defines.h"

void WriteLCD(u8 data){
	SCLRBIT(IOCLR0,RW);//R/W=0 for write operations
	WRITEBYTE(IOPIN0,LCD_DATA,data);
	SSETBIT(IOSET0,EN);
	delay_us(1);
	SCLRBIT(IOCLR0,EN);
	delay_ms(2);
	
}

void CmdLCD(u8 cmd){
	SCLRBIT(IOCLR0,RS);
	WriteLCD(cmd);
}

void CharLCD(u8 ch){
	SSETBIT(IOSET0,RS);
	WriteLCD(ch);
}
void InitLCD(void){
	WRITEBYTE(IODIR0,LCD_DATA,0XFF);
	SETBIT(IODIR0,RS);
	SETBIT(IODIR0,RW);
	SETBIT(IODIR0,EN);
	
	delay_ms(15);
	CmdLCD(0x30);
	delay_ms(5);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30);
	
	CmdLCD(0x38);
	CmdLCD(0x0c);
	CmdLCD(0x01);
	CmdLCD(0x06);
	
}

void StrLCD(s8* str){
	while(*str){
		CharLCD(*str++);
	}
}

void U32LCD(s32 num){
	s32 i=0;
	s8 buf[100];
	if(num<0){
		CharLCD('-');
		num=-num;
	}
	if(num==0){
		CharLCD('0');
		return;
	}
	while(num){
		buf[i++]=(num%10)+'0';
		num/=10;
	}
	buf[i]='\0';
	for(--i;i>=0;i--){
		CharLCD(buf[i]);
	}
}

void FloatLCD(f32 fnum,s32 dp){
	s32 num;
	if(fnum<0.0){
		CharLCD('-');
		fnum=-fnum;
	}
	num=fnum;
	U32LCD(num);
	CharLCD('.');
	while(dp){
		fnum=fnum-num;
		fnum*=10;
		num=fnum;
		CharLCD(num+'0');
		dp--;
	}
	
}

void BinLCD(u32 n,u8 nbd){
	s32 i=0;
	for(i=(nbd-1);i>=0;i--){
		CharLCD(((n>>i)&1)+'0');
	}
}

void Buildcgram(u8* cg,u8 bytes){
	s32 i=0;
	CmdLCD(0x40);
	for(i=0;i<bytes;i++){
		CharLCD(cg[i]);
	}
	CmdLCD(0x80);
}


#include <lpc21xx.h>
#include "defines.h"
#include "types.h"
#include "delay.h"
#include "lcd.h"
#include "kpm_defines.h"

u8 kpmLUT[4][4]={	{'1','2','3','+'},
									{'4','5','6','-'},
									{'7','8','9','*'},
									{'C','0','O','/'}};
void Initkpm(){
		WRITENIBBLE(IODIR1,ROW0,0xf);
}

u8 colscan(void){
	if((READNIBBLE(IOPIN1,COL0))<15)
		return 0;
	else
		return 1;
}

u8 rowcheck(void){
	u8 rno;
	
	for(rno=0;rno<=3;rno++){
		WRITENIBBLE(IOPIN1,ROW0,~(1<<rno));
		if(colscan()==0){
			break;
		}
	}
	WRITENIBBLE(IOPIN1,ROW0,0X00);
	return rno;
}

u8 colcheck(void){
	u8 cno;
	for(cno=0;cno<=3;cno++){
	if((STATUSBIT(IOPIN1,(COL0+cno)))==0){
		break;
		}
	}
	return cno;
}

u8 keyscan(void){
	u8 rno,cno,keyno;
	
	while(colscan()){}
	
	 delay_ms(20); //new chnage
	
	rno=rowcheck();
	
	cno=colcheck();
	
	keyno=kpmLUT[rno][cno];
	
	 while(!colscan()); //new change
	
	return keyno;
}

u32 ReadNum(void)
{
	u8 key;
	u32 sum=0;
	while(1)
	{
		CmdLCD(0xc0);
	key=keyscan();
	if((key>='0')&&(key<='9'))
	{
		
		CharLCD(key);
		sum=(sum*10)+(key-48);
		
	}
	else if(key=='O')
		break;
	}
	return sum;
}

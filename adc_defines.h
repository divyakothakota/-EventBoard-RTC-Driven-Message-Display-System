
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define ADCLK 3000000
#define CLK_DIV ((PCLK/ADCLK)-1)

#define CHNO1 0X01
#define CHNO2 0X02
#define CHNO3 0X04
#define CHNO4 0X08

#define PDN_BIT (1<<21)
#define STA_CONV (1<<24)
#define CLKDIV (CLK_DIV<<8)

#define RESULT 6
#define DONE_BIT 31 

#define AIN0 0x01400000
/*#define AIN1 0X01000000
#define AIN2 0X04000000
#define AIN3 0X10000000	*/



#include <lpc21xx.h>
#include "types.h"


extern volatile u8 timer_fired; 

void Init_Timer0_Interrupt(void);
void Start_Scroll_Timer(void);
void Stop_Scroll_Timer(void);


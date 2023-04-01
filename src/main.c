
#include "MCAL/GPIO.h"
#include "MCAL/RCC.h"
#include "LED.h"
#include "SSegment.h"
#include "Switch.h"
#include "NVIC.h"
#include "Clock_Control.h"
#include "SysTick.h"
#include "Sched.h"

extern void Init_F(void);
int main()
{
	Init_F();
	Sched_Init();
	Sched_Start();

	return 0 ;
}




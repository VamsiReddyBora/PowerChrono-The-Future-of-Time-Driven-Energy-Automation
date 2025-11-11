#include "main.h"

extern ui32 device_on_hour; 
extern ui32 device_off_hour ;
extern ui32 device_on_min;
extern ui32 device_off_min;
extern ui32 device_on_sec;
extern ui32 device_off_sec;

void Eint_Init(void){
	CfgPortPinFunc(0,16,PIN_FUNC2);
	VICIntEnable=1<<EINT0_VIC_CHNO;
  VICVectCntl0 = (1<<5)|EINT0_VIC_CHNO;
	VICVectAddr0 = (ui32 ) eint0_isr;
	EXTMODE = 1<<0;
	IODIR1 |= 1<<EINT0_STATUS_LED;
}

/*void Eint_Init2(void){
	CfgPortPinFunc(0,14,PIN_FUNC3);
	VICIntEnable=1<<15;
  VICVectCntl0 = (1<<5)| 15;
	VICVectAddr0 = (ui32 ) eint0_isr2;
	EXTMODE = 1<<0;
	//IODIR1 |= 1<<EINT0_STATUS_LED;
}*/

void eint0_isr(void) __irq{
	EXTINT = 1<<0;
	VICVectAddr = 0;
	//write what to do
	//Eint_Init2();
	edit();
	//EXTINT = 1<<0;
	//VICVectAddr = 0;
}

/*void eint0_isr2(void) __irq{
	EXTINT = 1 << 0;
	VICVectAddr = 0;
	
	PrintONTime(device_on_hour, device_on_min, device_on_sec);
	PrintOFFTime(device_off_hour, device_off_min, device_off_sec);
	delay_s(1);
	COMMAND(CLEAR_LCD);
}*/

#include "main.h"

extern ui32 device_on_hour; 
extern ui32 device_off_hour ;
extern ui32 device_on_min;
extern ui32 device_off_min;
extern ui32 device_on_sec;
extern ui32 device_off_sec;
extern ui32 IRQ_fired;
extern ui32 interrupt_fired;

void Eint_Init(void){
	CfgPortPinFunc(0,16,PIN_FUNC2);
	VICIntEnable=1<<EINT0_VIC_CHNO;
  VICVectCntl0 = (1<<5)|EINT0_VIC_CHNO;
	VICVectAddr0 = (ui32 ) eint0_isr;
	EXTMODE = 1<<0;
	IODIR1 |= 1<<EINT0_STATUS_LED;
}

void Eint_FIQ(){
	PINSEL1 &= 0xCFFFFFFF; //Clearing p0.30 
	PINSEL1 |= (2 << 28); // p0.30 as EINT3
	EXTINT = 1 << 3; //Clearing Interrupt flag
	VICIntEnable |= 1 << EINT3_VIC_CHNO; // Enabling EINT3
	VICIntSelect |= 1 << EINT3_VIC_CHNO; // Setting as FIQ type
	EXTMODE |= (1 << 3); // Edge sensitive
  EXTPOLAR &= ~(1 << 3); // Falling edge
}

void FIQ_ISR(void){
	EXTINT = 1 << 3; //Clearing Interrupt Flag after fires
	interrupt_fired  = 1; //means the interrupt fired, usefull in main program
	COMMAND(CLEAR_LCD); //Clearing LCD Before going to HOME SCREEN
	IRQ_fired = 0; //Must be cleared
}

void eint0_isr(void) __irq{
	EXTINT = 1<<0;
	VICVectAddr = 0;
	IRQ_fired = 1;
}

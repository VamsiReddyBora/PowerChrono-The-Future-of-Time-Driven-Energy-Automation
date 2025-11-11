//Just include Mini project.h
//pin_connect_block.c
#include "types.h"
#include <LPC21xx.h>

void CfgPortPinFunc(ui32 portNo,ui32 pinNo,ui32 pinFunc)
{
	if(portNo==0)
	{
		if(pinNo<=15)
		{
			PINSEL0=((PINSEL0&~(3<<(pinNo*2)))|
			         (pinFunc<<(pinNo*2)));
		}
		else if(pinNo>=16 && pinNo<=31)
		{
			PINSEL1=((PINSEL1&~(3<<((pinNo-16)*2)))|
			         (pinFunc<<((pinNo-16)*2)));
		}
	}
	else if(portNo==1)
	{
   	if(pinNo>=16 && pinNo<=31)
		{
			PINSEL2=((PINSEL2&~(3<<((pinNo-16)*2)))|
			         (pinFunc<<((pinNo-16)*2)));
		}
		
	}
}

void ReservePins(ui32 port,ui32 pins,ui32 from,ui32 dir){
	if(port == 0){
		while(pins--){
			if((from < 32)){
					if(dir){
						IODIR0 |= (1 << from);
					}
					else{
						IODIR0 &= ~(1 << from);
					}
					from++;
			}
		}
	}
	else if(port == 1){
		while(pins--){
			if((from > 15) && (from < 32)){
					if(dir){
						IODIR1 |= (1 << from);
					}
					else{
						IODIR1 &= ~(1 << from);
					}
					from++;
			}
		}
	}
}

#include "delay.h"

int delay_s(unsigned int d_s){
	d_s *= 12000000;
	while(d_s--);
	return 0;
}

int delay_ms(unsigned int d_ms){
	d_ms *= 12000;
	while(d_ms--);
	return 0;
}

int delay_us(unsigned int d_us){
	d_us *= 12;
	while(d_us--);
	return 0;
}

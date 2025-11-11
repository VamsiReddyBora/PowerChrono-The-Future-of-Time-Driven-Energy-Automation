#include<LPC21xx.h>
#include "pin_connect_block.h"
#include "pin_function_defines.h"
#include "delay.h"
#include "lcd_functions.h"
#include "lcd_defines.h"
#include "rtc_functions.h"
#include "eint_defines.h"
#include "eint_functions.h"
#include "led.h"
#include "keypad.h"
#include "keymatrix.h"
#include "types.h"

#define SW (IOPIN0 & (1 << 14))

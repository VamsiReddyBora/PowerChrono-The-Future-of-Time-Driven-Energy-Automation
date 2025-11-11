#include <LPC21xx.h>             // Header file for LPC21xx series microcontrollers (defines all SFRs)
#include "defines.h"              // Contains macros like SETBIT(), CLRBIT(), etc.
#include "led.h"                  // LED-related functions or macros (possibly defines P1.18 usage)
#include "lcd_functions.h"        // LCD display function prototypes (DATA(), LCD_SetCursor(), etc.)
#include "lcd_defines.h"          // LCD command definitions and control pin constants

//============================ FUNCTION: Device_ON ============================//
// Purpose : Turns ON the device connected to Port 1.18 and updates LCD status
// Arguments : None
// Return : None
void Device_ON(void){
	SETBIT(IOSET1,18);            // Set bit 18 of Port1 ? drives output HIGH ? device ON (relay or LED)
	LCD_SetCursor(11,0);          // Position LCD cursor to column 11, line 0 (first row)
	DATA("DEV:");                 // Display static label "DEV:" on LCD
	LCD_SetCursor(15,0);          // Move cursor to column 15, line 0 (last column)
	DATA_char(0);                 // Display custom character stored in CGRAM location 0 (symbol for “ON”)
}

//============================ FUNCTION: Device_OFF ============================//
// Purpose : Turns OFF the device connected to Port 1.18 and updates LCD status
// Arguments : None
// Return : None
void Device_OFF(void){
	CLRBIT(IOCLR1,18);            // Clear bit 18 of Port1 ? drives output LOW ? device OFF
	LCD_SetCursor(11,0);          // Move LCD cursor to column 11, line 0
	DATA("DEV:");                 // Display static label “DEV:” on LCD
	LCD_SetCursor(15,0);          // Move cursor to column 15, line 0 (same position as Device_ON)
	DATA_char(1);                 // Display custom character stored in CGRAM location 1 (symbol for “OFF”)
}


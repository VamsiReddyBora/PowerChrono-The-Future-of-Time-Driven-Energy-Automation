#include "lcd_defines.h"     // LCD command/address definitions
#include "lcd_functions.h"   // LCD function prototypes
#include "types.h"           // Project-specific typedefs (ui32, uc8, etc.)
#include "delay.h"           // delay_ms(), delay_us()
#include "defines.h"         // System-wide macros/defines (pins, masks, constants)
#include "keymatrix.h"       // get_key() matrix keypad scanner

ui32 timeout = TIMEOUT;      // Global timeout counter initialized to default TIMEOUT

extern ui32 device_on_hour;  // Extern ON time - hour (set elsewhere)
extern ui32 device_off_hour; // Extern OFF time - hour
extern ui32 device_on_min;   // Extern ON time - minute
extern ui32 device_off_min;  // Extern OFF time - minute
extern ui32 device_on_sec;   // Extern ON time - second
extern ui32 device_off_sec;  // Extern OFF time - second

//Initializing LCD
void InitializeLCD(void){
	delay_ms(15);                               // Mandatory power-up delay for HD44780
	COMMAND(0x30);                              // Force 8-bit mode (init step 1)
	delay_ms(4);                                // Wait >4.1ms
	delay_us(100);                              // Extra settle time
	COMMAND(0x30);                              // Force 8-bit mode (init step 2)
	delay_us(100);                              // Extra settle time
	COMMAND(0x30);                              // Force 8-bit mode (init step 3)
	COMMAND(MODE_8BIT_2LINE);                   // Function set: 8-bit, 2-line, 5x8 dots
	COMMAND(DSP_ON_CUR_OFF);                    // Display ON, Cursor OFF
	COMMAND(CLEAR_LCD);                         // Clear display & home cursor
	COMMAND(SHIFT_CUR_RIGHT);                   // Entry mode: increment cursor
}

//For giving Commands to the LCD
void COMMAND(uc8 cmd){
	WRITEBYTE(IOCLR0,0x00,0xFF);                // Clear data bus bits on port0
	WRITEBYTE(IOSET0,0x00,cmd);                 // Put command byte on data bus
	cmd_mode;                                   // RS=0, RW=0 (command write)
	write_lcd;                                  // Assert control lines for write
	enable_lcd();                               // E strobe to latch into LCD
	delay_ms(2);                                // Wait for instruction execution
}

//For printing character on to the screen
void DATA_char(uc8 ch){
	WRITEBYTE(IOCLR0,0x00,0xFF);                // Clear data bus bits on port0
	WRITEBYTE(IOSET0,0x00,ch);                  // Put data byte on data bus
	data_mode;                                  // RS=1, RW=0 (data write)
	write_lcd;                                  // Assert control lines for write
	enable_lcd();                               // E strobe to latch character
	delay_ms(2);                                // Wait for data write cycle
}

//For printing string on to the screen
void DATA(c8 *ptr){
	int i;                                      // Index through string
	WRITEBYTE(IOCLR0,0x00,0xFF);                // Clear data bus before starting
	for(i = 0; ptr[i]; i++){                    // Iterate until null terminator
		IOSET0 |= ptr[i];                       // Put next character on bus
		data_mode;                              // RS=1 for data
		write_lcd;                              // Start write
		enable_lcd();                           // Latch with E strobe
		delay_ms(2);                            // Interchar delay for LCD
	  WRITEBYTE(IOCLR0,0x00,0xFF);             // Clear bus between chars
	}
}

//Enable pin latching
void enable_lcd(){
	SETBIT(IOSET0,LCD_EN);                      // Set E=1 (start latch window)
	delay_us(2);                                // Short enable pulse width
	CLRBIT(IOCLR0,LCD_EN);                      // Set E=0 (capture data/command)
	delay_us(2);                                // Enable cycle closing delay
}

//For printing unsigned integer on to the screen
void U32LCD(ui32 n){
	i16 i=0;                                    // Digit stack index
	uc8 a[10];                                  // Buffer for up to 10 digits
	if(n==0){                                   // Special case zero
		DATA_char('0');                         // Print single '0'
	}
	else{
		while(n>0){                             // Extract digits in reverse
		  a[i++]=(n%10)+48;                     // Store ASCII of last digit
      n/=10;			                           // Drop last digit
		}
		for(--i;i>=0;i--)                       // Print digits in forward order
		  DATA_char(a[i]);                      // Output one by one
	}
}

//LCD Custom char intialization by storing message at CGRAM Location 0x40
void LCD_Custom_Char(uc8 location, uc8 *msg){
    uc8 i;                                     // Byte index within pattern
    if(location < 8){                           // HD44780 supports 8 custom chars (0..7)
        COMMAND(0x40 + (location * 8));         // Set CGRAM base address for this slot
        for(i = 0; i < 8; i++)                  // Each custom char has 8 rows
            DATA_char(msg[i]);                  // Write row pattern
    }
}

//To move LCD cursor to any one of the position and any one of the line
void LCD_SetCursor(uc8 col, uc8 row){
    uc8 addr;                                   // DDRAM address to compute
    if(row == 0)
        addr = 0x80 + col;                      // Line 1 base 0x80 + column
    else if(row == 1)
        addr = 0xC0 + col;                      // Line 2 base 0xC0 + column
    else
        return;                                 // Invalid row: do nothing
    COMMAND(addr);                              // Issue Set DDRAM Address
}

//Menu 1 deals with Editing RTC Time and Editing ON/OFF times
void edit(){
	START:COMMAND(CLEAR_LCD);                    // Start label: clear screen
	delay_ms(5);                                 // Small UI delay
	COMMAND(GOTO_LINE1_POS0);                    // Move to line1,col0
	DATA("1.E.TIME");                            // Option 1 label
	COMMAND(GOTO_LINE1_POS0 + 10);               // Move to line1,col10
	DATA("3.BACK");                              // Option 3 label
	COMMAND(GOTO_LINE2_POS0);                    // Move to line2,col0
	DATA("2.E.ONOFF TIME");                      // Option 2 label
	
	while(timeout--){                            // UI loop with timeout countdown
		uc8 keypress;                            // Stores pressed key
		keypress = get_key();                    // Poll keypad
		if(keypress != '\0'){                    // If any key detected
			timeout = TIMEOUT;                   // Reset inactivity timer
			switch(keypress){                    // Handle menu selection
				case '1':if(edit_time())         // Go to edit_time() submenu
										return;                    // If it returns 1, exit menu
									goto START;                   // Else redraw current menu
						
				case '2':if(edit_on_off_time())  // Go to ON/OFF time submenu
										return;                    // Exit if requested
								 goto START;                    // Redraw otherwise
					
				case '3':COMMAND(CLEAR_LCD);      // Back to previous context
								return;                          // Exit this menu
				
				case '*':COMMAND(CLEAR_LCD);      // Emergency exit shortcut
								 DATA(" EMERGENCY EXIT");       // Notify user
								 delay_ms(500);                 // Brief message hold
								 COMMAND(CLEAR_LCD);            // Clear again
								 return;                        // Exit
				
				default:COMMAND(CLEAR_LCD);       // Any other key: invalid
								DATA("INVALID  CHOICE");        // Inform user
								COMMAND(GOTO_LINE2_POS0);       // Move to next line
								DATA("* for EMER.EXIT");        // Show help
								delay_ms(500);                  // Hold message
								goto START;                     // Redraw menu
			}
		}
	}
	//Timeout delay to exit, if no operation choose for long time
	COMMAND(CLEAR_LCD);                          // Timeout message
	DATA("    TIME OUT!   ");
	delay_ms(250);                               // Brief display
	COMMAND(CLEAR_LCD);                          // Clear
	timeout = TIMEOUT;                           // Reset global timeout
	return;                                      // Exit menu
}

//Menu 2 deals with Editing RTC Information like Time, Date and Day
i32 edit_time(){
	START:COMMAND(CLEAR_LCD);                     // Draw submenu
	delay_ms(5);                                  // UI delay
	COMMAND(GOTO_LINE1_POS0);                     // Line1
	DATA("1.TIME    2.DATE");                     // Show options
	COMMAND(GOTO_LINE2_POS0);                     // Line2
	DATA("3.DAY     4.BACK");                     // More options
	
	while(timeout--){                             // Loop until timeout
		uc8 keypress;                             // Key buffer
		keypress = get_key();                     // Scan keypad
		if(keypress != '\0'){                     // If pressed
			timeout = TIMEOUT;                    // Reset inactivity timer
			switch(keypress){                     // Decode selection
				case '1':if(time_())              // Edit RTC time
										return 1;               // Return to caller with 1
									goto START;                // Otherwise redraw
						
				case '2':if(date_())              // Edit RTC date
										return 1;               // Exit with 1
									goto START;                // Redraw
					
				case '3':if(day_())               // Edit RTC day
										return 1;               // Exit with 1
									goto START;                // Redraw
				
				case '4':COMMAND(CLEAR_LCD);       // Back to previous
								return 0;                      // Indicate normal back
				
				case '*':COMMAND(CLEAR_LCD);       // Emergency exit
								 DATA(" EMERGENCY EXIT");
								 delay_ms(500);
								 COMMAND(CLEAR_LCD);
								 return 1;                     // Signal exit
				
				default:COMMAND(CLEAR_LCD);        // Invalid input message
								DATA("INVALID  CHOICE");
								COMMAND(GOTO_LINE2_POS0);
								DATA("* for EMER.EXIT");
								delay_ms(500);
								goto START;                    // Redraw options
			}
		}
	}
	//Timeout delay to exit, if no operation choose for long time
	COMMAND(CLEAR_LCD);                           // Timeout message
	DATA("    TIME OUT!   ");
	delay_ms(250);
	COMMAND(CLEAR_LCD);
	timeout = TIMEOUT;                            // Reset timeout
  return 1;                                      // Report as exited (timeout)
}

//Menu 3 Edits RTC Hour, Min or Sec Information
i32 time_(){
	i32 hour_count = HOUR, min_count = MIN, sec_count = SEC; // Load current RTC values
	i32 index = 1;                             // 1=hour, 2=min, 3=sec selection
	uc8 keypress;                              // Keypad input
	COMMAND(CLEAR_LCD);                        // Clear screen
	
	BEGIN:COMMAND(GOTO_LINE1_POS0);            // Top banner with arrows
	DATA_char('<');                            // Left arrow custom/ASCII
	COMMAND(GOTO_LINE1_POS0 + 15);             // Far right position
	DATA_char('>');                            // Right arrow
	
	timeout = timeout / 4445;                  // Reduce timeout window for this screen
	
	while(timeout--){                          // UI loop
		COMMAND(GOTO_LINE2_POS0);              // Move to time row start
		DATA_char(2);                          // Custom icon at start (e.g., clock)
		COMMAND(GOTO_LINE2_POS0 + 4);          // Position for HH tens
		if((hour_count >= 0) && (hour_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 4);      // If single digit hour, print leading zero
			DATA_char('0');
		}
		U32LCD(hour_count);                     // Print hours (00-23)
		DATA_char(':');                         // Colon separator
		if((min_count >= 0) && (min_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 7);      // Leading zero for minutes
			DATA_char('0');
		}
		U32LCD(min_count);                      // Print minutes (00-59)
		DATA_char(':');                         // Colon separator
		if((sec_count >= 0) && (sec_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 10);     // Leading zero for seconds
			DATA_char('0');
		}
		U32LCD(sec_count);                      // Print seconds (00-59)
		COMMAND(GOTO_LINE2_POS0 + 3);           // Cosmetic alignment
		LCD_SetCursor(15,1);                    // Put cursor at end of line
		DATA_char(3);                           // Custom icon at end
	
		
		if(index == 1){                         // Highlight text for selection
			COMMAND(GOTO_LINE1_POS0 + 4);
			DATA("HH      ");                  // Indicate editing hours
		}
		else if(index == 2){
			COMMAND(GOTO_LINE1_POS0 + 4);
			DATA("   MM   ");                  // Indicate editing minutes
		}
		else if(index == 3){
			COMMAND(GOTO_LINE1_POS0 + 4);
		  DATA("      SS");                    // Indicate editing seconds
		}
	
		keypress = get_key();                   // Read keypad
		if(keypress != '\0'){
			timeout = TIMEOUT;                  // Reset after activity
			switch(keypress){
				case 'A':if(index == 3){        // Next field (cyclic)
										index = 1;
									}	
								else
										index++;
								break;
				case 'B':if(index == 1){        // Previous field (cyclic)
										index = 3;
									}
									else
										index--;
									break;
				case 'C':if(index == 1){        // Increment field
										if(hour_count == 23){
											hour_count = 0;     // Wrap 23->0
										}
										else
											hour_count++;
									}
									else if(index == 2){
											if(min_count == 59){
												min_count = 0;  // Wrap 59->0
											}
											else
												min_count++;
									}
									else if(index == 3){
											if(sec_count == 59){
												sec_count = 0;  // Wrap 59->0
											}
											else
												sec_count++;
									}
									break;
				case 'D':if(index == 1){        // Decrement field
										if(hour_count == 0){
											hour_count = 23;    // Wrap 0->23
										}
										else
											hour_count--;
									}
									else if(index == 2){
											if(min_count == 0){
												min_count = 59; // Wrap 0->59
											}
											else
												min_count--;
									}
									else if(index == 3){
											if(sec_count == 0){
												sec_count = 59; // Wrap 0->59
											}
											else
												sec_count--;
									}
									break;
									
				case '*':COMMAND(CLEAR_LCD);    // Emergency exit
								 DATA(" EMERGENCY EXIT");
								 delay_ms(500);
								 COMMAND(CLEAR_LCD);
								 return 1;              // Signal abort
									
				case '=': HOUR = hour_count;    // Save edited time to global RTC vars
									MIN = min_count;
									SEC = sec_count;
									COMMAND(CLEAR_LCD);
									DATA(" TIME UPDATED!"); // Confirmation
									delay_ms(500);
									COMMAND(CLEAR_LCD);
									return 0;              // Normal exit
									
			default: COMMAND(CLEAR_LCD);        // Help hint for wrong key
								COMMAND(GOTO_LINE1_POS0);
								DATA("B/A TO NAVIGATE");
								COMMAND(GOTO_LINE2_POS0);
								DATA("C/D TO INCREMENT");
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								goto BEGIN;               // Redraw same screen
				}
			}
	}
	//Timeout delay to exit, if no operation choose for long time
	COMMAND(CLEAR_LCD);                        // Timeout reached
	DATA("    TIME OUT!   ");
	delay_ms(250);
	COMMAND(CLEAR_LCD);
	timeout = TIMEOUT;                         // Reset for next time
	return 1;                                  // Report timeout/abort
}

//Menu 4 Edits RTC Date Information
i32 date_(){
	int date_count = 15, month_count = 6, year_count = 2025; // Defaults for date edit
	int index = 1;                             // 1=DD, 2=MM, 3=YYYY
	unsigned char keypress;                    // Keypress buffer
	COMMAND(CLEAR_LCD);                        // Clear screen
	
	BEGIN:COMMAND(GOTO_LINE1_POS0);            // Draw header with arrows
	DATA_char('<');
	COMMAND(GOTO_LINE1_POS0 + 15);
	DATA_char('>');
	
	timeout = timeout / 4445;                  // Reduce timeout for this menu
	
	while(timeout--){                          // UI loop
		COMMAND(GOTO_LINE2_POS0);              // Start of date display row
		DATA_char(2);                          // Decorative/custom icon
		COMMAND(GOTO_LINE2_POS0 + 3);          // Position before DD
		if((date_count >= 0) && (date_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 3);      // Leading zero for day
			DATA_char('0');
		}
		U32LCD(date_count);                     // Print DD
		DATA_char('/');                         // Separator
		if((month_count >= 0) && (month_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 6);      // Leading zero for month
			DATA_char('0');
		}
		U32LCD(month_count);                    // Print MM
		DATA_char('/');                         // Separator
		if((year_count >= 0) && (year_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 9);      // Leading zeros logic placeholder
			DATA_char('0');
		}
		U32LCD(year_count);                     // Print YYYY
		COMMAND(GOTO_LINE2_POS0 + 3);          // Cosmetic alignment
		LCD_SetCursor(15,1);                    // End marker
		DATA_char(3);                           // Decorative/custom icon
		
		switch(index){                          // Show which field is selected
			case 1: COMMAND(GOTO_LINE1_POS0 + 3);
							DATA("DD        ");  // Day selected
							break;
			case 2: COMMAND(GOTO_LINE1_POS0 + 3);
							DATA("   MM     ");  // Month selected
							break;
			case 3: COMMAND(GOTO_LINE1_POS0 + 3);
							DATA("      YYYY");  // Year selected
							break;
		}
	
		keypress = get_key();                   // Read keypad
		if(keypress != '\0'){
			timeout = TIMEOUT;                  // Reset inactivity
			switch(keypress){
				case 'A':if(index == 3){        // Next field (cyclic)
										index = 1;
									}
								else
										index++;
								break;
				case 'B':if(index == 1){        // Previous field (cyclic)
										index = 3;
									}
									else
										index--;
									break;
				case 'C':if(index == 1){        // Increment with wrap limits
										if(date_count == 31){
											date_count = 1;
										}
										else
											date_count++;
									}
									else if(index == 2){
											if(month_count == 12){
												month_count = 1;
											}
											else
												month_count++;
									}
									else if(index == 3){
											if(year_count == 4095){
												year_count = 1;
											}
											else
												year_count++;
									}
									break;
				case 'D':if(index == 1){        // Decrement with wrap limits
										if(date_count == 1){
											date_count = 31;
										}
										else
											date_count--;
									}
									else if(index == 2){
											if(month_count == 1){
												month_count = 12;
											}
											else
												month_count--;
									}
									else if(index == 3){
											if(year_count == 1){
												year_count = 4095;
											}
											else
												year_count--;
									}
									break;
									
				case '*':COMMAND(CLEAR_LCD);    // Emergency exit
								 DATA(" EMERGENCY EXIT");
								 delay_ms(500);
								 COMMAND(CLEAR_LCD);
								 return 1;              // Abort
									
				case '=': DOM = date_count;     // Save new date to globals
									MONTH = month_count;
									YEAR = year_count;
									COMMAND(CLEAR_LCD);
									DATA(" DATE UPDATED!"); // Confirmation
									delay_ms(500);
									COMMAND(CLEAR_LCD);
									return 0;              // Normal return
									
			default: COMMAND(CLEAR_LCD);        // Help for invalid keys
								COMMAND(GOTO_LINE1_POS0);
								DATA("B/A TO NAVIGATE");
								COMMAND(GOTO_LINE2_POS0);
								DATA("C/D TO INCREMENT");
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								goto BEGIN;               // Redraw
			}
		}
	}
	//Timeout delay to exit, if no operation choose for long time
	COMMAND(CLEAR_LCD);                        // Show timeout message
	DATA("    TIME OUT!   ");
	delay_ms(250);
	COMMAND(CLEAR_LCD);
	timeout = TIMEOUT;                         // Restore timeout baseline
	return 1;                                  // Indicate timeout/abort
}

//Menu 5 Edits RTC Day Information
i32 day_(){
	i32 day_count = DOW;                        // Load current Day-Of-Week (0..6)
	
	c8 day[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"}; // Day names
	
	uc8 keypress;                               // Key buffer
	COMMAND(CLEAR_LCD);                         // Clear
	
	BEGIN:COMMAND(GOTO_LINE1_POS0);             // Header with arrows
	DATA_char('<');
	COMMAND(GOTO_LINE1_POS0 + 15);
	DATA_char('>');
	
	COMMAND(GOTO_LINE2_POS0);                   // Begin content row
	DATA_char(2);                               // Decorative/custom icon
	COMMAND(GOTO_LINE2_POS0 + 3);               // Spacer
	LCD_SetCursor(15,1);                        // Put end marker
	DATA_char(3);                               // Decorative/custom icon
	
	COMMAND(GOTO_LINE1_POS0 + 4);               // Title text
	DATA("WEEK DAY");                           
	
	timeout = timeout / 700;                    // Shorter timeout for this simple menu
	
	while(timeout--){                           // UI loop
	COMMAND(GOTO_LINE2_POS0 + 6);               // Position for day text
	DATA(day[day_count]);                        // Print current day string
	keypress = get_key();                        // Read keypad
	if(keypress != '\0'){
		timeout = TIMEOUT;                       // Reset inactivity timer
		switch(keypress){
			case 'C': if(day_count == 6){        // Next day (wrap)
									day_count = 0;
								}
								else
									day_count++;
								break;
			case 'D': if(day_count == 0){        // Previous day (wrap)
									day_count = 6;
								}
								else
									day_count--;
								break;
								
			case '*':COMMAND(CLEAR_LCD);         // Emergency exit
							  DATA(" EMERGENCY EXIT");
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								return 1;         // Abort
								
			case '=': DOW = day_count;           // Save new DOW
								COMMAND(CLEAR_LCD);
								DATA("  DAY UPDATED!"); // Confirmation
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								return 0;         // Normal return
								
			default: COMMAND(CLEAR_LCD);         // Help for invalid key
								COMMAND(GOTO_LINE1_POS0);
								DATA("C/D TO INCREMENT");
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								goto BEGIN;       // Redraw
		}
	}
}
	//Timeout delay to exit, if no operation choose for long time
	COMMAND(CLEAR_LCD);                          // Timeout message
	DATA("    TIME OUT!   ");
	delay_ms(250);
	COMMAND(CLEAR_LCD);
	timeout = TIMEOUT;                           // Reset for next time
	return 1;                                    // Indicate timeout/abort
}

//Menu 6 deals with Editing/Setting Device ON/OFF Timings
i32 edit_on_off_time(){
	START:COMMAND(CLEAR_LCD);                    // Draw submenu
	delay_ms(5);                                 // UI delay
	COMMAND(GOTO_LINE1_POS0);                    // Line 1
	DATA("1.ONTIME  3.BACK");                    // Options: ON time / Back
	COMMAND(GOTO_LINE2_POS0);                    // Line 2
	DATA("2.OFFTIME");                           // Option: OFF time
	
	while(timeout--){                            // UI loop with timeout
		uc8 keypress;                            // Key buffer
		keypress = get_key();                    // Read keypad
		if(keypress != '\0'){                    // If a key was pressed
			timeout = TIMEOUT;                   // Reset inactivity timer
			switch(keypress){
				case '1':if(edit_on_time())      // Go to ON time editor
								    return 1;     // Exit if it signals to leave
				         goto START;             // Else redraw this menu
						
				case '2':if(edit_off_time())     // Go to OFF time editor
										return 1;  // Exit if signaled
								  goto START;      // Else redraw
				
				case '3':COMMAND(CLEAR_LCD);      // Back
								return 0;           // Normal return
				
				case '*':COMMAND(CLEAR_LCD);      // Emergency exit
								 DATA(" EMERGENCY EXIT");
								 delay_ms(500);
								 COMMAND(CLEAR_LCD);
								 return 1;          // Signal abort
				
				default:COMMAND(CLEAR_LCD);       // Invalid key
								DATA("INVALID  CHOICE");
								COMMAND(GOTO_LINE2_POS0);
								DATA("* for EMER.EXIT");
								delay_ms(500);
								goto START;         // Redraw menu
			}
		}
	}
	//Timeout delay to exit, if no operation choose for long time
	COMMAND(CLEAR_LCD);                           // Timeout message
	DATA("    TIME OUT!   ");
	delay_ms(250);
	COMMAND(CLEAR_LCD);
	timeout = TIMEOUT;                            // Reset timeout
	return 1;                                     // Report timeout/abort
}

//Menu 7 deals with Editing Device ON Time
i32 edit_on_time(){
	int hour_count = device_on_hour, min_count = device_on_min, sec_count = device_on_sec; // Load current ON time
	int index = 1;                               // 1=HH, 2=MM, 3=SS
	unsigned char keypress;                      // Key buffer
	COMMAND(CLEAR_LCD);                          // Clear
	
	BEGIN:COMMAND(GOTO_LINE1_POS0);              // Header with arrows
	DATA_char('<');
	COMMAND(GOTO_LINE1_POS0 + 15);
	DATA_char('>');
	
	timeout = timeout / 4300;                    // Shorter timeout for this screen
	
	while(timeout--){                            // UI loop
		COMMAND(GOTO_LINE2_POS0);                // Content line start
		DATA_char(2);                            // Decorative/custom icon
		COMMAND(GOTO_LINE2_POS0 + 4);            // Position for HH
		if((hour_count >= 0) && (hour_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 4);        // Leading zero for HH<10
			DATA_char('0');
		}
		U32LCD(hour_count);                       // Print hours
		DATA_char(':');                           // Separator
		if((min_count >= 0) && (min_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 7);        // Leading zero for MM<10
			DATA_char('0');
		}
		U32LCD(min_count);                        // Print minutes
		DATA_char(':');                           // Separator
		if((sec_count >= 0) && (sec_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 10);       // Leading zero for SS<10
			DATA_char('0');
		}
		U32LCD(sec_count);                        // Print seconds
		COMMAND(GOTO_LINE2_POS0 + 3);            // Cosmetic spacing
		LCD_SetCursor(15,1);                      // End marker
		DATA_char(3);                             // Decorative/custom icon
		
		switch(index){                            // Show selected field text
			case 1: COMMAND(GOTO_LINE1_POS0 + 4);
							DATA("HH      ");      // Hours selected
							break;
			case 2: COMMAND(GOTO_LINE1_POS0 + 4);
							DATA("   MM   ");      // Minutes selected
							break;
			case 3: COMMAND(GOTO_LINE1_POS0 + 4);
							DATA("      SS");      // Seconds selected
							break;
		}
	
		keypress = get_key();                     // Read keypad
		if(keypress != '\0'){
			timeout = TIMEOUT;                    // Reset inactivity timer
		switch(keypress){
			case 'A':if(index == 3){              // Next field (wrap)
									index = 1;
								}
							 else
									index++;
							 break;
			case 'B':if(index == 1){              // Previous field (wrap)
									index = 3;
								}
								else
									index--;
								break;
			case 'C':if(index == 1){              // Increment selected field
									if(hour_count == 23){
										hour_count = 0;       // Wrap 23->0
									}
									else
										hour_count++;
								}
								else if(index == 2){
										if(min_count == 59){
											min_count = 0;     // Wrap 59->0
										}
										else
											min_count++;
								}
								else if(index == 3){
										if(sec_count == 59){
											sec_count = 0;     // Wrap 59->0
										}
										else
											sec_count++;
								}
								break;
			case 'D':if(index == 1){              // Decrement selected field
									if(hour_count == 0){
										hour_count = 23;      // Wrap 0->23
									}
									else
										hour_count--;
								}
								else if(index == 2){
										if(min_count == 0){
											min_count = 59;    // Wrap 0->59
										}
										else
											min_count--;
								}
								else if(index == 3){
										if(sec_count == 0){
											sec_count = 59;    // Wrap 0->59
										}
										else
											sec_count--;
								}
								break;
								
			case '*':COMMAND(CLEAR_LCD);           // Emergency exit
								DATA(" EMERGENCY EXIT");
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								return 1;           // Abort
								
			case '=': device_on_hour = hour_count; // Save ON time fields
								device_on_min = min_count;
								device_on_sec = sec_count;
								COMMAND(CLEAR_LCD);
								DATA("ON TIME UPDATED!"); // Confirmation
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								return 0;           // Normal return
								
			default: COMMAND(CLEAR_LCD);           // Help for invalid key
								COMMAND(GOTO_LINE1_POS0);
								DATA("B/A TO NAVIGATE");
								COMMAND(GOTO_LINE2_POS0);
								DATA("C/D TO INCREMENT");
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								goto BEGIN;         // Redraw
			}
		}
	}
	//Timeout delay to exit, if no operation choose for long time
	COMMAND(CLEAR_LCD);                           // Timeout message
	DATA("    TIME OUT!   ");
	delay_ms(250);
	COMMAND(CLEAR_LCD);
	timeout = TIMEOUT;                            // Reset timeout baseline
	return 1;                                     // Indicate timeout/abort
}

//Menu 8 deals with Editing Device OFF Time
i32 edit_off_time(){
	int hour_count = device_off_hour, min_count = device_off_min, sec_count = device_off_sec; // Load OFF time
	int index = 1;                               // 1=HH, 2=MM, 3=SS
	unsigned char keypress;                      // Key buffer
	COMMAND(CLEAR_LCD);                          // Clear
	
	BEGIN:COMMAND(GOTO_LINE1_POS0);              // Header with arrows
	DATA_char('<');
	COMMAND(GOTO_LINE1_POS0 + 15);
	DATA_char('>');
	
	timeout = timeout / 4300;                    // Shorter timeout for this screen
	
	while(timeout--){                            // UI loop
		COMMAND(GOTO_LINE2_POS0);                // Content line
		DATA_char(2);                            // Decorative/custom icon
		COMMAND(GOTO_LINE2_POS0 + 4);            // Position for HH
		if((hour_count >= 0) && (hour_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 4);        // Leading zero if needed
			DATA_char('0');
		}
		U32LCD(hour_count);                       // Print hours
		DATA_char(':');                           // Separator
		if((min_count >= 0) && (min_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 7);        // Leading zero for minutes
			DATA_char('0');
		}
		U32LCD(min_count);                        // Print minutes
		DATA_char(':');                           // Separator
		if((sec_count >= 0) && (sec_count <= 9)){
			COMMAND(GOTO_LINE2_POS0 + 10);       // Leading zero for seconds
			DATA_char('0');
		}
		U32LCD(sec_count);                        // Print seconds
		COMMAND(GOTO_LINE2_POS0 + 3);            // Cosmetic spacing
		LCD_SetCursor(15,1);                      // End marker
		DATA_char(3);                             // Decorative/custom icon
		
		switch(index){                            // Indicate selected field
			case 1: COMMAND(GOTO_LINE1_POS0 + 4);
							DATA("HH      ");
							break;
			case 2: COMMAND(GOTO_LINE1_POS0 + 4);
							DATA("   MM   ");
							break;
			case 3: COMMAND(GOTO_LINE1_POS0 + 4);
							DATA("      SS");
							break;
		}
	
		keypress = get_key();                     // Read keypad
		if(keypress != '\0'){
			timeout = TIMEOUT;                    // Reset inactivity timer
		switch(keypress){
			case 'A':if(index == 3){              // Next field (wrap)
									index = 1;
								}
							 else
									index++;
							 break;
			case 'B':if(index == 1){              // Previous field (wrap)
									index = 3;
								}
								else
									index--;
								break;
			case 'C':if(index == 1){              // Increment selected field
									if(hour_count == 23){
										hour_count = 0;
									}
									else
										hour_count++;
								}
								else if(index == 2){
										if(min_count == 59){
											min_count = 0;
										}
										else
											min_count++;
								}
								else if(index == 3){
										if(sec_count == 59){
											sec_count = 0;
										}
										else
											sec_count++;
								}
								break;
			case 'D':if(index == 1){              // Decrement selected field
									if(hour_count == 0){
										hour_count = 23;
									}
									else
										hour_count--;
								}
								else if(index == 2){
										if(min_count == 0){
											min_count = 59;
										}
										else
											min_count--;
								}
								else if(index == 3){
										if(sec_count == 0){
											sec_count = 59;
										}
										else
											sec_count--;
								}
								break;
								
			case '*':COMMAND(CLEAR_LCD);           // Emergency exit
								DATA(" EMERGENCY EXIT");
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								return 1;           // Abort
								
			case '=': if((device_on_hour <= hour_count) && (device_on_min < min_count)){
									device_off_hour = hour_count;
									device_off_min = min_count;
									device_off_sec = sec_count;
									COMMAND(CLEAR_LCD);
									DATA("OFFTIME UPDATED!");
									delay_ms(500);
									COMMAND(CLEAR_LCD);
									return 0;
                }
                else{
                  COMMAND(CLEAR_LCD);
                  DATA("INVALID OFFTIME");
                  delay_ms(500);
                  COMMAND(CLEAR_LCD);
                  goto BEGIN;
								 }           // Normal return
								
			default: COMMAND(CLEAR_LCD);           // Help for invalid key
								COMMAND(GOTO_LINE1_POS0);
								DATA("B/A TO NAVIGATE");
								COMMAND(GOTO_LINE2_POS0);
								DATA("C/D TO INCREMENT");
								delay_ms(500);
								COMMAND(CLEAR_LCD);
								goto BEGIN;         // Redraw
			}
		}
	}
	//Timeout delay to exit, if no operation choose for long time
	COMMAND(CLEAR_LCD);                           // Timeout message
	DATA("    TIME OUT!   ");
	delay_ms(250);
	COMMAND(CLEAR_LCD);
	timeout = TIMEOUT;                            // Reset timeout
	return 1;                                     // Indicate timeout/abort
}

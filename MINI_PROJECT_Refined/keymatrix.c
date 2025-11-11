#include "keymatrix.h"              // Matrix keypad pin/size macros and function prototypes
#include "delay.h"                  // Delay utilities: delay_ms(), delay_us()

#include "keymatrix.h"              // (Intentional repeat from your code) Same header included again
#include "delay.h"                  // (Intentional repeat) Delay header included again

c8 get_key(){                       // Returns a char for the pressed key, or '\0' if none
  i32 row,col;                      // Iteration indices for scanning rows and columns
	
	//For testing in Proteus Simulation
	uc8 key[4][4] = {                // Key mapping for Proteus: rows x cols -> character
    {'7', '8', '9', 'A'},           // Row 0: 7 8 9 A
    {'4', '5', '6', 'B'},           // Row 1: 4 5 6 B
    {'1', '2', '3', 'C'},           // Row 2: 1 2 3 C
    {'*', '0', '=', 'D'}            // Row 3: * 0 = D
	};
	
	//For testing on KIT
	/*uc8 key[4][4] = {              // Alternative mapping for the real kit (commented)
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '=', 'D'}
	};*/
        
	for(row = 0; row < ROWS; row++){          // Scan each row one by one
    WRITENIBBLE(IOSET1,20,0xF);               // Drive all row lines high (set R1..R4 = 1)
    CLRBIT(IOCLR1,(R1 + row));                 // Pull the current row low (activate this row)
    for(col = 0; col < COLS; col++){          // Scan all columns for this active row
       if(!(IOPIN1 & (1 << (C1 + col)))){     // Check if column line is pulled low (key press detected)
         delay_us(50);                        // Debounce: short delay to filter bouncing
         if(!(IOPIN1 & (1 << (C1 + col)))){   // Confirm key is still pressed after debounce
            while(!(IOPIN1 & (1 << (C1 + col)))); // Wait here until key is released (blocking)
              WRITENIBBLE(IOSET1,20,0xF);     // Restore rows to high before exiting
              return key[row][col];           // Return the mapped character for (row, col)
         }
			 }
		 }
	 }
   return '\0';                                // No key press detected after full scan
}

/*c8 get_key(){                                // --- Alternate version supporting long-press auto-repeat ---
	i32 row,col;                               // Row/column indices
	c8 pressedkey;                             // Stores the first detected key
	ui32 longpress;                            // Counter for press duration
	ui32 flag = 0;                             // Flag to indicate long-press repeat mode
	
	//For testing in Proteus Simulation
	uc8 key[4][4] = {                          // Same Proteus key map
    {'7', '8', '9', 'A'},
    {'4', '5', '6', 'B'},
    {'1', '2', '3', 'C'},
    {'*', '0', '=', 'D'}
	};*/
	
	//For testing on KIT
	/*uc8 key[4][4] = {                        // Alternate mapping for hardware kit
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '=', 'D'}
	};*/
	
	/*for(row = 0; row < ROWS; row++){         // Scan all rows
		WRITENIBBLE(IOSET1,20,0xF);            // Set all rows high
		CLRBIT(IOCLR1,(R1 + row));             // Pull selected row low
		for(col = 0; col < COLS; col++){       // Scan all columns for this row
			if(!(IOPIN1 & (1 << (C1 + col)))){ // If column goes low, a key is pressed
				delay_us(50);                   // Debounce delay
				if(!(IOPIN1 & (1 << (C1 + col)))){ // Confirm press after debounce
					pressedkey = key[row][col]; // Store which key it is
					longpress = 0;              // Reset press-duration counter
					while(!(IOPIN1 & (1 << (C1 + col)))){ // While key is still held
						longpress++;            // Count how long it's held
						if(longpress > 500000){ // Threshold for long-press
							flag = 1;           // Enter auto-repeat mode
							while(!(IOPIN1 & (1 << (C1 + col)))){ // While still held
								return key[row][col]; // Keep returning same key (repeat)
							}
						}
					}
					longpress = 0;              // Key released: clear counter
					if(!flag){                  // If not in long-press repeat mode
						return pressedkey;      // Return single press
					}
				}
			}
		}
	}
	return '\0';                                // No key pressed
}*/                                            // --- End alternate version ---

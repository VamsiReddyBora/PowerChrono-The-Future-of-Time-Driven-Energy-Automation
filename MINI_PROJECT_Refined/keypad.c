#include<LPC21xx.h>
#include "keypad.h"
#include "delay.h"

// Keypad lookup table for 4x4 keypad
char key_map[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

int get_keypad_input(void)
{
    int row, col;
    // Set all rows HIGH first
    IOCLR1 = (1<<20) | (1<<21) | (1<<22) | (1<<23);

    while (1)
    {
        for (row = 0; row < 4; row++)
        {
            // Set one row LOW at a time
            IOSET1 = (1 << (20 + row));
            for (col = 0; col < 4; col++)
            {
                // Check if column is LOW (pressed)
                if ((IOPIN1 & (1 << (24 + col))) == 0)
                {
                    // Debounce delay (simple)
                    //for (volatile int d = 0; d < 3000; d++);
                    delay_ms(5);
                    // Wait for key release
                    while ((IOPIN1 & (1 << (24 + col))) == 0);

                    // Reset row
                    IOCLR1 = (1 << (20 + row));
                    
                    return key_map[row][col]; // Return character
                }
            }
            // Reset row
            IOCLR1 = (1 << (20 + row));
        }
    }
}

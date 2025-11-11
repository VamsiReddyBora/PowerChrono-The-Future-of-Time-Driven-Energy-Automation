#include "types.h"

void COMMAND(uc8);
void DATA(c8 *);
void DATA_char(uc8);
void enable_lcd(void);
void U32LCD(ui32 n);
void InitializeLCD(void);
void LCD_Custom_Char(uc8 location, uc8 *msg);
void LCD_SetCursor(uc8, uc8);
void edit(void);
i32 edit_time(void);
i32 edit_on_off_time(void);
i32 myatoi(uc8 *str);
i32 edit_on_time(void);
i32 edit_off_time(void);
i32 time_(void);
i32 date_(void);
i32 day_(void);
void add_leading_zeros(int);
i32 count_digits(int);

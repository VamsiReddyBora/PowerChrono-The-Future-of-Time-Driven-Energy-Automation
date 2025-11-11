#include<LPC21xx.h>
#include "types.h"
#include "delay.h"

//Main Defines
#define SW (IOPIN0 & (1 << 16))

//Pin Connect Block
#define OUTPUT 1
#define INPUT 0
#define PIN_FUNC1 0
#define PIN_FUNC2 1
#define PIN_FUNC3 2
#define PIN_FUNC4 3

void CfgPortPinFunc(ui32 portNo,ui32 pinNo,ui32 pinFunc);
void ReservePins(ui32 port,ui32 pins,ui32 from,ui32 dir);

//LCD
#define CLEAR_LCD            0x01 
#define RET_CUR_HOME         0x02 
#define SHIFT_CUR_RIGHT      0x06 
#define SHIFT_CUR_LEFT       0x07 
#define DSP_OFF              0x08 
#define DSP_ON_CUR_OFF       0x0c 
#define DSP_ON_CUR_ON        0x0E 
#define DSP_ON_CUR_BLINK     0x0F 
#define SHIFT_DISP_LEFT      0x10 
#define SHIFT_DISP_RIGHT     0x14 
#define MODE_8BIT_1LINE      0x30 
#define MODE_4BIT_1LINE      0x20 
#define MODE_8BIT_2LINE      0x38 
#define MODE_4BIT_2LINE      0x28 
#define GOTO_LINE1_POS0      0x80 
#define GOTO_LINE2_POS0      0xC0 
#define GOTO_LINE3_POS0      0x94 
#define GOTO_LINE4_POS0      0xD4 
#define GOTO_CGRAM_START     0x40 

#define LCD_RS    9
#define LCD_RW   10
#define LCD_EN    8

#define cmd_mode IOCLR0 |= 1 << LCD_RS
#define data_mode IOSET0 |= 1 << LCD_RS
#define write_lcd IOSET0 &= ~(1 << LCD_RW) // No Need
#define read_lcd IOSET0 = 1 << LCD_RW // No Need
#define LCD_EN 8

void COMMAND(uc8);
void DATA(c8 *);
void DATA_char(uc8);
void enable_lcd(void);
void U32LCD(ui32 n);
void InitializeLCD(void);
void LCD_Custom_Char(uc8 location, uc8 *msg);
void LCD_SetCursor(uc8, uc8);
void edit(void);
void edit_time(void);
void edit_on_off_time(void);
int atoi(uc8 *str);
void edit_on_time(void);
void edit_off_time(void);

//Keypad
#define R1 20
#define R2 21
#define R3 22
#define R4 23
#define C1 24
#define C2 25
#define C3 26
#define C4 27

#define ROWS 4
#define COLS 4

c8 get_key(void);

//Defines
#define SETBIT(WORD,BIT) WORD |= 1<<BIT
#define CLRBIT(WORD,BIT) WORD |= 1<<BIT
#define READBIT(WORD,BIT)  ((WORD>>BIT)&1)
#define WRITEBIT(WORD,BITPOS,BITLEVEL) \
        (BITLEVEL?(WORD|=1<<BITPOS):(WORD&=~(1<<BITPOS)))
        //WORD=((WORD&~(1<<BITPOS))|(BIT<<BITPOS))     				
#define READWRITEBIT(DWORD,DBIT,SWORD,SBIT) \
        DWORD=((DWORD&~(1<<DBIT))|(((SWORD>>SBIT)&1)<<DBIT))

#define WRITE2PORTBIT(PORTNO,BITPOS,BITLEVEL) \
        ((PORTNO==0)? \
				(BITLEVEL?(IOSET0=1<<BITPOS):(IOCLR0=1<<BITPOS)): \
				(BITLEVEL?(IOSET1=1<<BITPOS):(IOCLR1=1<<BITPOS)))

#define WRITEBYTE(WORD,BITSTARTPOS,BYTE) \
        WORD=((WORD&~(0xFF<<BITSTARTPOS))|(BYTE<<BITSTARTPOS))
				
#define WRITENIBBLE(WORD,BITPOS,NIBBLE) \
				WORD |= NIBBLE << BITPOS
				
//RTC
#ifndef RTC_DEFINES_H
#define RTC_DEFINES_H

#define FOSC 12000000 // System clock and peripheral clock Macros
#define CCLK (5*FOSC)    
#define PCLK (CCLK/4) 
#define PREINT_VAL ((PCLK/32768)-1) // RTC Macros
#define PREFRAC_VAL (PCLK-(PREINT_VAL+1)*32768) 
#define RTC_ENABLE  (1<<0) //CCR register bits
#define RTC_RESET   (1<<1)
#define RTC_CLKSRC  (1<<4)
//#define _LPC2148

#endif

#ifndef RTC_H
#define RTC_H

void InitializeRTC(void);
void FetchTime(i16 *,i16 *,i16 *);
void PrintTime(ui32,ui32,ui32);
void FetchDate(i16 *,i16 *,i16 *);
void PrintDate(ui32,ui32,ui32);

void UpdateRTCTime(ui32,ui32,ui32);
void UpdateRTCDate(ui32,ui32,ui32);

void FetchDay(i16 *);
void PrintDay(ui32);
void UpdateRTCDay(ui32);

#endif

//External Interrupt
#define EINT0_VIC_CHNO 14
#define EINT0_STATUS_LED  16 //@p1.16

void eint0_isr(void) __irq;
void Eint_Init(void);

//LED
void Device_ON(void);
void Device_OFF(void);
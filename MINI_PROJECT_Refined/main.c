#include "main.h"

i16 hour,min,sec,date,month,year,day;
ui32 current_time;
ui32 longpress;// For Keypad Short + Long press action

//Global variables for Device ON/OFF time settings
ui32 device_on_hour; 
ui32 device_off_hour;
ui32 device_on_min;
ui32 device_off_min;
ui32 device_on_sec;
ui32 device_off_sec;
ui32 device_on_time;
ui32 device_off_time;

//CGRAM Byte values for Custom Icons
uc8 bulb_icon_on[8] = {0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x0E, 0x0A, 0x0A};
uc8 bulb_icon_off[8] = {0x0E, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x0A, 0x0A};
uc8 up_arrow[8] = {0x04, 0x0E, 0x1F, 0x15, 0x04, 0x04, 0x04, 0x04};
uc8 down_arrow[8] = {0x04, 0x04, 0x04, 0x15, 0x1F, 0x0E, 0x04};
uc8 bell_icon_on[8] = { 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00};
uc8 bell_icon_off[8] = { 0x04, 0x0A, 0x0A, 0x0A, 0x1F, 0x00, 0x04, 0x00};


int main(){
  //Allocating Pins for LCD(p0.9 - p0.0), KEYPAD(p1.20 - p1.27), LED(p1.18), SWITCH(p0.14)
	ReservePins(0,10,0,OUTPUT); //(port, how many pins, from which pin, I/O)
	ReservePins(1,4,20,OUTPUT); // For 4x4 Keypad - ROWS
	ReservePins(1,4,24,INPUT); // For 4x4 Keypad - COLS
	ReservePins(1,1,18,OUTPUT); // For LED
	ReservePins(0,1,14,INPUT); // For Switch p0.14
	
	InitializeRTC();
	InitializeLCD();
	LCD_Custom_Char(0, bulb_icon_on); // Store the bulb_on_icon at CGRAM position 0
	LCD_Custom_Char(1, bulb_icon_off); // Store the bulb_off_icon at CGRAM position 1
	LCD_Custom_Char(2, up_arrow); // Store the uparrow_icon at CGRAM position 2
	LCD_Custom_Char(3, down_arrow); // Store the down arrow at CGRAM position 3
	LCD_Custom_Char(4, bell_icon_on); // Store the bell_icon_on at CGRAM position 4
	LCD_Custom_Char(5, bell_icon_off); // Store the bell_icon_off at CGRAM position 5
	
	Eint_Init(); // External Interrupt 0 at p0.16
	
	while(1){
		//Displaying Time and date
		FetchTime(&hour,&min,&sec); //Fetches Time from RTC Block
		PrintTime(hour, min, sec); //Displaying Time info onto LCD Screen
		FetchDate(&date,&month,&year); //Fetching Date Info from RTC Block
		PrintDate(date,month,year); //Displaying Date Info onto LCD Screen
		FetchDay(&day); //Fetching Date
		PrintDay(day); //Displaying Date
		
		//Logic for Device ON/OFF based on device_on_time and off_time
		current_time = (hour * 60) + min;
		device_on_time = (device_on_hour * 60) + device_on_min;
		device_off_time = (device_off_hour * 60) + device_off_min;
		
		if((current_time >= device_on_time) && (current_time < device_off_time)){
			Device_ON();
		}
		else{
			Device_OFF();
		}
		
		//Switch configured to see ON/OFF Times on LCD Screen (p0.14)
		if(!SW){
			PrintONTime(device_on_hour, device_on_min, device_on_sec);
			PrintOFFTime(device_off_hour, device_off_min, device_off_sec);
			delay_s(1);
			COMMAND(CLEAR_LCD);
		}
		
		//Alarm Icon if ON and OFF Timings given 
		if((device_on_time != 0) && (device_off_time != 0)){
			LCD_SetCursor(9,0);
			DATA_char(4);
		}
		else{
			LCD_SetCursor(9,0);
			DATA_char(' ');
		}
	}
}

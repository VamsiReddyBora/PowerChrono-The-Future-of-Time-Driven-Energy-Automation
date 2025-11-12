# PowerChrono - The Future of Time-Driven Energy Automation 🚀
### PowerChrono is an intelligent, time-based power management system designed to optimize energy usage in industrial environments. This embedded project automates electrical devices according to user-defined time schedules, reducing energy wastage and improving operational reliability.

## Features ✨
1. Displays real-time clock (RTC) date and time on a 16x2 LCD.

2. Allows users to modify RTC settings easily using a 4x4 matrix keypad.

3. Supports setting device ON and OFF times for automated control.

4. Controls device operation based on programmed schedules.

5. Multiple menu navigation styles for intuitive and flexible user interaction.

6. Real-time display and menu updates with input validation for error-free operation.

## Hardware Components ⚙️
1. LPC2148 Microcontroller (ARM7-based)

2. 16x2 LCD Display (HD44780)

3. 4x4 Matrix Keypad

4. External Device (e.g., LED) for operation control

5. Switch for user interrupt input

6. USB-UART Converter and DB-9 Cable for serial communication

## Pin Configuration 📌

Signal	Port Pin	Description                                                                                                                                                 
| Signal            | Port Pin          | Description               |
| :---------------- | :---------------- | :------------------------ |
| LCD RS            | P0.9              |                          |
| LCD E             | P0.8              |                          |
| LCD Data Pins     | P0.7 - P0.7       |                          |
| Keypad Rows       | P1.20 - P1.23     |                          |
| Keypad Columns    | P1.24 - P1.27     |                          |
| LED Device        | P1.18             |                          |
| RTC Interrupt Pin | P0.16             |                          |
| Switch Input      | P0.14             |                          |                                                                                                                                          

## Software 🧩
Written in Embedded C for LPC2148 microcontroller.

Uses Flash Magic for flashing the code.

Implements timer interrupts and GPIO interrupts for real-time system control.

Matrix keypad scanning and LCD interfacing libraries.

Modular menu-driven design for RTC and device timing configuration.

User Guide 📋
Power on the system.

The current date and time display on the LCD.

Press the configured switch to enter the menu.

Use the keypad to navigate and edit RTC info or device ON/OFF schedules.

Confirm inputs; invalid entries prompt re-entry.

Exit menu to return to normal operation with updated settings.

Screenshots & Images 📸
Add your images here

![Project Hardware](

Caption describing hardware setup

![LCD Menu Interface](

Caption describing menu system

Future Enhancements 💡
Add support for multiple device control schedules.

Integrate with wireless communication for remote management.

Implement power consumption analytics and logging.

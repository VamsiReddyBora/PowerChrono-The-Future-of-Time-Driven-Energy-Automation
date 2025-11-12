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

| Signal            | Port Pin          | Description                                                      |
| :---------------- | :---------------- | :---------------------------------------------------------------|
| LCD RS            | P0.9              | Register Select pin: selects command (0) or data (1) register   |
| LCD E             | P0.8              | Enable pin: latches data into LCD on high-to-low transition     |
| LCD Data Pins     | P0.7 - P0.7       | Data bus lines for sending commands or data to the LCD          |
| Keypad Rows       | P1.20 - P1.23     | Rows of the 4x4 matrix keypad, scanned for key detection        |
| Keypad Columns    | P1.24 - P1.27     | Columns of the 4x4 matrix keypad, scanned for key detection     |
| LED Device        | P1.18             | Output pin controlling device (e.g., LED) state ON/OFF          |
| RTC Interrupt Pin | P0.16             | Input pin for Real-Time Clock interrupt signaling                |
| Switch Input      | P0.14             | Input pin connected to a user switch to see ON/OFF Times  |
                                                                                                                                          

## Software 🧩
1. Written in Embedded C for LPC2148 microcontroller.

2. Uses Flash Magic for flashing the code.

3. Implements timer interrupts and GPIO interrupts for real-time system control.

4. Matrix keypad scanning and LCD interfacing libraries.


## User Guide 📋
1. Power on the system.

2. The current date and time display on the LCD.

3. Press the configured switch to enter the menu.

4. Use the keypad to navigate and edit RTC info or device ON/OFF schedules.

5. Confirm inputs; invalid entries prompt re-entry.

6. Exit menu to return to normal operation with updated settings.

## Screenshots & Images 📸
### Overall Proteus Setup
<img width="1007" height="763" alt="image" src="https://github.com/user-attachments/assets/26b7024e-e38f-4bd3-9e25-9f68508bf761" />

### Home Screen
<img width="394" height="161" alt="image" src="https://github.com/user-attachments/assets/7713f46b-0dae-46aa-8f1b-72ae0fe9c7e5" />

### Editing Menu Style
>            >            -      To move Right (between HH:MM:SS)
>            <            -      To move Left (between HH:MM:SS)
>            up arrow     -      To Increment the value
>            down arrow   -      To Decrement the value
<img width="395" height="160" alt="image" src="https://github.com/user-attachments/assets/e9fc3926-c8a0-4143-af4f-984a1bea5b40" />

## Known Issues ⚠️
1. When setting ON and OFF Times                                                                                                                                              
   00:00 act as Starting of the Day for ON TIME, but                                                                                                                          
   00:00 act as Ending of the Day for OFF TIME.                                                                                                                               
2. The Year should be 2025. If you're trying to modify the year, you will be allowed to edit the year from (1 to 4095) but a WARNING message appears.

## Future Enhancements 💡
1. Add support for multiple device control schedules.

2. Integrate with wireless communication for remote management.

3. Implement power consumption analytics and logging.

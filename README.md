Project Title:
PowerChrono: The Future of Time-Driven Energy Automation 

Microcontroller Used:
LPC2129/LPC2148 (ARM7)

------------------------------------------------------------
Project Overview:
This project implements a Real-Time Clock (RTC) based timing control system with a unique navigation style for setting RTC time and Device ON/OFF times using a 4x4 matrix keypad and displaying information on a 16x2 LCD.

The user can:
- View and edit the RTC (Hours, Minutes, Seconds, Date, Month, Year).
- Set Device ON and OFF times.
- Navigate and modify values directly using arrow-based keypad inputs.

------------------------------------------------------------
Unique Navigation Style:
Instead of numeric menu selection, arrow keys (A/B/C/D) from the keypad are used for intuitive control:

A / B → Navigate between fields (LEFT/RIGHT or UP/DOWN)
C / D → Increment or decrement the selected value
Changes reflect immediately on the LCD display (HH, MM, SS, etc.)
LCD clearly indicates active fields using arrows (↑, ↓, ←, →)

Example LCD display during edit:
HH
00:00:01
↑         ↓

or

B/A TO NAVIGATE
C/D TO INCREMENT

------------------------------------------------------------
Connected Components:
- 16x2 LCD Display (LM016L) for showing RTC and menu.
- 4x4 Matrix Keypad for user input and navigation.
- LED (Device) to represent ON/OFF operation.
- Pull-up resistors used for stable input signals.

------------------------------------------------------------

Pin Connections (LPC2138):

Peripheral       | Pins Used             | Description
------------------------------------------------------------
LCD (16x2)       | P0.0 – P0.7           | 8-bit LCD interface
Keypad Rows      | P1.20 – P1.23         | Output to keypad rows
Keypad Columns   | P1.24 – P1.27         | Input from keypad columns
LED Device       | P1.18                 | Output LED indicating ON/OFF state
Interrupt Switch | P0.16 (EINT1)         | Used to trigger menu for editing time
Power            | VDD = 3.3V            | Supplied to LPC2138 and LCD
RTC VBAT         | 3.3V                  | RTC backup power
ON/OFF Switch    | P0.14                 | To see the ON/OFF Times
------------------------------------------------------------
Working Flow:
1. The system continuously reads RTC values and displays them on the LCD.
2. When the switch (connected to P0.16) is pressed, the edit menu appears.
3. Using the keypad navigation, user can:
   - Edit RTC information.
   - Set Device ON/OFF times.
4. The LED automatically turns ON at the programmed ON time and OFF at the OFF time.

------------------------------------------------------------
Software Used:
- Embedded C (Keil uVision)
- Flash Magic (Programming Tool)
- Proteus 8 (Simulation)

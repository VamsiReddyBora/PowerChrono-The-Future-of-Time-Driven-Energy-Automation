# ⚡ **PowerChrono – Smart Time-Driven Energy Automation System**

A next-generation, RTC-powered embedded system designed to **automate electrical devices based on precise time schedules**, ensuring **energy efficiency**, **industrial reliability**, and **intuitive user interaction**.

---

## ✨ Features
- ⏱️ **Real-Time Clock Display**  
  Shows accurate date and time on a 16×2 LCD using the on-chip RTC.

- 🛠️ **Easy RTC Configuration**  
  A 4×4 keypad allows quick editing of hours, minutes, seconds, date, month, and year.

- 🔌 **Automated Device ON/OFF Scheduling**  
  Users can set ON and OFF times for any connected device, enabling fully automated operation.

- 🧭 **Multiple Menu Navigation Styles**  
  Supports intuitive movement between fields, validation prompts, and live display updates.

- ⚡ **Real-Time Control Execution**  
  Devices toggle automatically when the RTC matches the user-programmed timestamps.

- ❗ **Error-Free Operation**  
  Input validation ensures time formats and values are always correct.

---

## 🛠️ Hardware Components
- **LPC2148 (ARM7) Microcontroller**  
- **16×2 HD44780-Based LCD Module**  
- **4×4 Matrix Keypad**  
- **External Device (LED / Load) for Control Demonstration**  
- **User Switch for Interrupt-Based Navigation**  
- **USB-UART Converter + DB-9 Cable (Programming/Debugging)**  

---

## 📌 Pin Configuration

| **Signal**          | **Pin**        | **Description**                                   |
|---------------------|----------------|---------------------------------------------------|
| LCD RS              | P0.9           | Selects LCD command/data mode                     |
| LCD EN              | P0.8           | Latches LCD data on falling edge                  |
| LCD Data            | P0.0 – P0.7    | 8-bit LCD parallel data bus                       |
| Keypad Rows         | P1.20 – P1.23  | Row scanning inputs                               |
| Keypad Columns      | P1.24 – P1.27  | Column scanning inputs                            |
| Device Output (LED) | P1.18          | Used to toggle ON/OFF device                      |
| Interrupt           | P0.16          | External Interrupt 0 (EINT0)                      |
| User Switch         | P0.14          | View ON/OFF schedule                              |

---

## 🧩 Software Overview
- Entire firmware written in **Embedded C** for ARM7 (LPC2148).  
- Programmed using **Flash Magic** via UART.  
- Implements:
  - Interrupts  
  - External GPIO interrupts  
  - Keypad scanning  
  - LCD command/data routines  
  - RTC read/write functions  
  - Scheduling logic  

---

## 🧭 User Guide

1. Power on the system — the LCD shows **Date & Time**.  
2. Press the configured **menu switch(Interrupt)** to enter settings.  
3. Use the keypad to edit:  
   - RTC Time (HH:MM:SS)  
   - RTC Date (DD/MM/YYYY)  
   - Device ON Time  
   - Device OFF Time  
4. Navigation controls:  
   - **“>”** → Move Right  
   - **“<”** → Move Left  
   - **↑** → Increment  
   - **↓** → Decrement  
5. Confirm inputs — invalid entries trigger warnings.  
6. Exit to return to normal running mode with updated parameters.

---

## 📸 Screenshots

### 🖥️ Full Proteus Setup  
<img width="1298" height="915" alt="image" src="https://github.com/user-attachments/assets/58901025-2568-4f2d-a4bd-8207e581b6be" />

### ⌚ Main Menu  
<img width="1644" height="664" alt="image" src="https://github.com/user-attachments/assets/42e1fd7b-1843-4ed3-8aab-427b7d384dba" />

### ✏️ Editing Menu (Time Configuration)
> **Right ( > )** → Move between HH • MM • SS  
>  
> **Left ( < )** → Move backward  
>  
> **↑** → Increase value  
>  
> **↓** → Decrease value  

<img width="1613" height="651" alt="image" src="https://github.com/user-attachments/assets/3d7bb0a1-1cdf-4bf0-aeed-37cc0db67e3c" />

---

## ⚠️ Known Issues
- Setting ON/OFF Time at **00:00** behaves differently:  
  - As **Day Start** for ON Time  
  - As **Day End** for OFF Time  

- Year field supports **1–4095**, but only **2025** is accepted for correct operation  
  (Warning message alerts the user during editing).

---

## 💡 Future Enhancements
- Multi-device scheduling  
- Wireless control & monitoring  
- Data logging & energy consumption analytics  
- Backup memory for schedule storage  
- OLED/Graphical display for advanced UI  

---

## 👤 Author
**Vamsi Reddy Bora**  
Electronics Engineer  
Developer of *PowerChrono – Time-Based Power Automation System*

---

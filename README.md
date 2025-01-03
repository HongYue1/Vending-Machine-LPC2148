# LPC2148 Vending Machine Controller

This repository contains the source code for a vending machine controller implemented using an LPC2148 microcontroller. The system handles money insertion, product selection via buttons, and dispensing of selected products using a stepper motor. It also uses an LCD to display messages and prompts.

## Features

-   **Money Insertion Detection:** Uses an LDR connected to an ADC to detect when money is inserted.
-   **Product Selection:** Allows the user to select from four products (Coke, Coffee, Pepsi, and Sprite) using dedicated buttons.
-   **Product Dispensing:** Uses a stepper motor to eject the selected product.
-   **LCD Display:** Displays messages to guide the user, such as prompting for money insertion and displaying selected product information.

## Hardware Setup

The following hardware components are used:

-   **LPC2148 Microcontroller:** The central processing unit.
-   **16x2 LCD:** For displaying messages.
-   **Push Buttons:** Four buttons for product selection.
-   **Stepper Motor:** For dispensing products.
-   **Light Dependent Resistor (LDR):** Used to detect money insertion via ADC.
-   **Resistors and Capacitors:** For pull up/down, ADC, and clock circuitry

**Pin Assignments:**

*   **ADC:** ADC0.6 - P0.4 (for LDR based money detection)
*   **LCD:**
    *   Data pins: P0.12-P0.15
    *   Control pins:
        *   RS: P0.0
        *   RW: P0.1
        *   EN: P0.2
*   **Product Selection Buttons:** P1.17, P1.18, P1.19, P1.20 
*   **Stepper Motor Control:** 
	*	For product 1: P0.16, P0.17, P0.18, P0.19
	*   For product 2: P0.20, P0.21, P0.22, P0.23
	*	For product 3: P0.25, P0.26, P0.27, P0.28
	*   For product 4: P1.21, P1.22, P1.23, P1.24

## Software Overview

### Code Structure

The code is written in C and consists of the following key components:

-   **Header Inclusion:** Includes the necessary LPC214x.h for LPC2148 specific registers and features.
-   **Definitions:** Defines constants for pin assignments and product codes.
-   **Function Declarations:** Declares various functions like `delay_ms`, `LCD_CMD`, `LCD_INIT`, `LCD_CHAR`, `LCD_String`, `adc`, `eject_product`, and `money_inserted`.
-   **`main` Function:** Contains the main program loop that handles initialization, money detection, product selection, and dispensing.
-   **`adc` Function:** Reads the analog value from a specified ADC channel.
-   **`delay_ms` Function:** Implements a software delay for timing purposes.
-   **LCD Control Functions (`LCD_CMD`, `LCD_CHAR`, `LCD_INIT`, `LCD_String`):** Functions to initialize and control the LCD for displaying messages.
-   **`eject_product` Function:** Controls the stepper motor sequence for dispensing a selected product.
-   **`money_inserted` Function:** Uses the ADC to detect if money is inserted.

### How it Works

1.  **Initialization:** The code initializes the ADC, LCD, and pin configurations in the `main` function. The LCD displays "Enter Money" to start.
2.  **Money Detection:** The `money_inserted` function continuously monitors the ADC value. When the ADC reading crosses a threshold indicating money insertion, it sets a flag (`money_flag`) and displays "Choose Product:".
3.  **Product Selection:** The `main` function checks for button presses. If a button corresponding to a product is pressed, the LCD displays the selected product.
4.  **Product Dispensing:** The `eject_product` function is called with a product number to actuate the stepper motor, dispensing the selected product.
5.  **Looping:** After dispensing, the system resets, displays "Enter Money" on the LCD, and waits for further input. If no money has been inserted, the system loops with a longer delay.

## How to Compile and Run

1.  **Install Keil uVision v4 IDE:** This project was developed using Keil uVision IDE for ARM microcontrollers.
2.  **Create a New Project:** Create a new project in Keil uVision for the LPC2148 microcontroller.
3.  **Add Source File:** Add the provided `.c` file to your project.
4.  **Configure Project Settings:** Configure the project settings for the LPC2148 microcontroller.
5.  **Compile the Code:** Compile the code using the Keil uVision IDE.
6.  **Flash the Code:** Connect the LPC2148 board using a JTAG programmer, and flash the generated hex file to the microcontroller.
7.  **Test:** Wire the peripheral components as described and power on the setup. The vending machine controller should be operational.

## Notes

-   The delay values used in the code may need to be adjusted based on your specific hardware.
-   The ADC reading threshold for money detection may need to be adjusted according to the LDR used.
-   The stepper motor sequence in the `eject_product` function may require fine-tuning for precise product dispensing.

## License

This project is open-source and is available under the MIT License. Feel free to use and modify the code as needed.

## Contact

If you have any questions or feedback, feel free to reach out.

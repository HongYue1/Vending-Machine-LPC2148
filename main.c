#include <lpc214x.h>

// LCD control pin definitions
#define RS 0
#define RW 1
#define EN 2
#define data_port 0xFFFF0FFF // Data pins mask

// ADC channel definition
#define ADC06 4

// Product codes
#define COKE 17
#define COFFEE 18
#define PEPSI 19
#define SPRITE 20

// Function declarations
void delay_ms(long ms);
void LCD_CMD(char cmd);
void LCD_INIT(void);
void LCD_CHAR(char data);
void LCD_String(char *str);
unsigned int adc(int no, int ch);
void eject_product(int p_no);
void money_inserted(void);

unsigned int val;
int money_flag = 0;

int main()
{
    // Configure ADC0.6 pin as input
    IO0DIR &= ~(1 << ADC06);

    // Set upper 16 bits of IO0 as outputs
    IO0DIR |= 0xFFFF0000;

    // Set IO1DIR for product selection pins and stepper motor control
    IO1DIR = 0x00010000;                                     // Set P1.16 as output
    IO1DIR |= (1 << 21) | (1 << 22) | (1 << 23) | (1 << 24); // Set P1.21 to P1.24 as outputs

    // Pin select for ADC0.6
    PINSEL0 = 0x0300; // P0.4 -- ADC0.6
    VPBDIV = 0x02;    // Set VPB clock divider to 0.5

    // Initialize LCD and display initial message
    LCD_INIT();
    LCD_String("Enter Money");

    while (1)
    {
        money_inserted(); // Check for money insertion

        if (money_flag)
        {
            // Check which product button is pressed
            if ((IO1PIN & (1 << COKE)) == 0)
            {
                LCD_CMD(0x01); // Clear LCD
                LCD_String("Coke Choosen");
                delay_ms(500);
                eject_product(1); // Eject Coke
                money_flag = 0;   // Reset money flag
                delay_ms(1000);
                LCD_CMD(0x01); // Clear LCD
                LCD_String("Enter Money");
            }
            else if ((IO1PIN & (1 << COFFEE)) == 0)
            {
                LCD_CMD(0x01); // Clear LCD
                LCD_String("Coffee Choosen");
                delay_ms(500);
                eject_product(2); // Eject Coffee
                money_flag = 0;   // Reset money flag
                delay_ms(1000);
                LCD_CMD(0x01); // Clear LCD
                LCD_String("Enter Money");
            }
            else if ((IO1PIN & (1 << PEPSI)) == 0)
            {
                LCD_CMD(0x01); // Clear LCD
                LCD_String("Pepsi Choosen");
                delay_ms(500);
                eject_product(3); // Eject Pepsi
                money_flag = 0;   // Reset money flag
                delay_ms(1000);
                LCD_CMD(0x01); // Clear LCD
                LCD_String("Enter Money");
            }
            else if ((IO1PIN & (1 << SPRITE)) == 0)
            {
                LCD_CMD(0x01); // Clear LCD
                LCD_String("Sprite Choosen");
                delay_ms(500);
                eject_product(4); // Eject Sprite
                money_flag = 0;   // Reset money flag
                delay_ms(1000);
                LCD_CMD(0x01); // Clear LCD
                LCD_String("Enter Money");
            }
            delay_ms(50); // Short delay
        }
        else
        {
            delay_ms(500); // Longer delay when no money is inserted
        }
    }
}

// ADC conversion function
unsigned int adc(int no, int ch)
{
    switch (no) // Select ADC module
    {
    case 0:
        AD0CR = 0x00200600 | (1 << ch); // Select channel and set control bits
        AD0CR |= (1 << 24);             // Start conversion
        while ((AD0GDR & (1U << 31)) == 0)
            ;         // Wait for conversion complete
        val = AD0GDR; // Read conversion result
        break;

    case 1:
        AD1CR = 0x00200600 | (1 << ch); // Select channel and set control bits
        AD1CR |= (1 << 24);             // Start conversion
        while ((AD1GDR & (1U << 31)) == 0)
            ;         // Wait for conversion complete
        val = AD1GDR; // Read conversion result
        break;
    }

    val = (val >> 6) & 0x03FF; // Extract 10-bit ADC value

    return val;
}

// Software delay function
void delay_ms(long ms)
{
    long i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 300; j++)
            ; // Delay loop
}

// Send command to LCD in 4-bit mode
void LCD_CMD(char cmd)
{
    IO0PIN = ((IO0PIN & data_port) | ((cmd & 0xF0) << 8)); // Upper nibble
    IO0SET = (1 << EN);                                    // Enable pulse
    IO0CLR = (1 << RS | (1 << RW));                        // RS and RW low
    delay_ms(5);
    IO0CLR = (1 << EN); // Enable low
    delay_ms(5);

    IO0PIN = ((IO0PIN & data_port) | ((cmd & 0x0F) << 12)); // Lower nibble
    IO0SET = (1 << EN);                                     // Enable pulse
    IO0CLR = (1 << RS | (1 << RW));                         // RS and RW low
    delay_ms(5);
    IO0CLR = (1 << EN); // Enable low
    delay_ms(5);
}

// Send data to LCD in 4-bit mode
void LCD_CHAR(char data)
{
    IO0PIN = ((IO0PIN & data_port) | ((data & 0xF0) << 8)); // Upper nibble
    IO0SET = (1 << RS) | (1 << EN);                         // RS high, Enable pulse
    IO0CLR = (1 << RW);                                     // RW low
    delay_ms(2);
    IO0CLR = (1 << EN); // Enable low
    delay_ms(5);

    IO0PIN = ((IO0PIN & data_port) | ((data & 0x0F) << 12)); // Lower nibble
    IO0SET = (1 << RS) | (1 << EN);                          // RS high, Enable pulse
    IO0CLR = (1 << RW);                                      // RW low
    delay_ms(2);
    IO0CLR = (1 << EN); // Enable low
    delay_ms(5);
}

// Initialize LCD
void LCD_INIT(void)
{
    IO0DIR |= 0x0000F007; // Set LCD data pins and control pins as outputs
    delay_ms(20);
    LCD_CMD(0x02); // Initialize LCD in 4-bit mode
    LCD_CMD(0x28); // 2 lines, 5x7 dots
    LCD_CMD(0x0C); // Display on, cursor off
    LCD_CMD(0x06); // Increment cursor
    LCD_CMD(0x01); // Clear display
    LCD_CMD(0x80); // Set cursor to first line, first position
}

// Print string to LCD
void LCD_String(char *str)
{
    while (*str != '\0')
    {
        LCD_CHAR(*str++);
    }
}

// Eject selected product using stepper motor
void eject_product(int p_no)
{
    switch (p_no)
    {
    case 1:
        // Stepper motor sequence for product 1
        IO0SET |= (1 << 16);
        IO0CLR |= (1 << 17) | (1 << 18) | (1 << 19);
        delay_ms(100);

        IO0SET |= (1 << 17);
        IO0CLR |= (1 << 16) | (1 << 18) | (1 << 19);
        delay_ms(100);

        IO0SET |= (1 << 18);
        IO0CLR |= (1 << 16) | (1 << 17) | (1 << 19);
        delay_ms(100);

        IO0SET |= (1 << 19);
        IO0CLR |= (1 << 16) | (1 << 17) | (1 << 18);
        delay_ms(100);
        break;

    case 2:
        // Stepper motor sequence for product 2
        IO0SET |= (1 << 20);
        IO0CLR |= (1 << 21) | (1 << 22) | (1 << 23);
        delay_ms(100);

        IO0SET |= (1 << 21);
        IO0CLR |= (1 << 20) | (1 << 22) | (1 << 23);
        delay_ms(100);

        IO0SET |= (1 << 22);
        IO0CLR |= (1 << 20) | (1 << 21) | (1 << 23);
        delay_ms(100);

        IO0SET |= (1 << 23);
        IO0CLR |= (1 << 20) | (1 << 21) | (1 << 22);
        delay_ms(100);
        break;

    case 3:
        // Stepper motor sequence for product 3
        IO0SET |= (1 << 25);
        IO0CLR |= (1 << 26) | (1 << 27) | (1 << 28);
        delay_ms(100);

        IO0SET |= (1 << 26);
        IO0CLR |= (1 << 25) | (1 << 27) | (1 << 28);
        delay_ms(100);

        IO0SET |= (1 << 27);
        IO0CLR |= (1 << 25) | (1 << 26) | (1 << 28);
        delay_ms(100);

        IO0SET |= (1 << 28);
        IO0CLR |= (1 << 25) | (1 << 26) | (1 << 27);
        delay_ms(100);
        break;

    case 4:
        // Stepper motor sequence for product 4
        IO1SET |= (1 << 21);
        IO1CLR |= (1 << 22) | (1 << 23) | (1 << 24);
        delay_ms(100);

        IO1SET |= (1 << 22);
        IO1CLR |= (1 << 21) | (1 << 23) | (1 << 24);
        delay_ms(100);

        IO1SET |= (1 << 23);
        IO1CLR |= (1 << 21) | (1 << 22) | (1 << 24);
        delay_ms(100);

        IO1SET |= (1 << 24);
        IO1CLR |= (1 << 21) | (1 << 22) | (1 << 23);
        delay_ms(100);
        break;

    default:
        break;
    }
}

// Check for money insertion using ADC (LDR)
void money_inserted(void)
{
    if (money_flag == 0) // If money isn't inserted yet
    {
        adc(0, 6); // Read ADC0 channel 6

        if (val > 500)
        {
            money_flag = 1;                // Money Inserted
            LCD_CMD(0x01);                 // Clear LCD
            LCD_String("Choose Product:"); // Prompt user to choose product
        }
    }
}

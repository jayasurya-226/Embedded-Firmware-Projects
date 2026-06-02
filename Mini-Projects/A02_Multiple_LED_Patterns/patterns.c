/*
 * File:   patterns.c
 * Author: B.jayasurya
 * Roll  : 167
 *
 * Created on 21 November, 2025, 8:33 AM
 */


#include <xc.h>
#include "digital_keypad.h"

static int delay = 10000;                // Software delay variable for all patterns
int i = 0;                               // Step counter used by pattern2

//===========================
//   PATTERN ? 1 (TRAIN BOGIE)
//===========================
void pattern1()
{
    static int step = 0;                 // Pattern step counter (0?16)

    if (delay-- == 0)                    // Execute only when delay expires
    {
        if (step < 8)                    // First 8 steps ? LEDs grow from LSB to MSB
        {
            PORTB |= (1 << step);        // Turn ON bits one-by-one
        }
        else if (step < 16)              // Next 8 steps ? LEDs shrink from MSB to LSB
        {
            PORTB >>= 1;                 // Shift bits right (turn OFF from MSB)
        }
        else                              // When 16 steps complete
        {
            step = 0;                    // Restart step count
            PORTB = 0x00;                // Clear LEDs
            delay = 30000;               // Reload delay
            return;                      // Exit early
        }

        step++;                          // Go to next step
        delay = 30000;                   // Reload delay
    }
}


//===========================
//   PATTERN ? 2 (TRAIN BOGIE SAME STYLE)
//===========================
void pattern2()
{
    if (delay-- == 0)                    // Execute only when delay expires
    {
        if (i < 8)                       // First 8 steps ? LEDs grow from LSB to MSB
        {
            PORTB |= (1 << i);           // Turn ON bits one-by-one
        }
        else if (i < 16)                 // Next 8 steps ? LEDs shrink from MSB to LSB
        {
            PORTB >>= 1;                 // Shift bits right
        }
        else                              // When sequence ends
        {
            i = 0;                        // Restart steps
            PORTB = 0x00;                 // Clear LEDs
            delay = 30000;                // Reload delay
            return;
        }

        i++;                             // Increment step counter
        delay = 30000;                   // Reload delay
    }
}


//===========================
//   PATTERN ? 3 (AA ? 55 Toggle)
//===========================
void pattern3()
{
    static unsigned char toggle = 0;     // Toggle flag

    if (delay-- == 0)                    // Execute only when delay expires
    {
        if (toggle)                      // If toggle = 1
            PORTB = 0xAA;                // Load 10101010
        else                             // If toggle = 0
            PORTB = 0x55;                // Load 01010101

        toggle = !toggle;                // Flip toggle
        delay = 30000;                   // Reload delay
    }
}


//===========================
//   PATTERN ? 4 (UPPER NIBBLE ? LOWER NIBBLE)
//===========================
void pattern4()
{
    static unsigned char toggle = 0;     // Toggle flag

    if (delay-- == 0)                    // Execute only when delay expires
    {
        if (toggle)                      // Toggle ON
            PORTB = 0xF0;                // Upper nibble ON
        else                             // Toggle OFF
            PORTB = 0x0F;                // Lower nibble ON

        toggle = !toggle;                // Flip toggle
        delay = 30000;                   // Reload delay
    }
}


//===========================
//   INITIAL CONFIGURATION
//===========================
static void init_config(void)
{
    ADCON1 = 0x0F;                       // Set all pins as digital
    TRISB = 0x00;                        // Set PORTB as output
    PORTB = 0x00;                        // Clear all LEDs
    init_digital_keypad();               // Initialize keypad
}


//===========================
//            MAIN
//===========================
void main(void)
{
    unsigned char key;                   // Stores key value
    int flag = 0;                        // Stores selected pattern number

    init_config();                       // Initialize peripherals

    while (1)                            // Infinite loop
    {
        key = read_digital_keypad(STATE_CHANGE);   // Read key only on state change

        //--------- RESET PATTERNS WHEN KEY PRESSED ---------
        if (key == SWITCH1)              // If key1 press detected
        {
            flag = 1;                    // Select pattern1
            i = 0;                       // Reset counter
            delay = 30000;               // Reset delay
            PORTB = 0x00;                // Reset LEDs
        }
        else if (key == SWITCH2)         // If key2 pressed
        {
            flag = 2;                    // Select pattern2
            i = 0;                       // Reset steps
            delay = 30000;               // Reset delay
            PORTB = 0x00;                // Reset LEDs
        }
        else if (key == SWITCH3)         // If key3 pressed
        {
            flag = 3;                    // Select pattern3
            delay = 30000;               // Reset delay
            PORTB = 0x00;                // Reset LEDs
        }
        else if (key == SWITCH4)         // If key4 pressed
        {
            flag = 4;                    // Select pattern4
            delay = 30000;               // Reset delay
            PORTB = 0x00;                // Reset LEDs
        }

        //--------- EXECUTE SELECTED PATTERN ---------
        switch (flag)
        {
            case 1:
                pattern1();              // Run pattern1
                break;

            case 2:
                pattern2();              // Run pattern2
                break;

            case 3:
                pattern3();              // Run pattern3
                break;

            case 4:
                pattern4();              // Run pattern4
                break;

            default:
                PORTB = 0x00;            // If no key pressed, keep LEDs OFF
                break;
        }
    }
}

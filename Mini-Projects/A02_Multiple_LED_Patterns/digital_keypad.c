#include <xc.h>                   // Include PIC microcontroller header file
#include "digital_keypad.h"       // Include digital keypad header file

void init_digital_keypad(void)
{
	TRISC = TRISC | INPUT_PINS;  // Set RC0?RC3 as input pins for switches
}

unsigned char read_digital_keypad(unsigned char detection_type)
{
	static unsigned char once = 1;    // Flag to ensure single detection for STATE_CHANGE mode

	if (detection_type == STATE_CHANGE)  // Check if detection mode is state change
	{
		if (((KEY_PORT & INPUT_PINS) != ALL_RELEASED) && once) // Detect key press only once
		{
			once = 0;  // Block repeated detection until key is released


			return (KEY_PORT & INPUT_PINS); // Return pressed key value
        }
		
		else if ((KEY_PORT & INPUT_PINS) == ALL_RELEASED) // Check if all keys are released
		{
			once = 1;  // Re-enable detection for next press
		}
	}
	else if (detection_type == LEVEL)  // Check if detection mode is level-based
	{
		return (KEY_PORT & INPUT_PINS);  // Return current key level directly
	}

	return 0xFF;       // Return 0xFF when no valid key press is detected
}
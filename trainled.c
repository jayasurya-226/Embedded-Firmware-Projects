/*
 * File:   trainled.c
 * Author: B,JAYASURYA
 * ROLL   : 25017B_167
 * Description:
 *  This program controls 8 LEDs to simulate a train bogie 
 *  coming out of a tunnel (EXIT) and going back inside a 
 *  tunnel (ENTRY). 
 * 
 *  The bogie moves left-to-right and right-to-left 
 *  alternatively. LEDs are turned ON one by one during EXIT 
 *  and turned OFF one by one during ENTRY. Direction changes 
 *  automatically after each full cycle.
 * 
 * Features:
 *  - No blocking delay (non-blocking software counter)
 *  - No loops for LED pattern generation
 *  - Direction reverse after each exit+entry
 *  - Analog inputs disabled, PORTB used as digital output
 * 
 *
 * Created on 19 November, 2025, 8:49 AM
 */


#include <xc.h>
void init_config() // configurations
{
    TRISB = 0x00;
    PORTB = 0x00;
}

unsigned int timer = 0;     // software timer
int step = 0;                // step number 0?7
int direction = 0;           // 0 = L?R, 1 = R?L
int mode = 0;                // 0 = exit, 1 = entry

void main(void) {
    
    init_config();
            
    while(1)
    {
        //NON - BLOCKING DELAY
        if(timer < 50000)       // delay approximately 1 sec
        {
            timer++;           // increment the counter 
            continue;          // skip
        }
          timer = 0;             // after delay complete update
          
          // Turn on LEDs one by one
          if(mode == 0)   // EXIT MODE
          {
              // LEFT TO RIGHT    
              if(direction == 0)
                PORTB = (unsigned char)(PORTB << 1) | 0x01;  // L-> R ON
              // RIGHT TO LEFT
              else
                  PORTB = (unsigned char)(PORTB >> 1) | 0x80; //R -> L ON
              
              step++;          // moving next step
              if(step == 8)   // if EXIT complete 
            {
                mode = 1;     // then ENTRY MODE
                step = 0;     // Reset step counter
            }

          }
          
           //LEDs OFF one-by-one
        else if(mode == 1)
        {
            // L ? R entry
            if(direction == 0)
                PORTB = (unsigned char)PORTB >> 1; // L -> R OFF

            // R ? L entry
            else
                PORTB = (unsigned char)PORTB << 1;  // R -> L OFF

            step++;  // move next step

            if(step == 8)   // entry complete
            {
                direction = !direction;  // reverse direction
                mode = 0;   // back to exit
                step = 0;   // reset step 

                PORTB = 0x00;  //clear LEDs
            }
        }
    }
          
    return;   
    }
  

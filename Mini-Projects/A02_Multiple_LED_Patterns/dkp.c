/*
 * File:   dkp.c
 * Author: jayasurya
 *
 * Created on 25 November, 2025, 8:53 PM
 */


#include <xc.h>
#include "dkp.h"

unsigned char read_dkp(void)
{
    if(PORTCbits.RC0 == 0) return 1;
    if(PORTCbits.RC1 == 0) return 2;
    if(PORTCbits.RC2 == 0) return 3;
    if(PORTCbits.RC3 == 0) return 4;
    return 0;
}

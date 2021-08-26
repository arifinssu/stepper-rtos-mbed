#ifndef __ARDUINOAPI_FTOA_H__
#define __ARDUINOAPI_FTOA_H__

// C program for implementation of ftoa()
#include <math.h>
#include <stdio.h>
  
void ftoa(float n, char* res, int afterpoint)
{
    int intpart, fracpart;
    intpart = (int)n;
    fracpart = (int)abs((n - intpart) * pow(10, afterpoint));
    sprintf(res, "%i.%i", intpart, fracpart);
}

#endif
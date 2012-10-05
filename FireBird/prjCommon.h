/* File: prjCommon.h
 * Description: Common definitions for the project
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

 #ifndef PRJ_COMMON_H
 #define PRJ_COMMON_H

 /* Common definitions */

 #define sbit(reg,bit)  reg |= (1<<bit)
 #define cbit(reg,bit)  reg &= ~(1<<bit)
 #define STATUS         int
 #define STATUS_OK      0
 typedef unsigned char  BYTE;

 /* Common headers */

#ifdef USE_AVR_STUDIO
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
#endif

 #endif /* PRJ_COMMON_H */
 

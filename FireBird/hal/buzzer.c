/** @file buzzer.c
 * Hardware abstraction layer for buzzer control
 */

/*
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */
 
 #include <prjParams.h>
 #include <prjCommon.h>
 #include "buzzer.h"

/** Initialize buzzer hardware */ 
 STATUS initBuzzer(void) {
	DDRC = DDRC | 0x08; 	/* Setting PORTC 3 as output */
	PORTC = PORTC & 0xF7; 	/* Setting PORTC 3 logic low to turnoff buzzer */
	return STATUS_OK;
 }
 
/** Turn on buzzer */
 void buzzerOn(void) {
	BYTE port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
 }
 
/** Turn off buzzer */
 void buzzerOff(void) {
	BYTE port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
 }


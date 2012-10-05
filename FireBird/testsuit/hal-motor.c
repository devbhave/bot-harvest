/* File: hal-motor.c
 * Description: Test cases for hardware abstraction layer for motor control
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

#ifndef USE_AVR_STUDIO 
#error "Use AVR Studio to build testsuit. Use compiler flag -DUSE_AVR_STUDIO."
#endif

 #include <prjParams.h>
 #include <prjCommon.h>
 #include <hal/motor.h>

 #define DELAY_COUNT  3000
 
 int main() {
	MotorDirection dirs[] = {FORWARD, BACKWARD, RIGHT, LEFT,
			SOFT_RIGHT, SOFT_LEFT, SOFT_RIGHT2, SOFT_LEFT2, STOP};

	int idx;
	
	/* Test #1: Check direction */
	
	motorVelocitySet(255, 255);
	for(idx = 0; idx < sizeof(dirs)/sizeof(dirs[0]); idx ++) {
		motorDirectionSet(dirs[idx]);
		
		/* Delay for some time */
		
		_delay_ms(DELAY_COUNT);
	}
	
	/* Test #2: Check speed */
	
	motorDirectionSet(FORWARD);
	motorVelocitySet(100, 100);
	
	/* Delay for some time */

	_delay_ms(DELAY_COUNT);
		
	/* Test #3: Check individual wheel speed */
	motorVelocitySet(100, 255);	/* Left turn expected */
	
	/* Delay for some time */
	
	_delay_ms(DELAY_COUNT);	
	
	motorVelocitySet(100, 100);	/* Right turn expected */
	
	/* Delay for some time */
	
	_delay_ms(DELAY_COUNT);
		
	motorDirectionSet(STOP);
	
	return 0;
 }

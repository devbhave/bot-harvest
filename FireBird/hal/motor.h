/* File: motor.h
 * Description: Motor control headers
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

 #ifndef MOTOR_H
 #define MOTOR_H
typedef enum TagMotorDirection {
  FORWARD     = 0x06,
  BACKWARD    = 0x09,
  RIGHT       = 0x05,
  LEFT        = 0x0A,
  SOFT_RIGHT  = 0x04,
  SOFT_LEFT   = 0x02,
  SOFT_RIGHT2 = 0x01,
  SOFT_LEFT2  = 0x08,
  STOP        = 0x00
} MotorDirection;

extern STATUS initMotor(void);
extern STATUS motorDirectionSet(MotorDirection direction);
extern STATUS motorVelocitySet(BYTE leftMotor, BYTE rightMotor);
 #endif /* MOTOR_H */
 

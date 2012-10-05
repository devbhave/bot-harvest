/* File: motor.h
 * Description: Motor control headers
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

 #ifndef MOTOR_H
 #define MOTOR_H
typedef enum TagMotionDirection {
  FORWARD     = 0x06,
  BACKWARD    = 0x09,
  RIGHT       = 0x05,
  LEFT        = 0x0A,
  SOFT_RIGHT  = 0x04,
  SOFT_LEFT   = 0x02,
  STOP        = 0x00
} MotionDirection;

 #endif /* MOTOR_H */
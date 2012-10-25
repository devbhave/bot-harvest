/* File: zigbee.h
 * Description: Zigbee control headers
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

 #ifndef ZIGBEE_H
 #define ZIGBEE_H
extern STATUS initZigbee(void);
extern STATUS zigbeeSend(char *data, UINT size);
 #endif /* ZIGBEE_H */
 

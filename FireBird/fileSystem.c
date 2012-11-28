/** @file fileSystem.c
 * File system module.
 */

/*
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

 #include <stdio.h>	
 #include <prjParams.h>
 #include <prjCommon.h> 
 #include <hal/zigbee.h>
 
 int mapReceiveByte(FILE *stream);

 FILE zigbeeStream = FDEV_SETUP_STREAM(zigbeeSendByte, 
    zigbeeReceiveByte, _FDEV_SETUP_RW);  /**< Device file descriptor for ZigBee.
                                          */
 FILE mapStream = FDEV_SETUP_STREAM(0, mapReceiveByte, _FDEV_SETUP_READ); /**< 
    Device file descriptor for map file. */
 
 const char mapFile[] = \ /**< Statically compiled-in read only map file data. 
                           */
	"4 "\
	"0   0   C "\
	"2040 0   C "\
	"2040 880 C "\
	"0   880 C "\
	"0   1 "\
	"1   2 "\
	"2   3 "\
	"3   0 "\
	"0   0";	/* End marker for map file */
 

#if 0
 const char mapFile[] = \
	"16 "\
	"0   0   C "\
	"203 0   C "\
	"406 0   C "\
	"609 0   C "\
	"0   203 C "\
	"203 203 C "\
	"406 203 C "\
	"609 203 C "\
	"0   406 C "\
	"203 406 C "\
	"406 406 C "\
	"609 406 C "\
	"0   609 C "\
	"203 609 C "\
	"406 609 C "\
	"609 609 C "\
	"0   1 "\
	"1 2 "\
	"2 3 "\
	"4 5 "\
	"5 6 "\
	"6 7 "\
	"8 9 "\
	"9 10 "\
	"10 11 "\
	"12 13 "\
	"13 14 "\
	"14 15 "\
	"0 4 "\
	"4 8 "\
	"8 12 "\
	"1 5 "\
	"5 9 "\
	"9 13 "\
	"2 6 "\
	"6 10 "\
	"10 14 "\
	"3 7 "\
	"7 11 "\
	"11 15 "\
	"0   0";	/* End marker for map file */
#endif

 /** Returns next one byte from map file.
  */
 int mapReceiveByte(FILE *stream) {
 	static UINT filePtr = 0;

	if(filePtr < sizeof(mapFile)) {
		return mapFile[filePtr ++];
	}
	return _FDEV_EOF;
 }

 /** Initializes file system module. Redirects standard input and output to 
  *  ZigBee.
  */
 STATUS initFileSystem(void) {
    stdin = stdout = &zigbeeStream;
 	return STATUS_OK;
 }


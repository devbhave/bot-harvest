 #include <prjParams.h>
 #include <prjCommon.h>
 #include <hal/adc.h>
 #include <hal/lcd.h>
 #include <hal/motor.h>
 #include <hal/buzzer.h>
 #include <hal/zigbee.h>
 #include <guidanceSystem.h>
 #include <whiteLineFollower.h>
 #include <assert.h>
 #include <stdio.h>
 #include <string.h>

 int main() {
    Map thisMap;
	STATUS ret;

	initZigbee();

	PRINT("Initializing the harware...\r\n");
	
	/* Initialize hardware */
	ret = initLcd();
	ASSERT(ret == STATUS_OK);
 	ret = initAdc();
	ASSERT(ret == STATUS_OK);
	ret = initMotor();
	ASSERT(ret == STATUS_OK);
	ret = initZigbee();
	ASSERT(ret == STATUS_OK);

	/* Initialize software libraries */
	ret = initWhiteLineFollower();
	ASSERT(ret == STATUS_OK);
    ret = initBotGuidanceSystem(&thisMap);
	ASSERT(ret == STATUS_OK);

	printMap(&thisMap);
		
	/* Perform task */
    ret = gotoPosition(&thisMap, 0, 203);
	ASSERT(ret == STATUS_OK);
	//rotateRight(180);


	while(1);
    return 0;
 }

 #include <stdio.h>
 #include <string.h>
 #include <prjParams.h>
 #include <prjCommon.h>
 #include <hal/adc.h>
 #include <hal/lcd.h>
 #include <hal/motor.h>
 #include <hal/buzzer.h>
 #include <hal/zigbee.h>
 #include <guidanceSystem.h>
 #include <fileSystem.h>
 #include <whiteLineFollower.h>
 #include <assert.h>
 

 int main() {
    Map thisMap;
	STATUS ret;

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

	ret = initFileSystem();
	ASSERT(ret == STATUS_OK);
	ret = initWhiteLineFollower();
	ASSERT(ret == STATUS_OK);
    ret = initBotGuidanceSystem(MAP_FILE, &thisMap);
	ASSERT(ret == STATUS_OK);

    printf("\nMap summary:\n");
	printMap(&thisMap);
		
	/* Perform task */
    ret = gotoPosition(&thisMap, 609, 609);
	ASSERT(ret == STATUS_OK);
	//rotateRight(180);


	while(1);
    return 0;
 }

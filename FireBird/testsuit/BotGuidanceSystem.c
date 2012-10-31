 #include <stdio.h>
 #include <string.h>
 #include <prjParams.h>
 #include <prjCommon.h>
 #include <hal/adc.h>
 #include <hal/lcd.h>
 #include <hal/motor.h>
 #include <hal/servo.h>
 #include <hal/buzzer.h>
 #include <hal/zigbee.h>
 #include <hal/power.h>
 #include <guidanceSystem.h>
 #include <fileSystem.h>
 #include <whiteLineFollower.h>
 #include <assert.h>
 

 int main() {
    Map thisMap;
	STATUS ret;
	UINT x, y;
	int data;
	UINT sa1, sa2, sa3;

	/* Initialize hardware */
	ret = initLcd();
	ASSERT(ret == STATUS_OK);
 	ret = initAdc();
	ASSERT(ret == STATUS_OK);
	ret = initMotor();
	ASSERT(ret == STATUS_OK);
	ret = initZigbee();
	ASSERT(ret == STATUS_OK);
	ret = initServo();
	ASSERT(ret == STATUS_OK);
    ret = initPower();
    ASSERT(ret == STATUS_OK);

	/* Initialize software libraries */	

	ret = initFileSystem();
	ASSERT(ret == STATUS_OK);
	ret = initWhiteLineFollower();
	ASSERT(ret == STATUS_OK);
    ret = initBotGuidanceSystem(MAP_FILE, &thisMap);
	ASSERT(ret == STATUS_OK);

    //printf("\nMap summary:\n");
	//printMap(&thisMap);

    printf("Power off\n");
    powerOff(SG_ALL);
    _delay_ms(5000);
    printf("Power on\n");
    powerOn(SG_ALL);

#if 0
	sa1 = sa2 = sa3 = 90;
	ret = servoSet(SERVO1, sa1);
	ASSERT(ret == STATUS_OK);
	ret = servoSet(SERVO2, sa2);
	ASSERT(ret == STATUS_OK);
	ret = servoSet(SERVO3, sa3);
	ASSERT(ret == STATUS_OK);
#endif
    gotoPosition(&thisMap, 1000, 0);
    _delay_ms(10000);
    gotoForward(&thisMap, 200);
    while(1);
    gotoForward(&thisMap, 50);
    gotoForward(&thisMap, 50);

	/* Perform task */
	while(1) {
		/*printf("\n\nGoto:\n");
		printf("X: ");
		scanf("%u", &x);
		printf("Y: ");
		scanf("%u", &y);*/
		x = y = 0;
	    //ret = gotoPosition(&thisMap, x, y);
		//ASSERT(ret == STATUS_OK);
		
		data = 1;
		while(data != 'X') {
			data = getchar();
			switch(data) {
			case 'L': sa1 -= 10; servoSet(SERVO1, sa1); break;
			case 'R': sa1 += 10; servoSet(SERVO1, sa1); break;
			case 'U': sa2 += 10; servoSet(SERVO2, sa2); break;
			case 'D': sa2 -= 10; servoSet(SERVO2, sa2); break;
			case 'C': servoSet(SERVO3, 0); servoSet(SERVO3, 120);
					  servoSet(SERVO3, 0);servoSet(SERVO3, 120);
					  break;
			case 'F': y += 100;
					  ret = gotoPosition(&thisMap, x, y);
					  ASSERT(ret == STATUS_OK); break;
            case 'I': sa2 = 90; servoSet(SERVO2, sa2); break;
			}
		}

	}

	while(1);
    return 0;
 }

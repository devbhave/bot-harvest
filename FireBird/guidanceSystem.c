/* File: guidanceSystem.c
 * Description: Bot guidance system file
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

 #include <prjParams.h>
 #include <prjCommon.h>
 #include <stdio.h>
 #include <hal/adc.h>
 #include <hal/lcd.h>
 #include <hal/motor.h>
 #include <hal/zigbee.h>
 #include <guidanceSystem.h>
 #include <whiteLineFollower.h>
 #include <assert.h>
 #include <string.h>
 
 /* Globals */
 
 static BotLocation thisBotLocation;
 static BotOrientation thisBotOrientation;
 static char msg[256];

 void printMap(Map *pMap);
  
 static UINT intSqrt(ULINT x) {
    register ULINT op, res, one;

    op = x;
    res = 0ul;

    /* "one" starts at the highest power of four <= than the argument. */
    one = 1ul << 30;  /* second-to-top bit set */
    while (one > op) one >>= 2;

    while (one != 0ul) {
        if (op >= res + one) {
            op -= res + one;
            res += one << 1;  // <-- faster than 2 * one
        }
        res >>= 1;
        one >>= 2;
    }
    return (UINT)res;
 }
 
 static void floydWarshall(Map *pMap) {
	UINT k, i, j;
	UINT d_temp[MAX_NODES][MAX_NODES], p_temp[MAX_NODES][MAX_NODES];

    ASSERT(pMap != NULL);
    
	/* Initialize path matrix */
	for (i = 0; i < pMap->nodeCount; i++) {
		for (j = 0; j < pMap->nodeCount; j++) {
            if((i == j) || (pMap->distanceMatrix[i][j] == INFINITY))
                pMap->pathMatrix[i][j] = 0;
            else
                pMap->pathMatrix[i][j] = i;
		}
	}

	for (k = 0; k < pMap->nodeCount; k++) {
		for (i = 0; i < pMap->nodeCount; i++) {
			for (j = 0; j < pMap->nodeCount; j++) {
				d_temp[i][j] =
				    MIN(pMap->distanceMatrix[i][j],
					pMap->distanceMatrix[i][k] + pMap->distanceMatrix[k][j]);
				if(pMap->distanceMatrix[i][j] <= pMap->distanceMatrix[i][k] + pMap->distanceMatrix[k][j]) {
				    p_temp[i][j] = pMap->pathMatrix[i][j];
				}
				else {
				    p_temp[i][j] = pMap->pathMatrix[k][j];
				}				    
			}
		}

		for (i = 0; i < pMap->nodeCount; i++) {
			for (j = 0; j < pMap->nodeCount; j++) {
				pMap->distanceMatrix[i][j] = d_temp[i][j];
				pMap->pathMatrix[i][j] = p_temp[i][j];
			}
		}
	}
 }

 static STATUS loadMap(Map *map) {
    UINT idx, idx2;
	
	ASSERT(map != NULL);

    map->nodeCount = 4;
    
    map->nodeList[0].posX = 0;
    map->nodeList[0].posY = 0;
    map->nodeList[0].isCheckpoint = 1;

    map->nodeList[1].posX = 203;
    map->nodeList[1].posY = 0;
    map->nodeList[1].isCheckpoint = 1;

    map->nodeList[2].posX = 203;
    map->nodeList[2].posY = 203;
    map->nodeList[2].isCheckpoint = 1;

    map->nodeList[3].posX = 0;
    map->nodeList[3].posY = 203;
    map->nodeList[3].isCheckpoint = 1;

    for(idx = 0; idx < map->nodeCount; idx ++) {
        for(idx2 = 0; idx2 < map->nodeCount; idx2 ++) {
            map->weightMatrix[idx][idx2] = INFINITY;
        }
    }
    
    /* Assign weights */
    map->weightMatrix[0][1] = map->weightMatrix[1][0] = 203;
    map->weightMatrix[1][2] = map->weightMatrix[2][1] = 203;
    map->weightMatrix[2][3] = map->weightMatrix[3][2] = 203;
    map->weightMatrix[0][3] = map->weightMatrix[3][0] = 203;

    for(idx = 0; idx < map->nodeCount; idx ++) {
        for(idx2 = 0; idx2 < map->nodeCount; idx2 ++) {
            map->distanceMatrix[idx][idx2] = map->weightMatrix[idx][idx2];
        }
    }
    
    floydWarshall(map);
    
	return STATUS_OK;
 }

static UINT getNearestNode(Map *pMap, UINT x, UINT y){
    UINT minDist, minIdx;
    UINT idx, dist;
	ULINT x1, y1, x2, y2;
	ULINT x_sq, y_sq;
    
    ASSERT(pMap != NULL);
    
    minDist = INFINITY;
    minIdx = 0;
	x1 = x;	y1 = y;
    
    for(idx = 0; idx < pMap->nodeCount; idx ++) {
		x2 = pMap->nodeList[idx].posX;
		y2 = pMap->nodeList[idx].posY;

		/* Ignore signs */
		if(x1 < x2) {
			x_sq = (x2-x1)*(x2-x1);
		}
		else {
			x_sq = (x1-x2)*(x1-x2);
		}
		if(y1 < y2) {
			y_sq = (y2-y1)*(y2-y1);
		}
		else {
			y_sq = (y1-y2)*(y1-y2);
		}
		
        // Find Euclidean distance
        dist = intSqrt(x_sq + y_sq);

//		snprintf(msg, sizeof(msg), "dist[%d]: %u\r\n", idx, dist);
//		PRINT(msg);

        if(dist < minDist) {
            minDist = dist;
            minIdx = idx;
        }
    }
    
    return minIdx;
}
 
 STATUS initBotGuidanceSystem(Map *pMap) {
 
    ASSERT(pMap != NULL);
    
	/* Initialize current location */
	
	thisBotLocation.posX = thisBotLocation.posY = 0;
	thisBotLocation.errorX = thisBotLocation.errorY = EPSILON;
	thisBotOrientation = EASTWARD;
	
	/* Load map */
	
	if(loadMap(pMap) != STATUS_OK) {
		/* Error loading map */
		return !STATUS_OK;
	}
	
	return STATUS_OK;
 }
 
 /***** Debug routines *****/

 void printMap(Map *pMap) {
    UINT idx, idx2;	
    
    ASSERT(pMap != NULL);
    
    snprintf(msg, sizeof(msg), "nodeCount: %u\r\n", pMap->nodeCount);
	PRINT(msg);

    for(idx = 0; idx < pMap->nodeCount; idx ++) {
        snprintf(msg, sizeof(msg), "node #%02u: %4u %4u ", idx, pMap->nodeList[idx].posX,
            pMap->nodeList[idx].posY);
		PRINT(msg);
        if(pMap->nodeList[idx].isCheckpoint == TRUE) {
            snprintf(msg, sizeof(msg), "C\r\n");
			PRINT(msg);
        }
        else {
            snprintf(msg, sizeof(msg), "N\r\n");
			PRINT(msg);
        }
    }
    
    /* Print weight matrix */
    snprintf(msg, sizeof(msg),"\r\nWeight matrix:\r\n");
	PRINT(msg);
    for(idx = 0; idx < pMap->nodeCount; idx ++) {
        for(idx2 = 0; idx2 < pMap->nodeCount; idx2 ++) {
            if(pMap->weightMatrix[idx][idx2] >= INFINITY) {
                snprintf(msg, sizeof(msg), "INF ");
				PRINT(msg);
            }
            else{
                snprintf(msg, sizeof(msg), "%3u ", pMap->weightMatrix[idx][idx2]);
				PRINT(msg);
            }
        }
        snprintf(msg, sizeof(msg), "\r\n");
		PRINT(msg);
    }

    /* Print distance matrix */
    snprintf(msg, sizeof(msg), "\r\nDistance matrix:\r\n");
	PRINT(msg);
    for(idx = 0; idx < pMap->nodeCount; idx ++) {
        for(idx2 = 0; idx2 < pMap->nodeCount; idx2 ++) {
            if(pMap->distanceMatrix[idx][idx2] >= INFINITY) {
                snprintf(msg, sizeof(msg), "INF ");
				PRINT(msg);
            }
            else{
                snprintf(msg, sizeof(msg), "%3u ", pMap->distanceMatrix[idx][idx2]);
				PRINT(msg);
            }
        }
        snprintf(msg, sizeof(msg), "\r\n");
		PRINT(msg);
    }

    /* Print path matrix */
    snprintf(msg, sizeof(msg), "\n\rPath matrix:\r\n");
	PRINT(msg);
    for(idx = 0; idx < pMap->nodeCount; idx ++) {
        for(idx2 = 0; idx2 < pMap->nodeCount; idx2 ++) {
            if(pMap->pathMatrix[idx][idx2] >= INFINITY) {
                snprintf(msg, sizeof(msg), "INF ");
				PRINT(msg);
            }
            else{
                snprintf(msg, sizeof(msg), "%3u ", pMap->pathMatrix[idx][idx2]);
				PRINT(msg);
            }
        }
        snprintf(msg, sizeof(msg), "\r\n");
		PRINT(msg);
    }
}

 static int getAngleFromOrientation(BotOrientation or) {
    const int angle[] = {0, 90, 180, 270};
    ASSERT((or == EASTWARD) || (or == NORTHWARD) || (or == WESTWARD) ||
           (or == SOUTHWARD));
    return angle[or];
 }
 
STATUS gotoPosition(Map *pMap, UINT posX, UINT posY) {
    UINT srcNode, destNode, prevNode;
    UINT shortestPath[MAX_NODES];
    UINT idx;
    BotLocation nextLocation;
    BotOrientation nextOrientation;
    int rotation;
	STATUS ret;
	UINT i;
  
    ASSERT(pMap != NULL);
  
    /* Step #1: Get shortest path from current location to destination */
    
    /* TODO: Using sub-optimal location choice, improve later */
    srcNode = getNearestNode(pMap, thisBotLocation.posX, thisBotLocation.posY);
    destNode = getNearestNode(pMap, posX, posY);

	// debug start
	snprintf(msg, sizeof(msg), "Current location: (%d, %d)\r\n", thisBotLocation.posX, thisBotLocation.posY);
	PRINT(msg);
	snprintf(msg, sizeof(msg), "Goto location: (%d, %d)\r\n", posX, posY);
	PRINT(msg);
	snprintf(msg, sizeof(msg), "src node: %d\r\n", srcNode);
	PRINT(msg);
	snprintf(msg, sizeof(msg), "dest node: %d\r\n", destNode);
	PRINT(msg);
	//debug end
  
    idx = 0;
    shortestPath[0] = destNode;
    prevNode = destNode;
    while(prevNode != srcNode) {
        idx = idx + 1;
        prevNode = pMap->pathMatrix[srcNode][prevNode];
        shortestPath[idx] = prevNode;

        ASSERT(idx < MAX_NODES);
    }
    
    /* shortestPath[idx] holds srcNode index */

	// debug start
	i = idx;
    while(i != 0) {
        snprintf(msg, sizeof(msg), "%d, ", shortestPath[i]);
		PRINT(msg);
        i --;
    }
    snprintf(msg, sizeof(msg), "%d\n", shortestPath[0]);
	PRINT(msg);
    // debug end

    /* Step #2: Go from current location to start of shortest path */
    
    /* Step #3: Move along shortest path */

    /* LOOP INVARIANT: Bot is standing at location of the node shortestPath[idx]
     */
    while(idx != 0) {
		/* Assert loop invariant */
		ASSERT((thisBotLocation.posX == pMap->nodeList[shortestPath[idx]].posX) &&
		       (thisBotLocation.posY == pMap->nodeList[shortestPath[idx]].posY));

        /* Step #3.a: Retrieve next node location */

        nextLocation.posX = pMap->nodeList[shortestPath[idx-1]].posX;
        nextLocation.posY = pMap->nodeList[shortestPath[idx-1]].posY;

        if(pMap->nodeList[idx-1].isCheckpoint == TRUE) {
            /* Error in locating checkpoint is known */
            nextLocation.errorX = EPSILON;
            nextLocation.errorY = EPSILON;
        }
        else {
            /* TODO:  Error in locating checkpoint is to be computed */
            /* Since all nodes are checkpoints, this condition shall never occur
             */
             ASSERT(0);
        }
        
		// debug start
		snprintf(msg, sizeof(msg), "Current location: (%d, %d)\r\n", thisBotLocation.posX, thisBotLocation.posY);
		PRINT(msg);
		snprintf(msg, sizeof(msg), "Next location: (%d, %d)\r\n", nextLocation.posX, nextLocation.posY);
		PRINT(msg);
		//debug end

        /* Step #3.b: Set Bot orientation */
		nextOrientation = thisBotOrientation;
        if( (thisBotLocation.posX < nextLocation.posX) && 
            (thisBotLocation.posY == nextLocation.posY) ) {
            nextOrientation = EASTWARD;
        }
        else if( (thisBotLocation.posX > nextLocation.posX) && 
            (thisBotLocation.posY == nextLocation.posY) ) {
            nextOrientation = WESTWARD;
        }
        else if( (thisBotLocation.posX == nextLocation.posX) && 
            (thisBotLocation.posY < nextLocation.posY) ) {
            nextOrientation = NORTHWARD;
        }
        else if( (thisBotLocation.posX == nextLocation.posX) && 
            (thisBotLocation.posY > nextLocation.posY) ) {
            nextOrientation = SOUTHWARD;
        }
        else {
            /* Fatal Logical Error */        
            ASSERT(0);
        }

		// debug start
		snprintf(msg, sizeof(msg), "thisBotOrientation: %d\r\n", thisBotOrientation);
		PRINT(msg);
		snprintf(msg, sizeof(msg), "nextOrientation: %d\r\n", nextOrientation);
		PRINT(msg);
		// debug end

        if(thisBotOrientation != nextOrientation) {
            rotation = getAngleFromOrientation(nextOrientation) - 
                getAngleFromOrientation(thisBotOrientation);
                
            ASSERT(rotation != 0);
            if(rotation > 180) { rotation = rotation - 360;}
            else if(rotation < -180) { rotation = rotation + 360;}

		// debug start
		snprintf(msg, sizeof(msg), "rotation: %d\r\n", rotation);
		PRINT(msg);
		// debug end

            if(rotation > 0) {
                rotateBot(LEFT, rotation);
            }
            else {
                rotateBot(RIGHT, rotation);
            }
             
            thisBotOrientation = nextOrientation;            
        }

        /* Step #3.c: Follow the white line till next node */
        /* TODO: Add code */
		ret = moveForwardFollwingLineByCheckpoint(1);
		ASSERT(ret == STATUS_OK);
		
		ret = moveForwardFollwingLineByDistance(65);
		ASSERT(ret == STATUS_OK);
		
        thisBotLocation = nextLocation;
        
        idx --;
    }
    
    /* Bot is currently standing at end node in shortest path */
    
    /* Step #4: Go from end of shortest path to desired location */

	return STATUS_OK;
}



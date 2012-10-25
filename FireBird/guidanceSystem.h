/* File: guidanceSystem.h
 * Description: Headers for Bot guidance system
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

 #define EPSILON 		5	/* Minimum error in mm */
 #define LOC_ERR_PER_METRE  30  /* In mm */
 #define LOC_ERR_FOR_DIST_MM(x) ((x) * LOC_ERR_PER_METRE / 100)
 #define MAX_NODES      10  /* Maximum nodes supported in any graph */
 #define INFINITY       0xFFFF/2
 
 typedef struct tagBotLocation {
	UINT posX, posY;		/* Current Bot position in mm */
	UINT errorX, errorY;    /* Over-estimated error in mm */
 } BotLocation;
 
 typedef struct tagNode {
	UINT posX, posY;		/* Co-ordinates of node in mm */
	BOOL isCheckpoint;		/* Whether node is checkpoint */
 } Node;
 
 typedef struct tagMap {
	UINT nodeCount;				/* No. of nodes in the map */
	Node nodeList[MAX_NODES];	/* List of all nodes in the map */
	UINT weightMatrix[MAX_NODES][MAX_NODES];	/* Map represented as graph */
	UINT distanceMatrix[MAX_NODES][MAX_NODES];  /* Shortest distance matrix */
	UINT pathMatrix[MAX_NODES][MAX_NODES];		/* Shortest path matrix */
 } Map;
 
 typedef enum tagBotOrientation {
    EASTWARD  = 0,
    NORTHWARD = 1,
    WESTWARD  = 2,
    SOUTHWARD = 3
 } BotOrientation;

 STATUS initBotGuidanceSystem(Map *pMap);
 STATUS gotoPosition(Map *pMap, UINT posX, UINT posY);

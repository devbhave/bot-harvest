/** @file prjCommon.h
 * Common definitions for the project
 */

/*
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

/*! \mainpage Greenhouse Harvesting And Monitoring Using Automatic Bot Guidance System
 *
 * \section intro_sec Introduction
 *
 * Greenhouse is the building in which plants are grown. Building is covered with various
covering materials like plastic sheets or glass. This allows solar or artificial light to enter the
building, but traps the heat inside the building. Weather conditions inside a greenhouse
can be controlled. Our project aims to harvest crops in such greenhouses completely
autonomously.

 *
 * \section install_sec Problem Statement
Project consists of two key entities – greenhouse and user. A single greenhouse consists of
greenhouse building with plants arranged in aisles and troughs formation, greenhouse controller and the Bot. 
There could be multiple such greenhouses each one equipped with its
own greenhouse controller and the Bot. The entity user refers to human farmer. 
Greenhouse controller autonomously manages greenhouse functions like sunlight control using
sun shades, humidity control using blowers, etc. It has its own set of sensors, actuators
and control logic. User merely specifies its parameters like amount of sunlight needed for
plants, permissible humidity range, etc. Operation of such greenhouse controller is out of
scope for this project.

Each greenhouse has one Bot to carry out certain farming tasks. Bot consists of FireBird
robot with mounted wireless network camera and robotic arm. 
User communicates with Bot via suitable technology. Such communication method
is assumed to be present. We have used ZigBee point-to-point link for communication
between Linux machine and FireBird bot. User assigns work to the Bot. Bot has capability
to move anywhere inside the greenhouse, to take photos and videos of plants. It may
communicate with greenhouse controller, monitor plant growth, harvest crops and alert
user when necessary.

\section Map File Format
Map of the green house is specified in simple text file. Map is graph where each
node as associated Cartesian co-ordinates. Co-ordiantes are measured in 
millimeters.

Typical map file looks like this. Note that contents of the map file are on left half. Right half
below is explanation for that line, and should \b not be included in map file.
 \code
	4                       <-- First row: Number of nodes in the graph 
	0    0    C             (Node index #0)     +--+  Description of each node.
	2040 0    C             (Node index #1)        |  First column is X co-ordiantes of the node on the map
	2040 880  C             (Node index #2)        |  Second column is Y co-ordiantes of the node on the map
	0    880  C             (Node index #3)     +--+  Third column specifies node is checkpoint.
	0    1                   +------+   
	1    2                          |  List of undirected edges between nodes.
	2    3                          | <-- e.g. This line says that there is edge between nodes with index 2 and 3
	3    0                   +------+   
	0    0	
 \endcode
 *
 */

 #ifndef PRJ_COMMON_H
 #define PRJ_COMMON_H

 /* Common definitions */

 #define sbit(reg,bit)  reg |= (1<<bit)
 #define cbit(reg,bit)  reg &= ~(1<<bit)
 #define STATUS         int
 #define STATUS_OK      0
 #define INT_LOCK()		cli()
 #define INT_UNLOCK()	sei()
 #define INTR_ON		1
 #define INTR_OFF		0
 #define MIN(x, y)		(((x) < (y))? (x):(y))
 #define MAX(x, y)		(((x) < (y))? (y):(x))
 #define TRUE			1
 #define FALSE			0
 
 typedef unsigned char  BYTE;
 typedef unsigned int  	UINT;
 typedef unsigned long int 	ULINT;
 typedef unsigned int  	BOOL;
 #ifndef NULL
 #define NULL 			(void*)0
 #endif

 /* Common headers */

#ifdef USE_AVR_STUDIO
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
#endif

 #endif /* PRJ_COMMON_H */
 

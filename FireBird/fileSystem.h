/** @file fileSystem.h
 * File system headers.
 */

/*
 * Written By: Devendra Bhave (devendra@cse.iitb.ac.in)
 * Copyright (c) IIT Bombay. All Rights Reserved.
 */

 #ifndef FILESYSTEM_H
 #define FILESYSTEM_H

 #define MAP_FILE	(&mapStream)    /**< File handle for compiled-in map file */

 extern FILE mapStream;
 extern const char mapFile[];
 extern STATUS initFileSystem(void);

 #endif /* FILESYSTEM_H */

/**
 * Handles everything to do with reading our PBRAIN programs into memory
 * @author: Kyle Goodale
 * @date: 09/06/2017
 */


#ifndef OPERATINGSYSTEM_FILELOADER_H
#define OPERATINGSYSTEM_FILELOADER_H

/**
 * Loads the file passed into the memory array passed starting at the execution contexts current pcb base register address
 * filePath: The path of the input file
 * Return 0 on success, -1 on failure
 */
int loadFile( char *filePath );

#endif //OPERATINGSYSTEM_FILELOADER_H


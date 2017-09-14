/**
 * Handles everything to do with reading our PBRAIN programs into memory
 * @author: Kyle Goodale
 * @date: 09/06/2017
 */


#ifndef OPERATINGSYSTEM_FILELOADER_H
#define OPERATINGSYSTEM_FILELOADER_H

#endif //OPERATINGSYSTEM_FILELOADER_H

/**
 * Loads the file passed into the memory array passed starting at the position passed
 * filePath: The path of the input file
 * memory: A pointer to the VM's main memory
 * memoryStartLocation: A pointer to the current line we are reading to, usually 0 unless we are reading more then one
 * program into memory.
 * Return 0 on success, -1 on failure
 */
int loadFile( char *filePath );
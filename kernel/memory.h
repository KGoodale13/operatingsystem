/**
 * Handles all operations having to do with the os memory.
 * Provides a lot of helper functions for interacting with memory and the registers
 * These operations are from the perspective on the whole OS memory array and don't take the
 * current execution context into consideration. To interact with memory using a programs execution context
 * use the methods found in executionContext.h
 * @author: Kyle Goodale
 * @date: 09/06/2017
 */


#ifndef OPERATINGSYSTEM_MEMORY_H
#define OPERATINGSYSTEM_MEMORY_H

const int MEMORY_LINES = 1000; // How many rows of memory we have

// Dumps main memory to console for debugging purposes
void _debug_dumpMemory();

/**
 * Writes an entire line into memory at the location specified
 * @param location - The line in memory to write to
 * @param line - The 6 character array to write into memory
 * @return 0 on success, -1 on error
 */
int _memoryWriteLine( int location, char *line );

/**
 * Reads a raw line from memory
 * @param location - The line in memory to write to
 * @return 0 on success, -1 on error
 */
char* _memoryReadLine( int location);

/**
 * Reads a line of memory as a value, this line must be in the format ZZ0000 -> ZZ9999
 * @param location - The line in memory to write to
 * @return 0 on success, -1 on error
 */
int _memoryReadValue( int location );

/**
 * Writes the number value to the memory location in the format ZZ0000 -> ZZ9999
 * @param location - The line in memory to write to
 * @param value - The value to write, max 9999
 * @return 0 on success, -1 on error
 */
int _memoryWriteValue( int location, int value );

/**
 * Clears 'lines' number of lines starting from the 'fromLocation'
 * @param fromLocation - The start location
 * @param lines  - The number of lines to clear
 * @return 0 on success, -1 on error
 */
int _memoryClearBlock( int fromLocation, int lines );

#endif OPERATINGSYSTEM_MEMORY_H
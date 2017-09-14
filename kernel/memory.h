/**
 * Handles all operations having to do with the os memory.
 * Provides a lot of helper functions for interacting with memory and the registers
 * @author: Kyle Goodale
 * @date: 09/06/2017
 */


#ifndef OPERATINGSYSTEM_MEMORY_H
#define OPERATINGSYSTEM_MEMORY_H

#endif //OPERATINGSYSTEM_MEMORY_H

// Main memory
extern char memory[][6];

// Operating System Registers:
extern short int PC; // Program counter
extern short int P0, P1, P2, P3; // Pointer registers
extern int R0, R1, R2, R3, ACC; // Registers and accumulator
extern char PSW[2], SP[2], IR[6];


/**
 * @param index - The index of the pointer you want. 0-3
 * @return The value stored at the pointer or -1 if error
 */
int getPointer( int index );

/**
 * @param index - The index of the pointer you want. 0-3
 * @param value - The value to store in the pointer. This should be a memory location.
 * @return 0 on success -1 on error
 */
int setPointer( int index, int value );

/**
 * @param index - The index of the pointer you want. 0-3
 * @param value - The value to adjust the pointer value by.
 * @return 0 on success -1 on error
 */
int adjustPointer( int index, int amount );

/**
 * @param index - The index of the register you want. 0-3
 * @return The value stored at the register or -1 if error
 */
int getRegister( int index );

/**
 * @param index - The index of the register you want. 0-3
 * @param value - The value to store in the register.
 * @return 0 on success -1 on error
 */
int setRegister( int index, int value );

/**
 * Writes the passed number to the memory location specified
 * @param location - The line of memory you want to write to. 0-99
 * @param value - The value to store at this location. int of max size 9999
 * @return 0 on success -1 on error
 */
int writeValueToMemory( int location, int value);

/**
 * Reads a value stored in memory. i.e ZZ3000
 * @param location - The line of memory you want to read from
 * @return The value stored there or -1 on error
 */
int readValueFromMemory( int location );

/**
 * Reads a value stored in memory at the location the pointer has stored
 * @param index - The pointer index we should use for getting a memory location
 * @return The value stored there or -1 on error
 */
int getPointerMemoryValue( int index );

/**
 * Sets the ACC to the value passed
 * @param value - The value to set the ACC to. MAX 9999
 * @return 0 on success -1 on error
 */
int setAccumulator( int value );

/**
 * Increments the ACC by the amount passed
 * @param value - The value to increment the ACC by. This will overflow if current_val + value > 9999
 * @return 0 on success -1 on error
 */
int addAccumulator( int amount );

/**
 * Decrements the ACC by the amount passed
 * @param value - The value to decrement the ACC by. This will overflow if current_val + value > 9999
 * @return 0 on success -1 on error
 */
int subtractAccumulator( int amount );

/**
 * Gets the value stored in the ACC
 * @return The value currently in the ACC
 */
int getAccumulator( );

/**
 * Sets the PSW[0] value to either 1 or 0
 * @param bool - The value to set PSW[0] to (1 or 0)
 */
int setPSW( int boolean );

/**
 * @return - Returns PSW[0] (1 or 0)
 */
int getPSW();

/**
 * Sets the program counter to the value passed
 * @param value - The value to set PC to
 */
int branch( int value );

/**
 * Compares the accumulators value to the value passed based on the comparison type enum passed
 * @param value - The value to compare
 * @return 0 -> False, 1 -> True, -1 -> Error
 */
int accCompare( int value, short int comparisonType );


/**
 * Coverts however many values passed as len starting at start to one number.
 * @param arr - The array to parse values from
 * @param start - The starting index
 * @param len  - How many indexes to include
 * @return An int representing the values at those indexes i.e [3,4] = 34
 */
int parseArray( const char arr[], int start, int len );

// Dumps main memory to console for debugging purposes
void dumpMemory();
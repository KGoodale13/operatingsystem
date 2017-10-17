/**
 * Execution Context provides an abstraction layer for interacting with a process it handles all the
 * details of interacting with memory and registers associated with the current running process.
 *
 * By: Kyle Goodale
 * Date: 10/02/2017
 */

#ifndef OPERATINGSYSTEM_EXECUTIONCONTEXT_H
#define OPERATINGSYSTEM_EXECUTIONCONTEXT_H


/**
 * Sets the Execution Context's CurrentPCB, you can use this to switch between which PCB these operations will use
 * @param PID - The PID to switch to
 * @return 0 on success, -1 on error
 */
int ExecutionContext_SwitchProcess( int PID );

// Returns the current executing processes id or NULL if there is none
int getPID();

// Returns the instruction register
char *getIR();

// loads the IR with the current line at PC
int loadIR();


/**************************************************************************************
 *    Functions for interacting with the current processes PCB and memory locations  **
 **************************************************************************************/


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
 * Writes the line passed to the location in memory relative to the CurrentPCB's base address
 * @param location - The location to write to (relative to the currentPCB)
 * @param line - The line to write to that location
 * @return -1 on error, 0 on success
 */
int writeLineToMemory( int location, char* line );

/**
 * Reads an entire line directly from memory with no formatting or parsing
 * @param location - The location in memory to read from relative to the current pcb
 * @return The line found out that location or NULL
 */
char *readLineFromMemory( int location );

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
int branch( short int value );


/**
 * Increments the program counter
 * @return 0-> On success, -1 -> Error
 */
int incrementPC();

/**
 * Increments the program counter
 * @return 0-> On success, -1 -> Error
 */
int incrementPC();

/**
 * Increments the PC and loads the next line into the IR, then checks the IR for line ending
 * @return 0 -> on success, 1 -> End reached, -1 -> On Error
 */
int PCNextLine();

#endif //OPERATINGSYSTEM_EXECUTIONCONTEXT_H
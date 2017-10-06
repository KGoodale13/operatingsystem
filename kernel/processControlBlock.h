#include "memory.h"

#ifndef OPERATINGSYSTEM_PROCESSCONTROLBLOCK_H
#define OPERATINGSYSTEM_PROCESSCONTROLBLOCK_H

// The struct we will use to store memory associated with the current state of the program running
typedef struct {
    int PID; // This Processes ID
    int R0, R1, R2, R3; // Our Registers 0-3
    short int PC, P0, P1, P2, P3; // Our Program counter, and Pointer Registers
    int ACC;
    char PSW[2];
    char IR[6]; // The current instruction being processed
    int BaseReg, LimitReg; // The min and max memory address
} PCB;

int ProcessCount;
const int MAX_PROCESSES = 10;
const int MAX_MEM_PER_PROC = MEMORY_LINES / MAX_PROCESSES; // How many lines each process gets in main memory

/**
 * Creates a new PCB for a program and returns its PID
 * @return The PID of the new PCB or -1 on failure
 */
int newPCB();

/**
 * Gets a programs PCB by its ID
 * @param PID
 * @return PCB - Returns a pointer to the Program Control Block corresponding to that PID or nullptr if not found
 */
PCB *getPCB( int PID );


/**
 * Destroys the PCB associated with the PID passed and wipes the main memory associated with it
 * @param PID
 * @return 1 on success, 0 on failure or not found, -1 on error
 */
int destroyPCB( int PID );


// Dumps the entire PCB linked list and information about each PCB. Useful for debugging
void _debug_DumpPCBs();


#endif OPERATINGSYSTEM_PROCESSCONTROLBLOCK_H


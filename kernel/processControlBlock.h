

#ifndef OPERATINGSYSTEM_PROCESSCONTROLBLOCK_H
#define OPERATINGSYSTEM_PROCESSCONTROLBLOCK_H/

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
extern const int MAX_PROCESSES;
extern const int MAX_MEM_PER_PROC; // How many lines each process gets in main memory

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
 * Resets memory unowned by the PCB passed and frees all memory allocated to it and removes it from the pcb list
 * @param PCB - The PCB
 * @return 1 on success, 0 on failure or not found, -1 on error
 */
static void destroyPCB( PCB *PCB );

/**
 * Removed the pcb identified by the pid passed
 * @param PID - the process pid
 * @return 1 on success, 0 on failure or not found, -1 on error
 */
int removePCBNode( int PID );


/**
 * Forks the current PCB, creating a copy of the current PCB and setting it to use the same
 * memory and also setting the PC to PC + 1
 */
int forkPCB( PCB *PCB );

// Dumps the entire PCB linked list and information about each PCB. Useful for debugging
void _debug_DumpPCBs();


#endif //OPERATINGSYSTEM_PROCESSCONTROLBLOCK_H


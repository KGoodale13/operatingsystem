// Created by Kyle Goodale. See header for details

#include "processControlBlock.h"
#include "memory.h"
#include <stddef.h>
#include <stdlib.h>
#include <printf.h>

int ProcessCount = 0; // How many processes we currently have
const int MAX_PROCESSES = 10;
const int MAX_MEM_PER_PROC = 100; // How many lines each process gets in main memory

// The PCBNode struct is just a wrapper around the PCB struct that contains a link to the next PCB.
// We use a separate struct because we shouldn't and don't need to expose that externally.
typedef struct PCBNode PCBNode;
struct PCBNode {
    PCBNode *NextPCB;
    PCB *pcb;
} ;

// The head and Tail of our PCB linked list
PCBNode *PCBListHead = NULL;

// Find the node and remove it and set its previous node to the current nodes next, 0 on success, -1 on not found
int removePCBNode( int PID ){

    struct PCBNode *CurrentNode = PCBListHead;
    struct PCBNode *PreviousNode = NULL;

    while( CurrentNode != NULL ) {
        if( CurrentNode->pcb->PID == PID ){
            // Update the previous node if it exists
            if( PreviousNode != NULL ) {
                PreviousNode->NextPCB = CurrentNode->NextPCB;
            }
            // Free our memory
            destroyPCB( CurrentNode->pcb );
            free( CurrentNode );
            return 0;
        }
        PreviousNode = CurrentNode;
        CurrentNode = CurrentNode->NextPCB;
    }
    return -1; // PCB with that PID not found
}

PCB *getPCB( int PID ){
    struct PCBNode *CurrentNode = PCBListHead;
    while( CurrentNode != NULL ) {
        if( CurrentNode->pcb->PID == PID ){
            return CurrentNode->pcb;
        }
        CurrentNode = CurrentNode->NextPCB;
    }
    return NULL; // PCB with that PID not found
}

// Creates a new pcb and pcbnode and adds it to the linked list
int newPCB() {
    if( ProcessCount == MAX_PROCESSES ) {
        printf("Error: Max process count exceeded. A maximum of %i programs may be loaded at once.\n", MAX_PROCESSES);
        return -1;
    }
    ProcessCount++;

    // Create the PCBNode that will contain the PCB in our linked list
    PCBNode *NewPCBNode;
    NewPCBNode = ( PCBNode * ) malloc( sizeof( PCBNode ) );

    // Initialize the new PCB with default data
    PCB *NewPCB;
    NewPCB = ( PCB * ) malloc( sizeof( PCB ) );
    NewPCB->PID = ProcessCount;
    NewPCB->PC = 0;
    NewPCB->R0, NewPCB->R1, NewPCB->R2, NewPCB->R3 = 0;
    NewPCB->P0, NewPCB->P1, NewPCB->P2, NewPCB->P3 = 0;
    NewPCB->BaseReg = MAX_MEM_PER_PROC * (ProcessCount-1); // Each program gets 100 lines of memory
    NewPCB->LimitReg = NewPCB->BaseReg + MAX_MEM_PER_PROC - 1;

    // Add the PCB to the node
    NewPCBNode->pcb = NewPCB;
    NewPCBNode->NextPCB = NULL;

    // Insert our node into the end of the linked list
    if( PCBListHead == NULL ){ // This is the first element
        PCBListHead = NewPCBNode;
    } else {
        PCBNode *CurrentPCBNode = PCBListHead;
        while (CurrentPCBNode->NextPCB != NULL) {
            CurrentPCBNode = CurrentPCBNode->NextPCB;
        }
        CurrentPCBNode->NextPCB = NewPCBNode;
    }




    return NewPCB->PID;
}

int forkPCB( PCB *originalPCB ) {

    // Create our fork process and set the values needed to match our original process
    int pid = newPCB();
    PCB* pcb = getPCB( pid );
    pcb->PC = (short)(originalPCB->PC + 1);
    pcb->BaseReg = originalPCB->BaseReg;
    pcb->LimitReg = originalPCB->LimitReg;

    return pid;

}


static void destroyPCB( PCB* targetPCB ){
    _memoryClearBlock( targetPCB->BaseReg, MAX_MEM_PER_PROC );
    free( targetPCB );
}


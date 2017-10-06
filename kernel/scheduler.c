// Created by Kyle Goodale. See header for details

#include "scheduler.h"
#include <stdlib.h>
#include <printf.h>
#include "executionContext.h"
#include "opcodes.h"
#include "processControlBlock.h"


// The QueueNode struct is used for our circular linked list queue
typedef struct QueueNode QueueNode;
struct QueueNode {
    QueueNode *next; // The next item in the queue
    int pid; // The pid of the process
};

// The current QueueNode we are processing
QueueNode *QueueHead = NULL;



static void removeFromQueue( int PID ){
    if( QueueHead == NULL ){
        printf("Attempted to remove process %i from empty queue!\n", PID);
        return;
    }

    // check if the queue head is the node
    if( QueueHead->pid == PID ){

    }

    QueueNode *CurrentNode = QueueHead->next;
    while( CurrentNode != QueueHead ){
        
    }
}


void Scheduler_Queue_Process( int PID ) {

    QueueNode *NewNode = ( QueueNode * ) malloc( sizeof( QueueNode ) );
    NewNode->next = QueueHead;
    NewNode->pid = PID;

    // Check if this is the first element to be inserted
    if( QueueHead == NULL ) {
        QueueHead = NewNode;
        QueueHead->next = QueueHead;
        return;
    }

    // Traverse to the end of the queue and then insert our new node there
    QueueNode *CurrentNode = QueueHead;
    while( CurrentNode->next != QueueHead && CurrentNode->next != CurrentNode ){
        CurrentNode = CurrentNode->next;
    }
    CurrentNode->next = NewNode;
    printf("Process: %i inserted into the scheduler queue.\n", PID);
}


int Scheduler_Start( ){

}









// The PCBNode struct is just a wrapper around the PCB struct that contains a link to the next PCB.
// We use a separate struct because we shouldn't and don't need to expose that externally.
typedef struct PCBNode PCBNode;
struct PCBNode {
    PCBNode *NextPCB;
    PCB *pcb;
} ;

// The head and Tail of our PCB linked list
PCBNode *PCBListHead = NULL;
PCBNode *PCBListTail = NULL;

// Find the node and remove it and set its previous node to the current nodes next, 0 on success, -1 on not found
static int removePCBNode( int PID ){

    struct PCBNode *CurrentNode = PCBListHead;
    struct PCBNode *PreviousNode = NULL;

    while( CurrentNode != NULL ) {
        if( CurrentNode->pcb->PID == PID ){
            // Update the previous node if it exists
            if( PreviousNode != NULL ) {
                PreviousNode->NextPCB = CurrentNode->NextPCB;
            }
            // Free our memory
            free( CurrentNode->pcb );
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

    // Set the current tails next PCB to this element, or if this is the first element added to the list set the tail
    if( PCBListTail != NULL ) {
        PCBListTail->NextPCB = NewPCBNode;
    } else { // For the first element in the list
        PCBListTail = NewPCBNode;
    }

    return NewPCB->PID;
}

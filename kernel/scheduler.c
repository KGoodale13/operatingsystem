// Created by Kyle Goodale. See header for details

#include "scheduler.h"
#include <stdlib.h>
#include <printf.h>
#include "executionContext.h"
#include "opcodes.h"



// The QueueNode struct is used for our circular linked list queue
typedef struct QueueNode QueueNode;
struct QueueNode {
    QueueNode *next; // The next item in the queue
    int waiting; // Is this process waiting for a semaphore?
    int pid; // The pid of the process
};

// The current QueueNode we are processing
QueueNode *QueueHead = NULL;


void Scheduler_Remove( int PID ) {
    if( QueueHead == NULL ){ return; } // Empty list case

    struct QueueNode *CurrentNode = QueueHead->next;
    struct QueueNode *PreviousNode = QueueHead;

    // If there's only one element in the list
    if( CurrentNode == PreviousNode ){
        if( CurrentNode->pid != PID ){
            printf("Unable to remove PID %i from queue (not found)[x00]\n", PID);
            return;
        }
        free( QueueHead );
        QueueHead = NULL;
        return;
    }

    while ( CurrentNode != NULL ) {
        if ( CurrentNode->pid == PID ) {
            if( CurrentNode == PreviousNode ){ // only one item in list
                PreviousNode = NULL;
            }
            // Update the previous node if it exists
            if ( PreviousNode != NULL ) {
                PreviousNode->next = CurrentNode->next;
            }
            free(CurrentNode);
            return;
        }

        PreviousNode = CurrentNode;
        CurrentNode = CurrentNode->next;
    }
    printf("Unable to remove PID %i from queue (not found)[x01]\n", PID);
}


void Scheduler_Queue_Process( int PID ) {

    QueueNode *NewNode = ( QueueNode * ) malloc( sizeof( QueueNode ) );
    NewNode->next = QueueHead;
    NewNode->waiting = 0;
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

// Sets the processes waiting status.
void Scheduler_Process_SetWaiting( int pid, int waiting ){
    if( QueueHead == NULL ){ return; } // Empty list case
    struct QueueNode *CurrentNode = QueueHead;
    do {
        if (CurrentNode->pid == pid) {
            CurrentNode->waiting = waiting;
            printf("Set waiting status of PID %i to %i\n", pid, waiting);
            return;
        }
        CurrentNode = CurrentNode->next;
    } while ( CurrentNode != QueueHead && CurrentNode != NULL );
}

// Overrides the timeslice to be a constant value instead of random
int IC_OVERRIDE = NULL;
void Scheduler_OverrideIC( int ic ){
    IC_OVERRIDE = ic;
}


int Scheduler_Start( enum error_handle error_handler ){

    int waitingCount = 0; // Used to detect deadlocks.
    while( QueueHead != NULL ) {

        if( QueueHead->waiting == 1 ){

            if( waitingCount > 10 ){ // We have a max of 10 processes so if 10 processes are waiting in a row we are at a deadlock
                printf("Deadlock detected. All processes are waiting on semaphores. The philosophers have starved! Aborting...\n");
                return -1;
            }

            printf("Skipping PID %i because it's waiting for a semaphore\n", QueueHead->pid);
            waitingCount++;
        } else {
            waitingCount = 0; // Reset the waiting count since this process isn't waiting

            int timeSlice = IC_OVERRIDE == NULL ? (rand() % 5 + 6 ) : IC_OVERRIDE; // Random timeslice between 5-10
            // Switch the execution context
            printf( "Switching Execution context to PID %i giving timeslice of size %i\n", QueueHead->pid, timeSlice );
            if( ExecutionContext_SwitchProcess( QueueHead->pid ) == -1 ) {
                printf( "Failed to switch to PID %i removing from scheduler\n", QueueHead->pid );
                Scheduler_Remove( QueueHead->pid );
            }


            // Run the program for its allocated amount of time
            int timeRemaining = timeSlice;
            while (timeRemaining >= 0 && QueueHead->waiting == 0 ) {
                timeRemaining = timeRemaining - 1;
                printf("Processing Opcode: %.6s for PID %i\n", getIR(), getPID());

                // Execute instruction
                if (processOpcode() == -1) {
                    printf("Exception encountered in PID %i. Program terminated. \n", getPID());

                    Scheduler_Remove(getPID());
                    if (error_handler == CONTINUE_ON_ERROR) {
                        if( QueueHead != NULL && QueueHead->next != NULL ) {
                            QueueHead = QueueHead->next;
                            Scheduler_Remove(getPID());
                            break;
                        } else{
                            return -1;
                        }
                    } else {
                        return -1;
                    }
                }
                // Check our next line for end of program
                int nextLineStatus = PCNextLine(); // Get the status of the next line
                if (nextLineStatus == 1) { // End of program
                    printf("Program execution complete for PID %i\n", getPID());

                    QueueHead = QueueHead->next;
                    Scheduler_Remove(getPID());
                    break;
                } else if (nextLineStatus == -1) {
                    printf("Error occurred while iterating to next line in PID: %i\n", getPID());
                    Scheduler_Remove(getPID());
                    if (error_handler == CONTINUE_ON_ERROR) {
                        QueueHead = QueueHead->next;
                        Scheduler_Remove(getPID());
                        break;
                    } else {
                        return -1;
                    }
                }
            }
            if (QueueHead == NULL) {
                printf("Scheduler completed running all programs.\n");
                return 0;
            }
        }
        QueueHead = QueueHead->next; // Move to the next program
    }
    return 0;
}
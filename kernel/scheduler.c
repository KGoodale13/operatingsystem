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
    int pid; // The pid of the process
};

// The current QueueNode we are processing
QueueNode *QueueHead = NULL;


static void removeFromQueue( int PID ) {
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


int Scheduler_Start( enum error_handle error_handler ){

    while( QueueHead != NULL ) {

        int timeSlice = rand() % 5 + 6; // Random timeslice between 5-10
        // Switch the execution context
        printf( "Switching Execution context to PID %i giving timeslice of size %i\n", QueueHead->pid, timeSlice );
        if( ExecutionContext_SwitchProcess( QueueHead->pid ) == -1 ) {
            printf( "Failed to switch to PID %i removing from scheduler\n", QueueHead->pid );
            removeFromQueue( QueueHead->pid );
        }

        // Run the program for its allocated amount of time
        int timeRemaining = timeSlice;
        while( timeRemaining >= 0 ) {
            timeRemaining = timeRemaining - 1;
            printf("Processing Opcode: %.6s for PID %i\n", getIR(), getPID());

            // Execute instruction
            if (processOpcode() == -1) {
                printf("Exception encountered in PID %i. Program terminated. \n", getPID());

                removeFromQueue( getPID() );
                if( error_handler == CONTINUE_ON_ERROR ){
                    QueueHead = QueueHead->next;
                    removeFromQueue( getPID() );
                    break;
                } else{
                    return -1;
                }
            }
            // Check our next line for end of program
            int nextLineStatus = PCNextLine(); // Get the status of the next line
            if( nextLineStatus == 1 ){ // End of program
                printf("Program execution complete for PID %i\n", getPID() );

                QueueHead = QueueHead->next;
                removeFromQueue( getPID() );
                break;
            } else if( nextLineStatus == -1 ) {
                printf("Error occurred while iterating to next line in PID: %i\n", getPID() );
                removeFromQueue( getPID() );
                if( error_handler == CONTINUE_ON_ERROR ){
                    QueueHead = QueueHead->next;
                    removeFromQueue( getPID() );
                    break;
                } else{
                    return -1;
                }
            }
        }
        if( QueueHead == NULL ){
            printf("Scheduler completed running all programs.\n");
            return 0;
        }
        QueueHead = QueueHead->next; // Move to the next program
    }
    return 0;
}
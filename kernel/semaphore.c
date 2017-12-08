//
// Created by Kyle Goodale on 11/28/17.
//
#include <printf.h>
#include <stdlib.h>
#include "scheduler.h"

typedef struct Semaphore Semaphore;
typedef struct ProcessQueueNode ProcessQueueNode;

struct ProcessQueueNode {
    int pid;
    ProcessQueueNode *next;
};

struct Semaphore {
    int count; // How many locks are available on this
    ProcessQueueNode *next; // Next process inline to use this semaphore
};

// Our "fork" semaphores ( For the dining philosopher problem, only one may use a fork at a time and there are 5 forks)
Semaphore semaphores[6]; // Semaphore 6 is our "doorman semaphore"

int semaphores_initialized = 0; // Used so we can lazy init our semaphores
void init_semaphores(){ // init all the semaphore counts to 1.
    for( int i = 0; i < 6; i++ ){
        semaphores[i].count = i == 5 ? 4 : 1; // The doorman semaphore has a count of 5
    }
    semaphores_initialized = 1;
}

// Adds the process to the end of the queue in the semaphore
static void QueueProcessInSemaphore( int pid, Semaphore *semaphore ){

    ProcessQueueNode *nextProcess = (ProcessQueueNode *) malloc(sizeof(ProcessQueueNode));
    nextProcess->pid = pid;
    nextProcess->next = NULL;
    if( semaphore->next == NULL ){
        semaphore->next = nextProcess;
    } else{
        // Traverse to the end of the queue for this semaphore
        ProcessQueueNode *node = semaphore->next;
        while( node != NULL && node->next != NULL ){
            node = node->next;
        }
        node->next = nextProcess;
    }

    // Set its status to waiting so the scheduler won't run it anymore
    Scheduler_Process_SetWaiting( pid, 1 );
}


// Attempts to lock the semaphore for the pid passed. If the semaphore is occupied it will remove the process from the scheduler queue
void Semaphore_Wait( int pid, int semaphoreID ){

    // Check if our semaphores have been initialized. If not do so now ( lazy initialization )
    if( semaphores_initialized == 0 ){
        printf("Semaphores not yet initialized. Initializing now...\n");
        init_semaphores();
    }

    if ( semaphoreID > 5 || semaphoreID < 0 ){
        printf("Error: Invalid SemaphoreID passed to Semaphore_Wait. Expected (0-4) Got: %i", semaphoreID);
        return;
    }

    Semaphore *semaphore = &semaphores[semaphoreID];

    semaphore->count = semaphore->count - 1;

    if( semaphore->count < 0 ){ //Semaphore is not available. We will queue this process and run it once it is
        QueueProcessInSemaphore( pid, semaphore );
        printf("Semaphore %i, is not available for pid %i. Process queued.\n", semaphoreID, pid);
        return;
    }

    printf("Semaphore %i, acquired by pid %i\n", semaphoreID, pid);
}

// We call this when a semaphore is released so we can give it to the next process in line
static void CheckSemaphoreQueue( int pid, Semaphore *semaphore ){
    if( semaphore->next != NULL ){
        // Pop the next process in the queue off the list
        ProcessQueueNode *head = semaphore->next;
        semaphore->next = head->next;

        // Set the waiting status back to 0 now that it has the semaphore it needed
        Scheduler_Process_SetWaiting( head->pid, 0 );

        free( head );
    }
}

// Called when the process is done with this semaphore and wants to free it
void Semaphore_Signal( int pid, int semaphoreID ){
    if ( semaphoreID > 5 || semaphoreID < 0 ){
        printf("Error: Invalid SemaphoreID passed to Semaphore_Wait. Expected (0-4) Got: %i", semaphoreID);
        return;
    }

    Semaphore *semaphore = &semaphores[semaphoreID];
    semaphore->count = semaphore->count + 1;
    CheckSemaphoreQueue( pid, semaphore );

    printf("Semaphore %i, released by pid %i\n", semaphoreID, pid);
}
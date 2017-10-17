/**
 * Handles scheduling of the different processes. So we can load multiple programs into memory and then run a
 * certain amount of operations on each and keep switching between them until they all complete.
 * @author: Kyle Goodale
 * @date: 10/05/2017
 */


#ifndef OPERATINGSYSTEM_SCHEDULER_H
#define OPERATINGSYSTEM_SCHEDULER_H

enum error_handle{
    CONTINUE_ON_ERROR, // If a program errors the scheduler will remove the program and continue with the others
    INTERRUPT_ON_ERROR // If a program fails it will be removed and the scheduler will stop and return
};

/**
 * Inserts the passed process identified by the passed PID into the execution queue.
 * @param PID - The PID of the process
 */
void Scheduler_Queue_Process( int PID );

/**
 * Starts running the processes in the queue. Will continue running them until it finishes
 * or a process throws an error. If a process throws an error the caller can decide what to do and
 * can continue the queue by calling Start again.
 * @return 0 on completion of all scheduled processes. -1 if a process throws an error.
 */
int Scheduler_Start( enum error_handle error_handler );

#endif //OPERATINGSYSTEM_SCHEDULER_H

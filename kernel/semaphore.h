//
// Created by Kyle Goodale on 11/28/17.
//

#ifndef OPERATINGSYSTEM_SEMAPHORE_H
#define OPERATINGSYSTEM_SEMAPHORE_H

void Semaphore_Wait( int pid, int semaphoreID );

void Semaphore_Signal( int pid, int semaphoreID );

#endif //OPERATINGSYSTEM_SEMAPHORE_H

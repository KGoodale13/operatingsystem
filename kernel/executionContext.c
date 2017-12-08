// Created by Kyle Goodale. See header for details


#include "../util/util.h"
#include "executionContext.h"
#include "processControlBlock.h"
#include "semaphore.h"
#include "memory.h"
#include "scheduler.h"

#include <printf.h>
#include <string.h>

static PCB *CurrentPCB = NULL;

// Switches our execution context to a different process
int ExecutionContext_SwitchProcess( int PID ){
    PCB *NewPCB = getPCB( PID );
    if( NewPCB == NULL ){
        printf("Error: Execution Context attempted to switch to invalid process. PID: %i. Ignoring.\n", PID);
        return -1;
    }
    CurrentPCB = NewPCB;

    if( CurrentPCB->PC == 0 ){ // We need a priming load because the IR wont be loaded initially
        loadIR();
    }

    return 0;
}

// Returns the current executing processes id
int getPID(){
    return CurrentPCB == NULL ? -1 : CurrentPCB->PID;
}

// Retrieves the memory location stored at a pointer
int getPointer( int index ){
    switch( index ){
        case 0: return CurrentPCB->P0;
        case 1: return CurrentPCB->P1;
        case 2: return CurrentPCB->P2;
        case 3: return CurrentPCB->P3;
        default:
            printf("Error: Attempted to access invalid pointer using getPointer. Expected (0-3), Got: %i\n", index);
            return -1;
    }
}

// Store a memory location in a pointer
int setPointer( int index, int value ){
    if( value < 0 || value > 99 ){
        printf("Error: Attempted to write to invalid pointer value. Valid locations are 0-%i, Got: %i\n", MAX_MEM_PER_PROC, value);
        return -1;
    }

    switch( index ){
        case 0: CurrentPCB->P0 = (short int) value; break;
        case 1: CurrentPCB->P1 = (short int) value; break;
        case 2: CurrentPCB->P2 = (short int) value; break;
        case 3: CurrentPCB->P3 = (short int) value; break;
        default:
            printf("Error: Attempted to write invalid memory address to pointer. Expected (0-3), Got: %i\n", index);
            return -1;
    }
    return 0;
}

int adjustPointer( int index, int amount ) {
    int pointerVal = getPointer( index );
    if( pointerVal == -1 ){
        printf("Error: Attempted to adjust value of invalid pointer. Expected (0-3), Got: %i\n", index);
    }
    return setPointer( index, pointerVal + amount );
}

// Gets the value stored in a register
int getRegister( int index ){
    switch( index ){
        case 0: return CurrentPCB->R0;
        case 1: return CurrentPCB->R1;
        case 2: return CurrentPCB->R2;
        case 3: return CurrentPCB->R3;
        default:
            printf("Error: Attempted to access invalid register index. Expected (0-3) Got: %i\n", index);
            return -1;
    }
}

// Stores a value in a register
int setRegister( int index, int value ){
    if( value < 0 || value > 2147483647 ){
        printf("Error: Attempted to write to invalid value to register. Expected (0-INT_MAX) Got: %i\n", value);
        return -1;
    }

    switch( index ){
        case 0: CurrentPCB->R0 = (short int) value; break;
        case 1: CurrentPCB->R1 = (short int) value; break;
        case 2: CurrentPCB->R2 = (short int) value; break;
        case 3: CurrentPCB->R3 = (short int) value; break;
        default:
            printf("Error: Attempted to access invalid pointer using getPointer. Expected (0-3), Got: %i\n", index);
            return -1;
    }
    return 0;
}

// Writes a number to a memory location in the format ZZXXXX where X:{0-9}
int writeValueToMemory( int location, int value ){
    if( location < 0 || location > MAX_MEM_PER_PROC ){
        printf("Error: Invalid line passed to branch. Expected 0-%i, Got: %i\n", MAX_MEM_PER_PROC, location);
        return -1;
    }

    return _memoryWriteValue( location + CurrentPCB->BaseReg, value  );
}

// Reads a number from memory that is in the format ZZXXXX where X:{0-9}
int readValueFromMemory( int location ) {
    if( location < 0 || location > MAX_MEM_PER_PROC ) {
        printf("Error: Attempted to read value from invalid memory location. Expected (0-%i), Got: %i\n", MAX_MEM_PER_PROC, location);
        return -1;
    }

   return _memoryReadValue( location + CurrentPCB->BaseReg );
}

// Writes the line passed into the OS memory with respect to the CurrentPCB's base memory address
int writeLineToMemory( int location, char* line ) {
    if( location < 0 || location > MAX_MEM_PER_PROC ) {
        printf("Error: Attempted to read value from invalid memory location. Expected (0-%i), Got: %i\n", MAX_MEM_PER_PROC, location);
        return -1;
    }
    return _memoryWriteLine( CurrentPCB->BaseReg + location, line );
}

// Reads an entire line directly from memory with no formatting or parsing
char *readLineFromMemory( int location ) {
    if( location < 0 || location > MAX_MEM_PER_PROC ) {
        printf("Error: Attempted to read value from invalid memory location. Expected (0-%i), Got: %i\n", MAX_MEM_PER_PROC, location);
        return NULL;
    }
    return _memoryReadLine( CurrentPCB->BaseReg + location );
}

// Gets the value stored in memory that the pointer points to
int getPointerMemoryValue( int index ){
    return readValueFromMemory( getPointer( index ) );
}

// Sets the accumulators value
int setAccumulator( int value ) {
    if( value < 0 || value > 2147483647 ){
        printf("Error: Attempted to assign out of range value to the accumulator. Max: 2147483647 Got: %i\n", value);
        return -1;
    }
    CurrentPCB->ACC = value;
    return 0;
}

// Adds the value passed to the accumulator
int addAccumulator( int amount ){
    return setAccumulator( CurrentPCB->ACC + amount );
}

// subtracts the value passed from the accumulator
int subtractAccumulator( int amount ){
    return setAccumulator(CurrentPCB-> ACC - amount );
}


int getAccumulator( ){
    return CurrentPCB->ACC;
}

// Sets PSW[0] only for now
int setPSW( int bool ){
    if( bool != 1 && bool != 0 ){
        printf("Error: Invalid value passed to setPSW. Expected 1 or 0, Got: %i\n", bool);
        return -1;
    }
    CurrentPCB->PSW[0] = (char)(bool + '0');
    return 0;
}

// Returns PSW[0] only for now
int getPSW(){
    return parseArray( CurrentPCB->PSW, 0, 1 );
}

// Sets the program counter to the line passed
int branch( short int line ){
    if( line < 0 || line > 99 ){
        printf("Error: Invalid line passed to branch. Expected 0-%i, Got: %i\n", MAX_MEM_PER_PROC, line);
        return -1;
    }
    CurrentPCB->PC = (short)(line - 1); // Because we increase the PC after this opcode completes
    return 0;
}

int incrementPC(){
    short newVal = (short)(CurrentPCB->PC + 1);
    if( newVal < 0 || newVal >= MAX_MEM_PER_PROC ){
        printf("Error: Attempted to set PC to out of bounds number Expected 0-%i, Got: %i\n", MAX_MEM_PER_PROC, newVal);
        return  -1;
    }
    CurrentPCB->PC = newVal;
    loadIR();
    return 0;
}

// increments the pc and checks the IR value to see if we have reached the EOF or EOP
int PCNextLine(){
    if( incrementPC() == -1 ){
        return -1;
    }

    if( strncmp( CurrentPCB->IR, "99ZZZZ", 6 ) == 0 || CurrentPCB->IR[0] == '\0'  ){ // Check for program ending
       return 1;
    }

    return 0;
}

int setPC( int pc ){
    if( pc < 0 || pc >= MAX_MEM_PER_PROC ){
        printf("Error: Attempted to set PC to out of bounds number Expected 0-%i, Got: %i\n", MAX_MEM_PER_PROC, pc);
        return  -1;
    }
    CurrentPCB->PC = (short)( pc );
    loadIR();
    return 0;
}

int loadIR() {
    strncpy( CurrentPCB->IR, _memoryReadLine( CurrentPCB->BaseReg + CurrentPCB->PC ), 6 );
    return 0;
}

// Returns the Instruction register of the CurrentPCB
char *getIR(){
    return CurrentPCB->IR;
}

// FORK
int fork(){
    int forkPID = forkPCB( CurrentPCB ); // Fork the current PCB
    Scheduler_Queue_Process( forkPID ); // Add it to the scheduler
    return 0;
}

// Used for system calls from brain (op36)
int trapInstruction( int opcode, int register2Index ) {
    printf("trap called with opcode %i\n", opcode );

    if( opcode == 1 ){ // Wait
        int semaphore = 0;
        int value = getRegister( register2Index );
        if( value == 0 ){ // Fork semaphore
            semaphore = getAccumulator();
        } else { // Doorman semaphore
            semaphore = 6;
        }
        Semaphore_Wait( getPID(), semaphore);
    } else if ( opcode == 2 ){ // Signal
        Semaphore_Signal( getPID(), getRegister( register2Index ) );
    } else if ( opcode == 3 ){ // Get PID
        setRegister( register2Index, getPID() );
    } else {
        printf("Unknown opcode %i passed to trap. Unable to continue\n", opcode );
        return -1;
    }
    return 0;
}

int modulo( int a, int b ) {
    setAccumulator( a % b );
    return 0;
}

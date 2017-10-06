#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "kernel/executionContext.h"
#include "kernel/processControlBlock.h"
#include "loader/fileloader.h"


/**
 * Entry point for running the os.
 * argv should contain paths to each program you want to load
 * @author: Kyle Goodale
 * @return:
 *          0: Program finished running with no errors
 *         -1: Program encountered an error. The error should be printed to console
 */

int main( int argc, char* argv[] ) {

    int argIndex = 0; // Current index we are trying
    int fileLoaded = false; // Whether we loaded a file

    // Load any valid program passed as a startup param into memory
    while( argIndex < argc ) {
        char *programFile = argv[ argIndex ];

        // Create a new process for our program to be loaded into
        int processId = newPCB();
        if( processId == -1 ){
            printf( "Process creation failed. Aborting current program loading. \n");
            continue;
        }

        ExecutionContext_SwitchProcess( processId );

        if( loadFile( programFile ) == -1 ) { // Load the file into main memory
            printf("Error: Program %s failed to load. Destroying associated PID %i\n", programFile, getPID());
            destroyPCB( processId );
        }

        argIndex++;
    }


    PC = 0; // Reset the program counter to the begining of the program

    while ( true ) { // OS loop
        // fetch current instruction and copy to ir
        strncpy( IR, memory[ PC ], 6 );

        if( strncmp( IR, "99ZZZZ", 6 ) == 0 || IR[0] == '\0'  ){ // Check for program ending
            printf("Program execution complete \n");
            dumpMemory();
            break;
        } else {
            printf("Processing Opcode: %.6s\n", IR);
            // Execute instruction
            if( processOpcode() == -1 ) {
                printf("Exception encountered. Program terminated. \n");
                return -1;
            }
            PC++;
        }

    }

    return 0;
}
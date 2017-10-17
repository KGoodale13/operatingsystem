#include <stdio.h>

#include "kernel/scheduler.h"
#include "kernel/executionContext.h"
#include "kernel/processControlBlock.h"
#include "loader/fileloader.h"
#include "kernel/memory.h"


/**
 * Entry point for running the os.
 * argv should contain paths to each program you want to load
 * @author: Kyle Goodale
 * @return:
 *          0: Program finished running with no errors
 *         -1: Program encountered an error. The error should be printed to console
 */

int main( int argc, char* argv[] ) {

    int argIndex = 1; // Current index we are trying
    int fileLoaded = 0; // Whether we loaded a file

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
            removePCBNode( processId );
        }
        fileLoaded = 1;
        Scheduler_Queue_Process( processId );

        argIndex++;
    }
    if( fileLoaded == 0 ){
        printf("Error: No valid program files loaded. Terminating... \n");
        return -1;
    }

    Scheduler_Start( CONTINUE_ON_ERROR );
    _debug_dumpMemory();
    return 0;
}
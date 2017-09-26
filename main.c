#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>

#include "loader/fileloader.h"
#include "kernel/memory.h"
#include "kernel/opcodes.h"

/**
 * Entry point for running the os. Argv[0] is expected to be the absolute path to
 * the compiled executable
 * @return:
 *          0: Program finished running with no errors
 *         -1: Program encountered an error. The error should be printed to console
 */

int main( int argc, char* argv[] ) {

    int argIndex = 0; // Current index we are trying
    int fileLoaded = false; // Whether we loaded a file

    // Attempt to load a program from a path passed as a launch arg
    while( argIndex < argc && !fileLoaded ) {
        char *programFile = argv[ argIndex ];
        printf( "Attempting to load program: %s\n", programFile );

        if( loadFile( programFile ) == 0 ) { // Load the file into main memory
            fileLoaded = true;
        }
        argIndex++;
    }

    if( !fileLoaded ){
        printf( "ERROR: No valid program found in launch arguments. Unable to continue. Terminating....\n" );
        return -1;
    }

    PC = 0; // Reset the program counter to the beginning of the program

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
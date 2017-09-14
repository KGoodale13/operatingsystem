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

    char *programFolder = argv[0]; // Get the path to our exe
    programFolder = dirname( dirname( programFolder ) ); // Remove the exe from the path, and the bin folder (cd ../../)
    strcat( programFolder, "/programs/Program1" ); // Add the programs directory to the path (cd programs)
    printf( "Attempting to load program: %s\n", programFolder );

    if( loadFile( programFolder ) == -1 ) { // Load the file into main memory
        return -1; // We didn't load the file so kill it
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
            printf("Processing Opcode: %s\n", IR);
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
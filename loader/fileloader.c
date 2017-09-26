// Created by Kyle Goodale. See header for more info

#include <fcntl.h>
#include <printf.h>
#include <sys/errno.h>
#include <unistd.h>
#include <string.h>
#include "../kernel/memory.h"

// If we read a program that's too large we need to wipe memory before returning so we can try to load the next file
// without side effects
void recoverFromBadFileRead( ){
    memset( memory, 0, 100 * sizeof( memory[0] ) );
    PC = 0;
}

int loadFile( char *filePath ) {
    printf( "Loading program:  %s\n", filePath );

    char input_buffer[ 7 ]; // Create the input buffer for reading lines of the program file
    int fileHandle  = open( filePath, O_RDONLY ); // Open the file

    if( fileHandle == -1 ) {
        printf("Error: Failed to open file. Error: %i\n", errno);
        return -1;
    }
    // Read it into memory
    ssize_t readReturn;
    while( ( readReturn = read( fileHandle, input_buffer, 7 ) ) > 0   ) {
        input_buffer[ strcspn( input_buffer, "\r\n" ) ] = 0; // Pesky new line chars at EOF. Source: https://stackoverflow.com/questions/2693776/

        if( PC >= 99 ){
            printf("Error: Memory overflowed while reading file into memory. Discarding file and wiping main memory... \n");
            recoverFromBadFileRead();
            return -1;
        }

        strcpy( memory[ PC ], input_buffer ); // Load this line of the program into memory
        PC++;
    }

    close( fileHandle ); // close our file

    if( readReturn == 0 ) {
        return 0;
    } else {
        printf("Error: An error occurred while reading the program file. Error: %i\n ", errno);
        return -1;
    }
}


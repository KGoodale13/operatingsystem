// Created by Kyle Goodale. See header for details

#include <fcntl.h>
#include <printf.h>
#include <sys/errno.h>
#include <unistd.h>
#include <string.h>
#include "../kernel/executionContext.h"
#include "../kernel/ProcessControlBlock.h"

int loadFile( char *filePath ) {
    printf( "Loading program:  %s into memory owned by pid %i\n", filePath, getPID() );

    char input_buffer[ 7 ]; // Create the input buffer for reading lines of the program file
    int fileHandle  = open( filePath, O_RDONLY ); // Open the file

    if( fileHandle == -1 ) {
        printf("Error: Failed to open file. Error: %i\n", errno);
        return -1;
    }

    int currentLine = 0;

    // Read it into memory
    ssize_t readReturn;
    while( ( readReturn = read( fileHandle, input_buffer, 7 ) ) > 0   ) {
        input_buffer[ strcspn( input_buffer, "\r\n" ) ] = 0; // Pesky new line chars at EOF. Source: https://stackoverflow.com/questions/2693776/

        if( currentLine >= MAX_MEM_PER_PROC ){
            printf("Error: Memory overflowed while reading file into memory. Discarding file and wiping main memory... \n");
            return -1;
        }

        writeLineToMemory( currentLine, input_buffer );
        currentLine++;
    }

    close( fileHandle ); // close our file

    if( readReturn == 0 ) {
        return 0;
    } else {
        printf("Error: An error occurred while reading the program file. Error: %i\n ", errno);
        return -1;
    }
}


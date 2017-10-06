// Created by Kyle Goodale. See header for details
#include <printf.h>
#include <string.h>
#include "memory.h"
#include "../util/util.h"


static char memory[ MEMORY_LINES ][ 6 ]; // Our operating systems main memory. Shared by all programs


// Pretty prints the current state of memory to console
void _debug_dumpMemory( ) {

    printf( "Main memory dump: \n " );
    for ( int line = 0; line < MEMORY_LINES; ++line ) {
        printf("|%.2i|%.6s|\n ", line, memory[ line ] );
    }
}


// Writes a number to a memory location in the format ZZXXXX where X:{0-9}
int _memoryWriteValue( int location, int value ){
    if( location < 0 || location > MEMORY_LINES ) {
        printf("Error: Attempted to write value to invalid memory location. Expected (0-%i), Got: %i\n", MEMORY_LINES, location);
        return -1;
    }

    if( value < 0 || value > 9999 ){
        printf("Error: Attempted to write invalid value to memory. Expected (0-9999), Got: %i\n", value);
        return -1;
    }

    // Break the number into 4 numbers i.e 1234 -> 1 2 3 4. So we can write it to memory
    int thousands = value / 1000;
    value -= thousands * 1000;
    int hundreds = value / 100;
    value -= hundreds * 100;
    int tens = value / 10;
    int ones = value - ( tens * 10);

    // Z padding is for when storing to memory we pad the first two indexes with ZZ i.e ZZ1234 vs setting the accumulator which has no padding

    char newValue[ 6 ] = { 'Z', 'Z', (char)('0' + thousands), (char)('0' + hundreds), (char)('0' + tens), (char)('0' + ones) };
    memcpy( memory[ location ], newValue, 6 );
    return 0;
}

// Reads a number from memory that is in the format ZZXXXX where X:{0-9}
int _memoryReadValue( int location ) {
    if( location < 0 || location > MEMORY_LINES ) {
        printf("Error: Attempted to read value from invalid memory location. Expected (0-%i), Got: %i\n", MEMORY_LINES, location);
        return -1;
    }
    if( strncmp( &memory[ location ][0], "Z", 1 ) != 0 ) {
        printf("Error: Attempted to read value from memory location %i but opcode at location was not 'ZZ'\n", location);
        return -1;
    }
    // Grabs the numbers stored in indexes 2-4 as one single number instead of 4 separate numbers
    return parseArray( memory[ location ], 2, 4 );
}

// Writes a character array directly into memory.
int _memoryWriteLine( int location, char *line ){
    if( location < 0 || location >= MEMORY_LINES ){
        printf( "Attempting to write line to invalid memory location. Expected(0-%i) Got: %i\n", MEMORY_LINES, location );
        return -1;
    }
    strcpy( memory[ location ], line );
    return 0;
}

// Reads a line directly from the memory table, doesn't format or process it at all
char *_memoryReadLine( int location ){
    return memory[location];
}


// Clears 'lines' number of lines starting from 'fromLocation'
int _memoryClearBlock( int fromLocation, int lines ){
    memset( memory, fromLocation, lines * sizeof( memory[0] ) );
}


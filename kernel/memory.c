// Created by Kyle Goodale. See header for details
#include <math.h>
#include <printf.h>
#include <string.h>

char memory[ 100 ][ 6 ];
short int PC = 0;
short int P0, P1, P2, P3 = 0;
int R0, R1, R2, R3, ACC = 0;
char PSW[2], SP[2], IR[6];


// Pretty prints the current state of memory to console
void dumpMemory( ) {

    printf( "Main memory dump: \n " );
    for (int line = 0; line < 100; ++line ) {
        printf("|%.2i|%.6s|\n ", line, memory[line] );
    }
}

// Converts multiple consecutive indexes in an array to a single value.
int parseArray( const char arr[], int start, int len ) {
    int value = 0;

    for( int i=start; i < start + len; ++i ) {
        value += ( arr[ i ] - 48 ) * (int) pow( 10, len - ( i-start ) - 1 );
    }
    return value;
}

// Retrieves the memory location stored at a pointer
int getPointer( int index ){
     switch( index ){
         case 0: return P0;
         case 1: return P1;
         case 2: return P2;
         case 3: return P3;
         default:
             printf("Error: Attempted to access invalid pointer using getPointer. Expected (0-3), Got: %i\n", index);
             return -1;
     }
}

// Store a memory location in a pointer
int setPointer( int index, int value ){
    if( value < 0 || value > 99 ){
        printf("Error: Attempted to write to invalid pointer value. Valid locations are 0-99, Got: %i\n", value);
        return -1;
    }

    switch( index ){
        case 0: P0 = (short int) value; break;
        case 1: P1 = (short int) value; break;
        case 2: P2 = (short int) value; break;
        case 3: P3 = (short int) value; break;
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
        case 0: return R0;
        case 1: return R1;
        case 2: return R2;
        case 3: return R3;
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
        case 0: R0 = (short int) value; break;
        case 1: R1 = (short int) value; break;
        case 2: R2 = (short int) value; break;
        case 3: R3 = (short int) value; break;
        default:
            printf("Error: Attempted to access invalid pointer using getPointer. Expected (0-3), Got: %i\n", index);
            return -1;
    }
    return 0;
}

// Writes a number to a memory location in the format ZZXXXX where X:{0-9}
int writeValueToMemory( int location, int value ){
    if( location < 0 || location > 99 ){
        printf("Error: Attempted to write value to invalid memory location. Expected (0-99), Got: %i\n", location);
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
    memcpy( memory[ location ], newValue , 6 );
    return 0;
}

// Reads a number from memory that is in the format ZZXXXX where X:{0-9}
int readValueFromMemory( int location ) {
    if( location < 0 || location > 99 ) {
        printf("Error: Attempted to read value from invalid memory location. Expected (0-99), Got: %i\n", location);
        return -1;
    }
    if( strncmp( &memory[ location ][0], "Z", 1 ) != 0 ) {
        printf("Error: Attempted to read value from memory location %i but opcode at location was not 'ZZ'\n", location);
        return -1;
    }
    // Grabs the numbers stored in indexes 2-4 as one single number instead of 4 separate numbers
    return parseArray( memory[ location ], 2, 4 );
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
    ACC = value;
    return 0;
}

// Adds the value passed to the accumulator
int addAccumulator( int amount ){
    return setAccumulator( ACC + amount );
}

// subtracts the value passed from the accumulator
int subtractAccumulator( int amount ){
    return setAccumulator( ACC - amount );
}


int getAccumulator( ){
    return ACC;
}

// Sets PSW[0] only for now
int setPSW( int bool ){
    if( bool != 1 && bool != 0 ){
        printf("Error: Invalid value passed to setPSW. Expected 1 or 0, Got: %i\n", bool);
        return -1;
    }
    return 0;
}

// Returns PSW[0] only for now
int getPSW(){
    return parseArray( PSW, 0, 1 );
}

// Sets the program counter to the line passed
int branch( short int line ){
    if( line < 0 || line > 99 ){
        printf("Error: Invalid line passed to branch. Expected 0-99, Got: %i\n", line);
        return -1;
    }
    PC = line;
    return 0;
}
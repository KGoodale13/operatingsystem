//
// Created by Kyle Goodale on 8/31/17.
//

#include <printf.h>
#include <string.h>
#include "memory.h"

// Enums for identifying operands
const short int OPERAND_INVALID = -1; // Any operand that doesn't match any of the ones bellow.
const short int OPERAND_POINTER = 0; // P0, P1, P2, P3
const short int OPERAND_REGISTER = 1; // R0, R1, R2, R3
const short int OPERAND_NUMBER = 2; // XX
const short int OPERAND_NULL = 3; // ZZ


// Enums for acc comparisons
const short int COMPARISON_EQUALS = 1;
const short int COMPARISON_GREATER = 2;
const short int COMPARISON_LESS = 3;


/**
 * Compares the accumulators value to the value passed based on the comparison type enum passed
 * @param value - The value to compare
 * @return 0 -> False, 1 -> True, -1 -> Error
 */
int accCompare( int value, short int comparisonType ){
    int accValue = getAccumulator();

    switch( comparisonType ){
        case COMPARISON_EQUALS: return accValue == value;
        case COMPARISON_GREATER: return accValue > value;
        case COMPARISON_LESS: return accValue < value;
        default:
            printf("Error: Invalid comparison enum passed to accCompare. Expected 1,2 or 3. Got: %i\n", comparisonType );
            return -1;
    }
}

// Gets the second operand as a number only works if both values are numbers. (_ _ _ _ X X)
int getOperand2( ) { return parseArray(IR, 4, 2 ); }
// Gets the first operand as a number only works if both values are numbers. (_ _ X X _ _)
int getOperand1( ) { return parseArray(IR, 2, 2 ); }
// Gets both operands as a single number instead of two separate ones. (_ _ X X X X)
int getOperandsAsSingle() { return parseArray(IR, 2, 4); }
// Gets the first two digits ( the Op code ) ( X X _ _ _ _ )
int getOpcode( ) { return parseArray(IR,  0, 2 ); }

// Converts the passed char to an operand type enum. Helper function for getOperandType()
short int charToOperandType( char c ){
    if( strncmp( &c, "P", 1 ) == 0 ){ return OPERAND_POINTER; }
    else if( strncmp( &c, "R", 1 ) == 0 ){ return OPERAND_REGISTER; }
    else if( strncmp( &c, "Z", 1 ) == 0 ){ return OPERAND_NULL; }
    else if( c > 47 && c < 58 ){ return OPERAND_NUMBER; }
    else {
        printf("Error: Invalid operand type, unable to identify. Operand: %c\n", c);
        return OPERAND_INVALID;
    }
}

// Used for pointer and register id's So if the opcode is 00P143 and you requested the first operand index it would return 1 because P1
int getOperandIndex( int index ){
    return IR[ (index*2) + 1 ] - 48;
}

/**
 * @param operandIndex - The index of the operand you want to type check. 1 or 2
 * @return The enum associated with the operand type
 */
short int getOperandType( int operandIndex ){

    if( operandIndex > 6 || operandIndex < 2 ){
        printf("ERROR: Invalid operand index passed to getOperandType. Expected 1 - 4 got: %i\n", operandIndex );
        return -1;
    }
    return charToOperandType( IR[ operandIndex  ] );
}


// Verifies the operand at that index is the correct type.
// Prints an error and returns -1 if not. Else returns 0
int checkOperandType( int index, short int expectedTypeEnum ) {
    index = index * 2;
    short int operandType = getOperandType( index );
    short int operandValue = getOperandType( index + 1 );
    // Check the operand type i.e the 'P' in 'P0'
    if( operandType != expectedTypeEnum ){
        printf( "Error: Invalid operand %i. Expected %i, Got %i\n", index, expectedTypeEnum, operandType );
        return -1;
    }

    // If its a register or pointer the value should be {0-9}
    if( expectedTypeEnum == OPERAND_POINTER || expectedTypeEnum == OPERAND_REGISTER ) {
        if( operandValue != OPERAND_NUMBER  ) {
            printf( "Error: Invalid value for operand %i. Expected %i, Got %i\n", index, expectedTypeEnum, operandType );
            return -1;
        }
    } else if( operandValue != expectedTypeEnum ) { // Nulls and numbers come in pairs so ZZ or XX
        printf( "Error: Invalid value for operand %i. Expected %i, Got %i\n", index, expectedTypeEnum, operandType );
        return -1;
    }

    return 0;
}


int OP0( ){
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           setPointer( getOperandIndex(1), getOperand2() ) : -1;
}

int OP1( ){
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           adjustPointer( getOperandIndex(1), getOperand2() ) : -1;
}

int OP2( ){
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           adjustPointer( getOperandIndex(1), getOperand2() * -1 ) : -1;
}

int OP3( ){
    return ( checkOperandType( 1, OPERAND_NUMBER ) + checkOperandType( 2, OPERAND_NUMBER ) == 0 ) ?
            setAccumulator( getOperandsAsSingle() ) : -1;
}

int OP4( ){
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           setAccumulator( getPointerMemoryValue( getOperandIndex(1) ) ) : -1;
}

int OP5( ){
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           setAccumulator( readValueFromMemory( getOperand1() ) ) : -1;
}

int OP6( ){
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           writeValueToMemory( getPointer( getOperandIndex( 1 ) ), getAccumulator() ) : -1;
}

int OP7( ){
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ? writeValueToMemory( getOperand1(), getAccumulator() ) : -1;
}

int OP8( ){
    return ( checkOperandType( 1, OPERAND_REGISTER ) + checkOperandType( 2, OPERAND_POINTER ) == 0 ) ?
           writeValueToMemory( getPointer( getOperandIndex( 1 ) ), getRegister( getOperandIndex( 2 ) ) ): -1;
}

int OP9( ){
    return ( checkOperandType( 1, OPERAND_REGISTER ) + checkOperandType( 2, OPERAND_NUMBER ) == 0 ) ?
           writeValueToMemory( getOperand2(), getRegister( getOperandIndex(1) ) ) : -1;
}

int OP10() {
    return ( checkOperandType( 1, OPERAND_REGISTER ) + checkOperandType( 2, OPERAND_POINTER ) == 0 ) ?
           setRegister( getOperandIndex(1), getPointerMemoryValue( getOperandIndex(2) ) ) : -1;
}

int OP11() {
    return ( checkOperandType( 1, OPERAND_REGISTER ) + checkOperandType( 2, OPERAND_POINTER ) == 0 ) ?
           setRegister( getOperandIndex(1), readValueFromMemory(  getOperand2() ) ) : -1;
}

int OP12() {
    return ( checkOperandType( 1, OPERAND_NUMBER ) + checkOperandType( 2, OPERAND_NUMBER ) == 0 ) ?
           setRegister( 0, getOperandsAsSingle() ) : -1;
}

int OP13() {
    return ( checkOperandType( 1, OPERAND_REGISTER ) + checkOperandType( 2, OPERAND_REGISTER ) == 0 ) ?
           setRegister( getOperandIndex(1), getRegister( getOperandIndex(2) ) ) : -1;
}

int OP14() {
    return checkOperandType( 1, OPERAND_REGISTER ) == 0 ?
           setAccumulator( getRegister( getOperandIndex(1) ) ) : -1;
}

int OP15() {
    return checkOperandType( 1, OPERAND_REGISTER ) == 0 ?
           setRegister( getOperandIndex(1), getAccumulator() ) : -1;
}

int OP16() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           addAccumulator( getOperandsAsSingle() ) : -1;
}

int OP17() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           subtractAccumulator( getOperandsAsSingle() ) : -1;
}

int OP18() {
    return checkOperandType( 1, OPERAND_REGISTER ) == 0 ?
           addAccumulator( getRegister( getOperandIndex(1) ) ) : -1;
}

int OP19() {
    return checkOperandType( 1, OPERAND_REGISTER ) == 0 ?
           subtractAccumulator( getRegister( getOperandIndex(1) ) ) : -1;
}

int OP20() {
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           addAccumulator( getPointerMemoryValue( getOperandIndex(1) ) )  : -1;
}

int OP21() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           addAccumulator( readValueFromMemory( getOperand1() ) )  : -1;
}

int OP22() {
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           subtractAccumulator( getPointerMemoryValue( getOperandIndex(1) ) )  : -1;
}

int OP23() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           subtractAccumulator( readValueFromMemory( getOperand1() ) )  : -1;
}

int OP24() {
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           setPSW( accCompare( getPointerMemoryValue( getOperandIndex(1) ), COMPARISON_EQUALS ) ) : -1;
}

int OP25() {
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           setPSW( accCompare( getPointerMemoryValue( getOperandIndex(1) ), COMPARISON_LESS ) ) : -1;
}

int OP26() {
    return checkOperandType( 1, OPERAND_POINTER ) == 0 ?
           setPSW( accCompare( getPointerMemoryValue( getOperandIndex(1) ), COMPARISON_LESS ) ) : -1;
}

int OP27() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           setPSW( accCompare( getOperandsAsSingle(), COMPARISON_GREATER ) ) : -1;
}

int OP28() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           setPSW( accCompare( getOperandsAsSingle(), COMPARISON_EQUALS ) ) : -1;
}

int OP29() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           setPSW( accCompare( getOperandsAsSingle(), COMPARISON_LESS ) ) : -1;
}

int OP30() {
    return checkOperandType( 1, OPERAND_REGISTER ) == 0 ?
           setPSW( accCompare( getRegister(getOperandIndex(1)), COMPARISON_EQUALS ) ) : -1;
}

int OP31() {
    return checkOperandType( 1, OPERAND_REGISTER ) == 0 ?
           setPSW( accCompare( getRegister(getOperandIndex(1)), COMPARISON_LESS ) ) : -1;
}

int OP32() {
    return checkOperandType( 1, OPERAND_REGISTER ) == 0 ?
           setPSW( accCompare( getRegister(getOperandIndex(1)), COMPARISON_GREATER ) ) : -1;
}

int OP33() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           ( getPSW() == 1 ? branch( getOperand1() ) : 0 ) : -1;
}

int OP34() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           branch( getOperand1() ) : -1;
}

int OP35() {
    return checkOperandType( 1, OPERAND_NUMBER ) == 0 ?
           ( getPSW() == 1 ? branch( getOperand1() ) : 0 ) : -1;
}



int processOpcode( ) {

    switch ( getOpcode() ) {
        case 0: return OP0( );
        case 1: return OP1( );
        case 2: return OP2( );
        case 3: return OP3( );
        case 4: return OP4( );
        case 5: return OP5( );
        case 6: return OP6( );
        case 7: return OP7( );
        case 8: return OP8( );
        case 9: return OP9( );
        case 10: return OP10( );
        case 11: return OP11( );
        case 12: return OP12( );
        case 13: return OP13( );
        case 14: return OP14( );
        case 15: return OP15( );
        case 16: return OP16( );
        case 17: return OP17( );
        case 18: return OP18( );
        case 19: return OP19( );
        case 20: return OP20( );
        case 21: return OP21( );
        case 22: return OP22( );
        case 23: return OP23( );
        case 24: return OP24( );
        case 25: return OP25( );
        case 26: return OP26( );
        case 27: return OP27( );
        case 28: return OP28( );
        case 29: return OP29( );
        case 30: return OP30( );
        case 31: return OP31( );
        case 32: return OP32( );
        case 33: return OP33( );
        case 34: return OP34( );
        case 35: return OP35( );
        case 99: return 1;
        default:
            printf("Error: Unknown opcode %i program terminated.\n", getOpcode() );
            return -1;

    }
}


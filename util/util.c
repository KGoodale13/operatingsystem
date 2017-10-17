//
// Created by Kyle Goodale on 10/3/17.
//

#include <math.h>

// Converts multiple consecutive indexes in an array to a single value.
int parseArray( const char arr[], int start, int len ) {
    int value = 0;

    for( int i=start; i < start + len; ++i ) {
        value += ( arr[ i ] - 48 ) * (int) pow( 10, len - ( i-start ) - 1 );
    }
    return value;
}



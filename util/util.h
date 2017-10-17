//
// Created by Kyle Goodale on 10/3/17.
//

#ifndef OPERATINGSYSTEM_UTIL_H
#define OPERATINGSYSTEM_UTIL_H


/**
 * Coverts however many values passed as len starting at start to one number.
 * @param arr - The array to parse values from
 * @param start - The starting index
 * @param len  - How many indexes to include
 * @return An int representing the values at those indexes i.e [3,4] = 34
 */
int parseArray( const char arr[], int start, int len );

#endif //OPERATINGSYSTEM_UTIL_H

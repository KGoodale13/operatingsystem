/**
 * Implements all the opcodes
 * calling processOpcode( ) starts executing the program starting at the program line in memory
 * the Program Counter references. If an exception occurs processOpcode will throw -1
 * Kind of a hacky exception handling implementation but it points our errors in our PBRAIN programs so its good enough
 *
 * @author: Kyle Goodale
 * @date: 09/06/2017
 */


#ifndef OPERATINGSYSTEM_OPCODES_H
#define OPERATINGSYSTEM_OPCODES_H


/**
 * Reads the current program line from memory using the PC ( program counter ) and parses and handles the opcode
 * found on that line.
 * @return -1 if the opcode is invalid, 0 on success and 1 if it reaches the halt opcode
 */
int processOpcode( );

#endif //OPERATINGSYSTEM_OPCODES_H
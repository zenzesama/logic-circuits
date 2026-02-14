/**
 * @file gates.c
 * @author zenze (zenzesama@gmail.com)
 * @brief this file contaisn the implementation of all logic gates
 * @version 0.1
 * @date 2026-02-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logic.h"

/**
 * @brief this is a simple "and gate" using a truth table
 * 
 * @param a - the first bit
 * @param b - the second bit
 * @return bit - the resultant bit
 */
bit andGate(bit a, bit b){
    static const bit tt[2][2] = {
        {LOW, LOW},
        {LOW, HIGH}
    };
    return tt[a][b];
}

/**
 * @brief this is a simple "or gate" using a truth table
 * 
 * @param a - the first bit
 * @param b - the second bit
 * @return bit - the resultant bit
 */
bit orGate(bit a, bit b){
    static const bit tt[2][2] = {
        {LOW, HIGH},
        {HIGH, HIGH}
    };
    return tt[a][b];
}

/**
 * @brief this is a simple "not gate" using a truth table
 * 
 * @param a - the first bit
 * @param b - the second bit
 * @return bit - the resultant bit
 */
bit notGate(bit a){
    static const bit tt[2] = {HIGH, LOW};
    return tt[a];
}

/**
 * @brief this is a simple "nand gate" using the previous gates
 * 
 * @param a - the first bit
 * @param b - the second bit
 * @return bit - the resultant bit
 */
bit nandGate(bit a, bit b){
    return notGate(andGate(a, b));
}

/**
 * @brief this is a simple "nor gate" using the previous gates
 * 
 * @param a - the first bit
 * @param b - the second bit
 * @return bit - the resultant bit
 */
bit norGate(bit a, bit b){
    return notGate(orGate(a, b));
}

/**
 * @brief this is a simple "xor gate" using the previous gates
 * 
 * @param a - the first bit
 * @param b - the second bit
 * @return bit - the resultant bit
 */
bit xorGate(bit a, bit b){
    return andGate(orGate(a, b), nandGate(a, b));
}
/**
 * @file hacks.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief this file will contain all the small hacks used throughout other files
 * @version 0.1
 * @date 2026-03-16
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logic.h"

/**
 * @brief helper function that directly maps hex characters to their values
 * 
 * @param c this is the enteres hex character
 * @return unsigned char this is the returned value
 */
static unsigned char hexVal(char c){
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'A' && c <= 'F') return c - 'A' + 10;
    if(c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

/**
 * @brief this converts a given hex number to a byte
 * 
 * @param hex this is the given hex number, like "0x2B"
 * @return byte this is the returned byte, like {0,0,1,0,1,0,1,1}
 */
byte hexToByte(char *hex){
    byte result;
    hex += 2;
    unsigned char value = (hexVal(hex[0]) << 4) | hexVal(hex[1]);

    for(int i = 0; i < 8; i++){
        result.bits[i] = (value >> (7 - i)) & 1;
    }

    return result;
}
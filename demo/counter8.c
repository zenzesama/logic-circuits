/**
 * @file counter.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief demonstrates the 8-bit counter
 * @version 0.1
 * @date 2026-03-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <stdio.h>
#include "logic.h"

int main(void){
    printf("8-Bit Counter Simulation\n");
    printf("========================\n\n");
    
    // create a new counter
    eight_bit_counter counter = newEightBitCounter();
    
    printf("Counting from 0 to 20:\n");
    printf("Tick | Binary   | Decimal\n");
    printf("-----|----------|--------\n");
    
    /* count from 0 to 20 */
    for(int tick = 0; tick <= 20; tick++){
        byte current_value = getEightBitCounterValue(&counter);
        int decimal = byteToDecimal(current_value);
        
        printf("%4d | ", tick);
        for (int i = 0; i < 8; i++){
            printf("%d", current_value.bits[i]);
        }
        printf(" | %3d\n", decimal);
        
        /* tick the counter to increment it */
        tickEightBitCounter(&counter);
    }
    
    printf("\n\nDemonstrating counter overflow (250-260):\n");
    printf("Tick | Binary   | Decimal\n");
    printf("-----|----------|--------\n");
    
    /* reset and count to 250 */
    resetEightBitCounter(&counter);
    for(int i = 0; i < 250; i++){
        tickEightBitCounter(&counter);
    }
    
    /* show the overflow from 255 to 0 */
    for(int tick = 250; tick <= 260; tick++){
        byte current_value = getEightBitCounterValue(&counter);
        int decimal = byteToDecimal(current_value);
        
        printf("%4d | ", tick);
        for (int i = 0; i < 8; i++){
            printf("%d", current_value.bits[i]);
        }
        printf(" | %3d", decimal);
        
        if(decimal == 255){
            printf(" <- Maximum value");
        } else if(tick == 256){
            printf(" <- Overflow! Back to 0");
        }
        printf("\n");
        
        tickEightBitCounter(&counter);
    }
    
    printf("\n\nDemonstrating reset:\n");
    resetEightBitCounter(&counter);
    byte reset_value = getEightBitCounterValue(&counter);
    printf("After reset: ");
    for (int i = 0; i < 8; i++){
        printf("%d", reset_value.bits[i]);
    }
    printf(" (Decimal: %d)\n", byteToDecimal(reset_value));
    
    return 0;
}
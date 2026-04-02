/**
 * @file ram_demo.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief demonstrates the 8x8 RAM
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include "logic.h"

int main(void){
    printf("8x8 RAM Demonstration\n");
    printf("=====================\n\n");

    ram8x8 ram = newRam8x8();

    char *addrs[8]    = { "0x00", "0x01", "0x02", "0x03",
                          "0x04", "0x05", "0x06", "0x07" };
    char *data_hex[8] = { "0x01", "0x2A", "0x55", "0x64",
                          "0x80", "0xAA", "0xCC", "0xFF" };

    /* write phase */
    printf("Writing to all 8 addresses:\n");
    printf("Addr  | Data | Decimal\n");
    printf("------|------|--------\n");

    for(int i = 0; i < 8; i++){
        byte data = hexToByte(data_hex[i]);
        writeRam8x8(&ram, addrs[i], data);

        char data_str[5];
        byteToHex(data, data_str);
        printf("%s  | %s |  %3d\n", addrs[i], data_str, byteToDecimal(data));
    }

    /* read phase */
    printf("\nReading back all 8 addresses:\n");
    printf("Addr  | Data | Decimal | Status\n");
    printf("------|------|---------|-------\n");

    for(int i = 0; i < 8; i++){
        byte expected = hexToByte(data_hex[i]);
        byte result   = readRam8x8(&ram, addrs[i]);

        char result_str[5];
        byteToHex(result, result_str);
        printf("%s  | %s |  %3d    | %s\n",
            addrs[i], result_str, byteToDecimal(result),
            byteToDecimal(result) == byteToDecimal(expected) ? "OK" : "MISMATCH"
        );
    }

    return 0;
}
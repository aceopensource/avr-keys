/**
 * AVR FLASH METHODS
 * Christopher Bero <berocs@acedb.co>
 */

#ifndef FLASH_H
#define FLASH_H 1

#include <avr/power.h>
#include <stdlib.h>
//#include "boot.h"
#include "avr-printf.h"

typedef uint16_t pagelen_t ;

// Flash function prototypes
void flash_get_byte(uint16_t addr, uint8_t * byte);
void flash_get_bytes(uint16_t addr, uint8_t * buf, uint8_t len);
void flash_put_byte(uint16_t addr, uint8_t byte);
void flash_put_bytes(uint16_t address, uint8_t * buf, uint8_t len);
void flash_clear_byte(uint16_t addr);
uint8_t flash_clear_bytes(uint16_t addr, const uint16_t len);

//void print_area();
//uint8_t flash_read_key(uint8_t key_index, uint8_t * buffer);


#endif // FLASH_H

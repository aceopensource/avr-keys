/**
 * AVR KEY METHODS
 * Christopher Bero <berocs@acedb.co>
 */

#ifndef KEY_H
#define KEY_H 1

#include <avr/pgmspace.h>
#include "eeprom.h"
#include "flash.h"

// Debugging
#define KEY_DEBUG 0

#define KEY_MAX_LEN 10

#define FLASH_DATA_SIZE (1000*KEY_MAX_LEN)
extern const uint8_t flash_data[FLASH_DATA_SIZE] PROGMEM;
#define FLASH_DATA_ADDRESS ((uint16_t)&flash_data)

#define EEPROM_ADDR_START ((uint8_t*)(E2END-(50*KEY_MAX_LEN)))
#define EEPROM_ADDR_END ((uint8_t*)E2END)

#define FLASH_ADDR_START (FLASH_DATA_ADDRESS)
#define FLASH_ADDR_END (FLASH_DATA_ADDRESS+FLASH_DATA_SIZE-1)

// Key function prototypes
void flash_format();
void eeprom_format();
uint16_t key_max_index();
uint8_t key_check(uint8_t * buf, uint16_t * index_out);
uint8_t key_get(uint16_t index, uint8_t * buf_out);
uint8_t key_get_byte(uint16_t key_index, uint8_t byte_index, uint8_t * byte_out);
uint8_t key_full();
uint8_t key_add(uint8_t * buf);
uint8_t key_remove(uint16_t index);



#endif // KEY_H

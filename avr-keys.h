/**
 * AVR KEY METHODS
 * Christopher Bero <berocs@acedb.co>
 */

#ifndef AVR_KEYS_H
#define AVR_KEYS_H

#include <avr/pgmspace.h>
#include <stdint.h>
#include "eeprom.h"
#include "flash.h"

#ifndef KEY_DEBUG
#define KEY_DEBUG 0
#endif // KEY_DEBUG

#ifndef KEY_MAX_LEN
#define KEY_MAX_LEN 10
#endif // KEY_MAX_LEN

#ifndef FLASH_NUM_KEYS
#define FLASH_NUM_KEYS 10
#endif // FLASH_NUM_KEYS

#ifndef EEPROM_NUM_KEYS
#define EEPROM_NUM_KEYS 10
#endif // EEPROM_NUM_KEYS

#ifndef NUM_KEYS
#define NUM_KEYS (FLASH_NUM_KEYS+EEPROM_NUM_KEYS)
#endif // NUM_KEYS

#ifndef FLASH_DATA_SIZE
#define FLASH_DATA_SIZE (FLASH_NUM_KEYS*KEY_MAX_LEN)
#endif // FLASH_DATA_SIZE

#ifndef EEPROM_DATA_SIZE
#define EEPROM_DATA_SIZE (EEPROM_NUM_KEYS*KEY_MAX_LEN)
#endif // EEPROM_DATA_SIZE

//#ifndef FLASH_DATA_EXTERN
extern const uint8_t flash_data[FLASH_DATA_SIZE] PROGMEM;
//#endif // FLASH_DATA_EXTERN

#ifndef FLASH_DATA_ADDRESS
#define FLASH_DATA_ADDRESS ((uint16_t)&flash_data)
#endif // FLASH_DATA_ADDRESS

#ifndef EEPROM_ADDR_START
#define EEPROM_ADDR_START ((uint8_t*)(E2END-(EEPROM_DATA_SIZE*KEY_MAX_LEN)))
#endif // EEPROM_ADDR_START

#ifndef EEPROM_ADDR_END
#define EEPROM_ADDR_END ((uint8_t*)E2END)
#endif // EEPROM_ADDR_END

#ifndef FLASH_ADDR_START
#define FLASH_ADDR_START (FLASH_DATA_ADDRESS)
#endif // FLASH_ADDR_START

#ifndef FLASH_ADDR_END
#define FLASH_ADDR_END (FLASH_DATA_ADDRESS+FLASH_DATA_SIZE-1)
#endif // FLASH_ADDR_END

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif // AVR_KEYS_H

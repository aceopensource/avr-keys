/**
 * AVR EEPROM METHODS
 * Christopher Bero <berocs@acedb.co>
 */

#ifndef EEPROM_H
#define EEPROM_H 1

#include <avr/eeprom.h>
#include "avr-printf.h"

// EEPROM function prototypes
uint8_t eeprom_get_byte(uint8_t * addr);
void eeprom_get_bytes(uint8_t * addr, uint8_t * buf, uint8_t len);
void eeprom_put_byte(uint8_t * addr, uint8_t byte);
void eeprom_put_bytes(uint8_t * addr, uint8_t * buf, uint8_t len);
void eeprom_clear_byte(uint8_t * addr);
void eeprom_clear_bytes(uint8_t * addr, uint16_t len);

//uint8_t eeprom_key_single_byte(uint8_t key, uint8_t index);
//void eeprom_key_remove(uint8_t * key, uint8_t keyLen);
//uint8_t * eeprom_key_add(uint8_t * key, uint8_t keyLen);
//uint8_t eeprom_full();
//uint8_t * eeprom_key_check(uint8_t* key, uint8_t keyLen);

//uint8_t eeprom_key_print(uint8_t * key, uint8_t keyLen);
//uint8_t eeprom_key_dump();
//void eeprom_byte_dump();

#endif // EEPROM_H

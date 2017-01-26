/**
 * AVR KEY METHODS
 * Christopher Bero <berocs@acedb.co>
 */

#include "avr-keys.h"

#ifndef FLASH_DATA_EXTERN
const uint8_t flash_data[FLASH_DATA_SIZE] PROGMEM __attribute__((used)) = {0x00};
#endif // FLASH_DATA_EXTERN

void flash_format()
{
//	for (uint16_t index = 0; index < FLASH_DATA_SIZE; index++)
//	{
//		flash_clear_byte(FLASH_ADDR_START+index);
//		printf("Clear %d\n", index);
//	}
	flash_clear_bytes(FLASH_ADDR_START, FLASH_DATA_SIZE);
}

// Formats the EEPROM key space to 0xff
void eeprom_format()
{
	eeprom_clear_bytes(EEPROM_ADDR_START, (EEPROM_ADDR_END-EEPROM_ADDR_START));
}

uint16_t key_max_index()
{
	return(((EEPROM_ADDR_END-EEPROM_ADDR_START)/KEY_MAX_LEN) + (FLASH_ADDR_END-FLASH_ADDR_START)/KEY_MAX_LEN);
}

uint8_t key_check(uint8_t * buf, uint16_t * index_out)
{
	uint8_t cmp_buf[KEY_MAX_LEN];
	uint8_t match_flag;
	// Start with eeprom
//	printf("key max index: %d\n", key_max_index());
	for (uint16_t index = 0; index <= key_max_index(); index++)
	{
		match_flag = 1;
		key_get(index, cmp_buf);
		for (uint8_t key_index = 0; key_index < KEY_MAX_LEN; key_index++)
		{
//			printf("Comparing [0x%02X] and [0x%02X]\n", cmp_buf[key_index], buf[key_index]);
			if (cmp_buf[key_index] != buf[key_index])
			{
				match_flag = 0;
				break;
			}
		}
		if (match_flag)
		{
			*index_out = index;
			return(0);
		}
	}
	return(1); // error, key not found
}

uint8_t key_get(uint16_t key_index, uint8_t * buf_out)
{
	uint8_t first_flash_index = (EEPROM_ADDR_END - EEPROM_ADDR_START)/KEY_MAX_LEN;
	if (key_index < first_flash_index)
	{
		// key is in eeprom
		if (KEY_DEBUG) printf("Key [%d] is in eeprom.\n", key_index);
		eeprom_get_bytes((uint8_t*)((key_index*KEY_MAX_LEN)+EEPROM_ADDR_START), buf_out, KEY_MAX_LEN);
		for (uint8_t index = 0; index < KEY_MAX_LEN; index++)
		{
			if (buf_out[index] == 0xff)
			{
				buf_out[index] = 0x00;
			}
		}
	}
	else if (key_index <= key_max_index())
	{
		// key is in flash
		if (KEY_DEBUG) printf("Key [%d] is in flash.\n", key_index);
		key_index -= first_flash_index;
//		flash_get_bytes(FLASH_ADDR_START + (key_index*KEY_MAX_LEN), buf_out, KEY_MAX_LEN);
		for (uint8_t index = 0; index < KEY_MAX_LEN; index++)
		{
			buf_out[index] = pgm_read_byte(&(flash_data[(key_index*KEY_MAX_LEN)+index]));
		}
	}
	else
	{
		// bad news
		return(1);
	}
	if (buf_out[0] == 0x00)
	{
		// Key is empty
		return(2);
	}
	return(0);
}

uint8_t key_get_byte(uint16_t key_index, uint8_t byte_index, uint8_t * byte_out)
{
	uint8_t first_flash_index = (EEPROM_ADDR_END - EEPROM_ADDR_START)/KEY_MAX_LEN;
	if (key_index < first_flash_index)
	{
		// key is in eeprom
		if (KEY_DEBUG) printf("Key [%d] is in eeprom.\n", key_index);
		*byte_out = eeprom_get_byte((uint8_t*)((key_index*KEY_MAX_LEN)+EEPROM_ADDR_START+byte_index));
		if (*byte_out == 0xff)
		{
			byte_out = 0x00;
		}
	}
	else if (key_index <= key_max_index())
	{
		// key is in flash
		if (KEY_DEBUG) printf("Key [%d] is in flash.\n", key_index);
		key_index -= first_flash_index;
//		flash_get_bytes(FLASH_ADDR_START + (key_index*KEY_MAX_LEN), buf_out, KEY_MAX_LEN);
		*byte_out = pgm_read_byte(&(flash_data[(key_index*KEY_MAX_LEN)+byte_index]));
	}
	else
	{
		// bad news
		return(1);
	}
	return(0);
}

uint8_t key_full()
{
	uint8_t empty_space[KEY_MAX_LEN] = {0};
	return(key_check(empty_space, NULL));
}

uint8_t key_add(uint8_t * buf)
{
	uint8_t first_flash_index = (EEPROM_ADDR_END - EEPROM_ADDR_START)/KEY_MAX_LEN;
	uint8_t empty_space[KEY_MAX_LEN] = {0};
	uint16_t key_index;
	uint8_t tmp;
	if (key_check(buf, &key_index) == 0)
	{
		// key already exists.
		printf("Key already exists.\n");
		return(2);
	}
	tmp = key_check(empty_space, &key_index);
	if (KEY_DEBUG) printf("key_add's key_check value: %d\n", tmp);
	if (tmp)
	{
		printf("No space left.\n");
		return(1); // no good
	}
	if (key_index < first_flash_index)
	{
		// store in eeprom
		eeprom_put_bytes(EEPROM_ADDR_START + (key_index*KEY_MAX_LEN), buf, KEY_MAX_LEN);
	}
	else if (key_index <= key_max_index())
	{
		// store in flash
		key_index -= first_flash_index;
		flash_put_bytes(FLASH_ADDR_START + (key_index*KEY_MAX_LEN), buf, KEY_MAX_LEN);
	}
	else
	{
		// uh oh
		printf("Index out of range.\n");
		return(1);
	}
	return(0);
}

uint8_t key_remove(uint16_t key_index)
{
	uint8_t top_eeprom_index = (EEPROM_ADDR_END - EEPROM_ADDR_START)/KEY_MAX_LEN;
	uint8_t empty_space[KEY_MAX_LEN+1] = {0};
	if (KEY_DEBUG) printf("Erasing key #%d\t", key_index);
	if (key_index < top_eeprom_index)
	{
		// clear from eeprom
		eeprom_put_bytes(EEPROM_ADDR_START + (key_index*KEY_MAX_LEN), empty_space, KEY_MAX_LEN);
//		eeprom_get_bytes(EEPROM_ADDR_START + (key_index*KEY_MAX_LEN), empty_space, KEY_MAX_LEN);
//		printf("New bytes in eeprom: %s\n", empty_space);
	}
	else if (key_index <= key_max_index())
	{
		// clear from flash
		key_index -= top_eeprom_index;
		flash_put_bytes(FLASH_ADDR_START + (key_index*KEY_MAX_LEN), empty_space, KEY_MAX_LEN);
//		flash_get_bytes(FLASH_ADDR_START + (key_index*KEY_MAX_LEN), empty_space, KEY_MAX_LEN);
//		for (uint8_t index = 0; index < KEY_MAX_LEN; index++)
//		{
//			empty_space[index] = pgm_read_byte(&(flash_data[(key_index*KEY_MAX_LEN)+index]));
//		}
//		printf("New bytes in flash: %s\n", empty_space);
	}
	else
	{
		// uh oh
		return(1);
	}
	return(0);
}

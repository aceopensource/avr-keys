/**
 * AVR FLASH METHODS
 * Christopher Bero <berocs@acedb.co>
 */

#include "flash.h"

// Function pointers to bootloader
void (*bootloader_func)(void) = (void*)0x7e00;
//void (*bootloader_putch)(char ch) = (void*)0x7f6a;
void (*bootloader_putch)(char ch) = (void*)0x3fb5;
uint8_t (*bootloader_read_mem)(uint8_t memtype, uint16_t address, pagelen_t length) = (void*)0x3fea;
void (*bootloader_writebuffer)(int8_t memtype, uint8_t *mybuff, uint16_t address, pagelen_t len) = (void*)0x3fba;
uint8_t (*bootloader_getch)(void) = (void*)0x7f78;

void flash_get_byte(uint16_t addr, uint8_t * byte)
{
//	UCSR0B |= (1 << TXEN0);
	power_usart0_disable();
	*byte = bootloader_read_mem(0, addr, 1);
	power_usart0_enable();
//	printf("Flash, got byte [0x%02X] data: 0x%02X\n", addr, *byte);
}

void flash_get_bytes(uint16_t addr, uint8_t * buf, uint8_t len)
{
//	printf("Flash address: 0x%02X, len: %d\n", addr, len);
	for (uint8_t index = 0; index < len; index++)
	{
		flash_get_byte(addr+index, &buf[index]);
	}
}

void flash_put_byte(uint16_t addr, uint8_t byte)
{
	flash_put_bytes(addr, &byte, 1);
}

void flash_put_bytes(uint16_t address, uint8_t * buf, uint8_t len)
{
	uint8_t tmp_page[SPM_PAGESIZE] = {0x00};
	uint16_t address_base;
	uint8_t offset;
	uint8_t current_index = 0;

	do
	{
		address_base = (address+current_index) & ~(SPM_PAGESIZE-1);
		offset = (address+current_index) - address_base;
//		printf("Offset: %d\n", offset);
		for (uint8_t index = 0 ; index < SPM_PAGESIZE; index++)
		{
			flash_get_byte(address_base+index, &tmp_page[index]);
		}
//		printf("Writing address 0x%02X to 0x%02X\n", address_base, address_base+SPM_PAGESIZE-1);
//		printf("Before modification:\n");
//		for (uint8_t index = 0 ; index < SPM_PAGESIZE; index++)
//		{
//			printf("[0x%02X] ", tmp_page[index]);
//			if (index % 8 == 7)
//			{
//				printf("\n");
//			}
//		}
		for (uint8_t index = 0; current_index < len && (index+offset) < SPM_PAGESIZE; current_index++, index++)
		{
			tmp_page[index+offset] = buf[current_index];
		}
//		printf("After modification\n");
//		for (uint8_t index = 0 ; index < SPM_PAGESIZE; index++)
//		{
//			printf("[0x%02X] ", tmp_page[index]);
//			if (index % 8 == 7)
//			{
//				printf("\n");
//			}
//		}
		bootloader_writebuffer('F', tmp_page, address_base, SPM_PAGESIZE);
//		printf("Page top: 0x%02X\n", (address_base+SPM_PAGESIZE-1));
//		printf("Address top: 0x%02X\n", (address+len-1));
	} while ((address_base+SPM_PAGESIZE-1) < (address+len-1));
}

void flash_clear_byte(uint16_t addr)
{
	flash_put_byte(addr, 0x00);
}

uint8_t flash_clear_bytes(uint16_t addr, uint16_t len)
{
	uint8_t * buf;
//	printf("Clearing %d bytes of flash.\n", len);
	if (len > SPM_PAGESIZE)
	{
//		printf("Larger than page size.\n");
//		uint8_t buf[SPM_PAGESIZE];
		if ((buf = malloc(SPM_PAGESIZE)) == NULL)
		{
			printf("Alloc failed.\n");
			return(1);
		}
		for (uint8_t index = 0; index < SPM_PAGESIZE; index++)
		{
			buf[index] = 0x00;
		}
		while (len > SPM_PAGESIZE)
		{
//			printf("Clearing a %d chunk at address 0x%02X, %d left.\n", SPM_PAGESIZE, addr, len);
			flash_put_bytes(addr, buf, SPM_PAGESIZE);
			addr += SPM_PAGESIZE;
			len -= SPM_PAGESIZE;
		}
	}
	else
	{
//		printf("Not larger than page size.\n");
//		uint8_t buf[len];
		if ((buf = malloc(len)) == NULL)
		{
			printf("Alloc failed.\n");
			return(1);
		}
		for (uint8_t index = 0; index < len; index++)
		{
			buf[index] = 0x00;
		}
	}
//	uint8_t buf[len];
//	for (uint16_t index = 0; index < len; index++)
//	{
//		buf[index] = 0x00;
//	}
	flash_put_bytes(addr, buf, len);
//	printf("Buf pointer value: %d\n", buf);
	free(buf);
	return(0);
}

//uint8_t flash_read_key(uint8_t key_index, uint8_t * buffer)
//{
//	printf("flash read key: %d\n", key_index);
//	for (uint8_t index = 0; index < 10; index++)
//	{
//		flash_get_byte((uint16_t)FLASH_DATA_ADDRESS + (key_index*10) + index, &buffer[index]);
//		printf("Address: 0x%02X, value: 0x%02X\n", (FLASH_DATA_ADDRESS + (key_index*10) + index), buffer[index]);
//	}
//	return(0);
//}

//void print_area()
//{
//	uint8_t buffer;
////	for (int index = 0; index < 8; index++)
////	{
////		for (int page_index = 0; page_index < 16; page_index++)
////		{
////			buffer = bootloader_read_mem(0, 0x5000+((index*64)+page_index), 1);
////			printf("[0x%02X, %c] ", buffer, buffer);
////		}
////		printf("\n");
////	}
//
//	printf("EEPROM:");
//    for (int addr = 0; (addr+7) <= FLASH_DATA_SIZE; addr+=8)
//    {
////		printf("\n[0x%03X] 0x%03X", (uint16_t)addr, eeprom_read_byte(addr));
////		if (((uint16_t)addr%100) == 0)
////		{
////			wdt_reset();
////		}
//        printf("\nAddr [0x%03X-0x%03X]\t", (uint16_t)(FLASH_DATA_ADDRESS+addr), (uint16_t)(FLASH_DATA_ADDRESS+addr+7));
//        for (int subaddr = 0; subaddr < 8; subaddr++)
//        {
////        	buffer = bootloader_read_mem(0, (0x5000+addr+subaddr), 1);
//        	flash_get_byte((uint16_t)(FLASH_DATA_ADDRESS+addr+subaddr), &buffer);
//			printf("[0x%02X, %c] ", buffer, buffer);
////            printf("[0x%02X] ", eeprom_read_byte(addr+subaddr));
//        }
//    }
//    printf("\n");
//}

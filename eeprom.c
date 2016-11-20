/**
 * AVR EEPROM METHODS
 * Christopher Bero <berocs@acedb.co>
 */

#include "eeprom.h"

// Prints and returns a single byte from eeprom
uint8_t eeprom_get_byte(uint8_t * addr)
{
    uint8_t byte = 0x00;
    byte = eeprom_read_byte(addr);
//    if (DEBUG)
//    {
//        printf("Addr: [%p] Dat: [0x%02X]\n", addr, byte = eeprom_read_byte(addr));
//    }
    return byte;
}

// Prints and returns a buffer of bytes from eeprom
void eeprom_get_bytes(uint8_t * addr, uint8_t * buf, uint8_t len)
{
	for (uint8_t index = 0; index < len; index++)
	{
		buf[index] = eeprom_get_byte(addr+index);
	}
}

// Sets a single byte of eeprom
void eeprom_put_byte(uint8_t * addr, uint8_t data)
{
    eeprom_update_byte(addr, data);
}

// Sets a single byte of eeprom
void eeprom_put_bytes(uint8_t * addr, uint8_t * buf, uint8_t len)
{
	for (uint8_t index = 0; index < len; index++)
	{
		eeprom_put_byte(addr+index, buf[index]);
	}
}

// "Formats" a single byte
void eeprom_clear_byte(uint8_t * addr)
{
	eeprom_put_byte(addr, 0xff);
}

// "Formats" a section of bytes
void eeprom_clear_bytes(uint8_t * addr, uint16_t len)
{
	for (uint16_t index = 0; index < len; index++)
	{
		eeprom_put_byte(addr+index, 0xff);
	}
}





//// Prints (does not return) a length of eeprom bytes
//uint8_t eeprom_key_print(uint8_t * key, uint8_t keyLen)
//{
//    uint8_t * index;
//    printf("/");
//    for (index = key; index < (key+keyLen); index++)
//    {
//        printf("%c", (char)eeprom_read_byte(index));
//    }
//    printf("\n");
//    return(0);
//}
//
//// Prints all eeprom bytes
//void eeprom_byte_dump()
//{
//    // Variables
//    uint8_t * addr;
//    uint8_t subaddr;
//
//    printf("EEPROM:");
//    for (addr = 0; (addr+7) <= (uint8_t*)E2END; addr+=8)
//    {
////		printf("\n[0x%03X] 0x%03X", (uint16_t)addr, eeprom_read_byte(addr));
////		if (((uint16_t)addr%100) == 0)
////		{
////			wdt_reset();
////		}
//        printf("\nAddr [0x%03X-0x%03X]\t", (uint16_t)addr, (uint16_t)(addr+7));
//        for (subaddr = 0; subaddr < 8; subaddr++)
//        {
//            printf("[0x%02X] ", eeprom_read_byte(addr+subaddr));
//        }
//    }
//    printf("\n");
//}
//
//// Prints all keys in eeprom
//uint8_t eeprom_key_dump()
//{
//    uint8_t * addr;
//    uint8_t count;
//    count = 0;
//    for (addr = (uint8_t*)1; addr < (uint8_t*)E2END; addr+=11)
//    {
////        if (((uint16_t)addr%100) == 0)
////        {
////            wdt_reset();
////        }
//        uint8_t subaddr;
//        if (eeprom_read_byte((addr-1)) == 0xff && eeprom_read_byte(addr) != 0xff)
//        {
//        	count++;
//            if(DEBUG) printf("/");
//            for (subaddr = 0; subaddr < 10; subaddr++)
//            {
//                if (DEBUG) printf("%c", eeprom_read_byte((addr+subaddr)));
//            }
//            if (DEBUG) printf("\n");
//        }
//    }
//    return count;
//}
//
//// Returns a single byte (index: 0-9) from the key'th key in eeprom
//uint8_t eeprom_key_single_byte(uint8_t key, uint8_t index)
//{
//	if (index < 9)
//		return eeprom_read_byte((uint8_t*)(1 + (11*key)) + index);
//	return (0x00);
//}
//
//// Formats a key's worth of eeprom
//void eeprom_key_remove(uint8_t * key, uint8_t keyLen)
//{
//    // Variables
//    uint8_t * addr;
//    uint8_t index;
//
//    // Initialization
//    addr = eeprom_key_check(key, keyLen);
//    index = 0;
//
//    if (DEBUG)
//        printf("len: %d\nrm: \n", keyLen);
//
//    // Get the EEPROM address of the key
//    if (addr < 0) // Key doesn't exist in EEPROM
//    {
//        if (DEBUG)
//            printf("Already vacant.\n");
//        return;
//    }
//    else
//    {
//        if (DEBUG)
//            printf("Found. Address: %p\n", addr);
//    }
//
//    // Check that a trailing 0xff exists,
//    // otherwise we may delete the wrong size key.
//    if (eeprom_read_byte((addr+keyLen)) != 0xff)
//    {
//        printf("Terminating 0xff is absent. Aborting!\n");
////		printf("EEPROM address: %p\n", addr+keyLen);
//        return;
//    }
//
//    // Purge the key (set to 0x00)
//    for (index = 0; index < keyLen; ++index)
//    {
//        eeprom_put_byte((addr+index), 0x00);
//    }
//}
//
//// Inserts a key into eeprom
//uint8_t * eeprom_key_add(uint8_t * key, uint8_t keyLen)
//{
//    // Variables
//    uint8_t * addr;
//    //uint8_t blank[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//    int index;
//
//    // Initialization
//    addr = eeprom_key_check(key, keyLen); // Get the EEPROM address of the key
//    //blank[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//
////	if (debug||loud)
////		printf("check address: 0x%02X\n", addr);
//
//    if (addr) // Key is already in EEPROM
//    {
//        if (DEBUG)
//            printf("Key already exists.\n");
//        return addr;
//    }
//
//    // Get the EEPROM address of an empty spot
//    addr = eeprom_key_check((uint8_t*)
//    {
//        0
//    }, keyLen); // Oh. Oh man. This is sneaky.
//
//    if (addr) // Found an empty spot
//    {
//        for (index = 0; index < keyLen; ++index)
//        {
//            // Check that there isn't a terminating 0xff in the middle of our key.
//            if (eeprom_read_byte(addr+index) == 0xff)
//            {
//                printf("Part of the key was 0xff. Aborting!\n");
////				printf("EEPROM address: %p\n", addr+index);
//                return(0);
//            }
//            if (DEBUG)
////                printf("Updating address %p to value 0x%02X\n", (addr+index), key[index]);
//                eeprom_update_byte((addr+index), key[index]);
//        }
//        eeprom_update_byte((addr+index), 0xff);
//    }
//    else // No empty spots
//    {
//        // Iterate until two adjacent bytes are 0xff
//        for (addr = (uint8_t*)1; addr <= (uint8_t*)E2END; ++addr)
//        {
//            if (eeprom_read_byte(addr) == 0xff && eeprom_read_byte((addr-1)) == 0xff) // Match
//            {
//                for (index = 0; index < keyLen; ++index)
//                {
//                    if (DEBUG)
////                        printf("Updating address %p to value 0x%02X\n", (addr+index), key[index]);
//                        eeprom_update_byte((addr+index), key[index]);
//                }
//                eeprom_update_byte((addr+index), 0xff);
//                return addr;
//            }
//        }
//        printf(":c no dice.\n");
//    }
//    return(0);
//}
//
//// Check if the eeprom space is full
//uint8_t eeprom_full()
//{
//    // Variables
//    uint8_t * addr;
//
//    // Iterate until two adjacent bytes are 0xff
//    for (addr = (uint8_t*)(E2END-10); addr <= (uint8_t*)E2END; ++addr)
//    {
//        if (eeprom_read_byte(addr) != 0xFF)
//            return 1;
//    }
//    return 0;
//}
//
///* Usage:
// * byte testData[] = { 0x11,0x11,0x11,0x11,0x22,0x33,0x44 };
// * if ( eeprom_key_check(testData,7) >= 0 ) {
// * printf("ok");
// * } else {
// * printf("no");
// * }
// */
//uint8_t * eeprom_key_check(uint8_t* key, uint8_t keyLen)
//{
//    // Variables
//    uint8_t * e2idx;
//    uint8_t i;
//
//    // Initialization
//    e2idx = 0;
//    i = 0;
//
////	if (debug)
////		printf("Checking a key of length %d\n", keyLen);
//
//    while ( e2idx <= (uint8_t*)E2END )
//    {
////		if (debug)
////			printf("Starting check at e2idx %p\n", e2idx);
//
//        while ( i < keyLen )
//        {
//            if ( eeprom_read_byte((e2idx+i)) != key[i] )
//            {
////				if (debug)
////					printf("0x%02X Doesn't match 0x%02X\n", eeprom_read_byte(e2idx+i), key[i]);
//                break;
//            }
////			if (debug)
////				printf("%d %d\n",i, keyLen);
//
//            i++;
//
//            if ( i >= keyLen && eeprom_read_byte((e2idx+i)) == 0xff)
//            {
//                //we got through the whole check without breaking - it must match
//                //printf("It matches!!!");
//                // if (eeprom_read_byte(e2idx+i) != 0xff) // The terminator is missing
//                // {
//                //
//                // }
//                printf("Found at e2idx %p\n", e2idx);
//                return e2idx;
//            }
//        } // end card while
//
////		if (debug)
////			printf("No card found looking at %p\n", e2idx);
//
//        while ( eeprom_read_byte(e2idx) != 0xff )
//        {
////			if (debug)
////				printf("%p ", e2idx);
//            e2idx++;
//        }
//        e2idx++; // Find the 0xFF and then skip it
//    }//end e2 while
//    return(0);
//}

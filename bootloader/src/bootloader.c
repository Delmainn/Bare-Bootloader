#include "common-defines.h"
#include <libopencm3/stm32/memorymap.h>       // gives us FLASH_BASE so we're not hardcoding 0x08000000 by hand

/**  The bootloader is allotted exactly the first 32KB of flash.
 This number has to agree in THREE other places or the whole scheme breaks:
   1) bootloader/linkerscript.ld    -> rom LENGTH = 32K
   2) pad-bootloader.py             -> pads bootloader.bin to exactly this many bytes
   3) app/src/firmware.c            -> SCB_VTOR = BOOTLOADER_SIZE                          **/


// we need to find the address of main application
#define BOOTLOADER_SIZE        (0x8000)
#define MAIN_APP_START_ADDRESS (FLASH_BASE + BOOTLOADER_SIZE)  //address of main application: 0x08000000 + 0x8000 = 0x08008000



static void jump_to_main(void) {
    typedef void (*void_fn)(void);

    

    uint32_t* reset_vector_entry = (uint32_t*)(MAIN_APP_START_ADDRESS + 4U);   //pointer to the reset vector
    uint32_t* reset_vector = (uint32_t*)(*reset_vector_entry);

    void_fn jump_fn = (void_fn)reset_vector;

    jump_fn();
}

int main(void) {
    jump_to_main();
    
    //Never return {would not make sense to end}
    return 0;
}

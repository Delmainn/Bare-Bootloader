#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/iwdg.h>
#include "bl-flash.h"
#include <string.h>

#define MAIN_APP_SECTOR_START  (2)
#define MAIN_APP_SECTOR_END    (7)

void bl_flash_erase_main_application(void){
  flash_unlock();
  for (uint8_t sector = MAIN_APP_SECTOR_START; sector <= MAIN_APP_SECTOR_END; sector++) {
    flash_erase_sector(sector, FLASH_CR_PROGRAM_X32);
    iwdg_reset(); // the main loop isn't running during erase; refresh between sectors
  }
  flash_lock();
}

bool bl_flash_write(const uint32_t address, uint8_t* data, const uint32_t length){
   flash_unlock();
   flash_program(address, data, length);
  flash_lock();

  //read back: catches programming errors and bits a failed erase left at 0
  return memcmp((const void*)address, data, length) == 0;
}

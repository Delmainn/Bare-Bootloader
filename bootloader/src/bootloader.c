#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/memorymap.h>       // gives us FLASH_BASE so we're not hardcoding 0x08000000 by hand
#include <libopencm3/cm3/vector.h>       

#include "core/uart.h"
#include "core/system.h"
#include "core/simple-timer.h"
#include "comms.h"
#include "bl-flash.h"

/**  The bootloader is allotted exactly the first 32KB of flash.
 This number has to agree in THREE other places or the whole scheme breaks:
   1) bootloader/linkerscript.ld    -> rom LENGTH = 32K
   2) pad-bootloader.py             -> pads bootloader.bin to exactly this many bytes
   3) app/src/firmware.c            -> SCB_VTOR = BOOTLOADER_SIZE                          **/


// we need to find the address of main application

#define UART_PORT (GPIOA)
#define RX_PIN    (GPIO3) 
#define TX_PIN    (GPIO2) 

#define BOOTLOADER_SIZE        (0x8000U)
#define MAIN_APP_START_ADDRESS (FLASH_BASE + BOOTLOADER_SIZE)  //address of main application: 0x08000000 + 0x8000 = 0x08008000

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA); 
    gpio_mode_setup(UART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TX_PIN | RX_PIN);
    gpio_set_af(UART_PORT, GPIO_AF7, TX_PIN | RX_PIN); 
}

static void jump_to_main(void) {
    vector_table_t* main_vector_table = (vector_table_t*)MAIN_APP_START_ADDRESS;
    main_vector_table->reset();
}

int main(void) {
    system_setup();
    // gpio_setup();
    // uart_setup();
    // comms_setup();

    simple_timer_t timer;
    simple_timer_t timer2;
    simple_timer_setup(&timer, 1000, false);
    simple_timer_setup(&timer2, 2000, true);
        
    while (true) {
     if (simple_timer_has_elapsed(&timer)) {
        volatile int x = 0;
        x++;
     }

     if (simple_timer_has_elapsed(&timer2)) {
        simple_timer_reset(&timer);
     }

    }

    // TODO: Teardown

    jump_to_main();
    
    //Never return {would not make sense to end}
    return 0;
}

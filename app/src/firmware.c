#include <libopencm3/stm32/rcc.h>  // Header file for clock configuration (rcc clock)
#include <libopencm3/stm32/gpio.h> // Header file for gpio peripheral configuration (GPIOA)

#include "core/system.h"

// Macros for making the code easier to read
#define LED_PORT (GPIOA)  
#define LED_PIN  (GPIO5)

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA); // Switches on the peripheral, as all of them are OFF unless turned on
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);   // Cofigure the pin the LD2 is connected to.

}

// Program logically starts here
int main(void) {
    system_setup();
    gpio_setup();
    
    uint64_t start_time = system_get_ticks();
    
    while (1) {
        if (system_get_ticks()- start_time >= 1000) {  //Non-blocking delay: toggle only once 100 ms have elapsed since the last toggle. The CPU is free to do other work between toggles
        gpio_toggle(LED_PORT, LED_PIN); // Turn the LED ON and OFF continuously
        start_time = system_get_ticks();  // reset the reference point for the next interval
        }

    }
    //Never return {would not make sense to end}
    return 0;
}

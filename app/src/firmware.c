#include <libopencm3/stm32/rcc.h>  // Header file for clock configuration (rcc clock)
#include <libopencm3/stm32/gpio.h> // Header file for gpio peripheral configuration (GPIOA)

#include "core/system.h"
#include "core/timer.h"

// Macros for making the code easier to read
#define LED_PORT (GPIOA)  
#define LED_PIN  (GPIO5)

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA); // Switches on the peripheral, as all of them are OFF unless turned on
    gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);   // Cofigure the pin the LD2 is connected to.
    gpio_set_af(LED_PORT, GPIO_AF1, LED_PIN); // set the alternate function of this pin

}

// Program logically starts here
int main(void) {
    system_setup();
    gpio_setup();
    timer_setup();
    
    uint64_t start_time = system_get_ticks();
    float duty_cycle = 0.0f;

    timer_pwm_set_duty_cycle(duty_cycle);
    
    while (1) {
    if (system_get_ticks()- start_time >= 10) {  // Every 10ms, step duty cycle by 1% (full 0 - 100% fade over ~1 second)
    duty_cycle += 1.0f;
    if (duty_cycle > 100.0f) {
       duty_cycle = 0.0f;

        }
    timer_pwm_set_duty_cycle(duty_cycle);

    start_time = system_get_ticks();  // reset the reference point for the next interval
        }

    }
    //Never return {would not make sense to end}
    return 0;
}

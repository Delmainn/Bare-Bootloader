#include <libopencm3/stm32/rcc.h>  // Header file for clock configuration (rcc clock)
#include <libopencm3/stm32/gpio.h> // Header file for gpio peripheral configuration (GPIOA)
#include <libopencm3/cm3/scb.h>    // needed now specifically for SCB_VTOR

#include "core/system.h"
#include "core/uart.h"
#include "timer.h"

#define BOOTLOADER_SIZE (0x8000U)   // must match the bootloader's own BOOTLOADER_SIZE


// Hardware definitions
#define LED_PORT (GPIOA)  
#define LED_PIN  (GPIO5)

#define UART_PORT (GPIOA)
#define RX_PIN    (GPIO3) 
#define TX_PIN    (GPIO2) 


static void vector_setup(void) {
     //Relocate the vector table to the application's flash region so
    // interrupts are handled by the application rather than the bootloader
    SCB_VTOR = BOOTLOADER_SIZE;
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA); // Switches on the peripheral, as all of them are OFF unless turned on
    gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);   // Cofigure the pin the LD2 is connected to.
    gpio_set_af(LED_PORT, GPIO_AF1, LED_PIN); // set the alternate function of this pin (the timer, if im not mistaken)

    gpio_mode_setup(UART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TX_PIN | RX_PIN);   // configure the pins the UART is connected to
    gpio_set_af(UART_PORT, GPIO_AF7, TX_PIN | RX_PIN); // set the alternate function of this pin (UART)

}

// Program logically starts here
int main(void) {
    vector_setup();
    system_setup();
    gpio_setup();
    timer_setup();
    uart_setup();
    
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

        while (uart_data_available()) {
            uint8_t data = uart_read_byte();
            uart_write_byte(data + 1);

        }

       system_delay(1000);
        //do useful work


    }
    //Never return {would not make sense to end}
    return 0;
}

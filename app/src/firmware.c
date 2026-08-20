#include <libopencm3/stm32/rcc.h>  // Header file for clock configuration (rcc clock)
#include <libopencm3/stm32/gpio.h> // Header file for gpio peripheral configuration (GPIOA)

// Macros for making the code easier to read
#define LED_PORT (GPIOA)  
#define LED_PIN  (GPIO5)


static void rcc_setup(void) // This function is visible and available ONLY to this translation unit i.e., this C file + included Header files
 {
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);  // Set the CPU clock (the hardware that sets up that clock frquency (phase locked loop)) and frequency
}   

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA); // Switches on the peripheral, as all of them are OFF unless turned on
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);   // Cofigure the pin the LD2 is connected to.

}
static void delay_cycles(uint32_t cycles) // Function to have a number of iterations to introduce a delay
{
    for (uint32_t i = 0; i < cycles; i++) {
        __asm__("nop");  // Avoid the compiler from optimising away this loop
    }
}

// Program logically starts here
int main(void) {
    rcc_setup();
    gpio_setup();

    while (1) {
        gpio_toggle(LED_PORT, LED_PIN); // Turn the LED ON and OFF continuously
        delay_cycles(84000000 / 4);  //We're calling it cycles, but its not really that... more like iterations!

    }


    //Never return {would not make sense to end}
    return 0;
}

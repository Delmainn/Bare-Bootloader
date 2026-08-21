#include <libopencm3/stm32/rcc.h>  // Header file for clock configuration (rcc clock)
#include <libopencm3/stm32/gpio.h> // Header file for gpio peripheral configuration (GPIOA)
#include <libopencm3/cm3/systick.h> // Header file for the systick timer
#include <libopencm3/cm3/vector.h>  // Header file for the vector table

// Macros for making the code easier to read
#define LED_PORT (GPIOA)  
#define LED_PIN  (GPIO5)

#define CPU_FREQ     (84000000)   //system clock: 84MHz
#define SYSTICK_FREQ (1000)       //Systick frequency: 1000Hz or 1ms per tick


// SysTick interrupt handler. This exact name is what libopencm3's vector
// table expects, so naming it sys_tick_handler wires it to the SysTick IRQ
// automatically.
volatile uint64_t ticks = 0;
void sys_tick_handler(void) {
    ticks++;

}

// Reads ticks counts
static uint64_t get_ticks(void){
    return ticks;
}

static void rcc_setup(void) // This function is visible and available ONLY to this translation unit i.e., this C file + included Header files
 {
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);  // Set the CPU clock (the hardware that sets up that clock frquency (phase locked loop)) and frequency
}   

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA); // Switches on the peripheral, as all of them are OFF unless turned on
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);   // Cofigure the pin the LD2 is connected to.

}

// Configure SysTick to interrupt at SYSTICK_FREQ given the CPU clock,
// enable the counter, and enable its interrupt so sys_tick_handler starts.
static void systick_setup(void) {
    systick_set_frequency(SYSTICK_FREQ,CPU_FREQ);
    systick_counter_enable();    // start counting
    systick_interrupt_enable(); // enable interrupt lol
}

// Program logically starts here
int main(void) {
    rcc_setup();
    gpio_setup();
    systick_setup();

    uint64_t start_time = get_ticks();
    
    while (1) {
        if (get_ticks()- start_time >= 100) {  //Non-blocking delay: toggle only once 100 ms have elapsed since the last toggle. The CPU is free to do other work between toggles
        gpio_toggle(LED_PORT, LED_PIN); // Turn the LED ON and OFF continuously
        start_time = get_ticks();  // reset the reference point for the next interval
        }

    }


    //Never return {would not make sense to end}
    return 0;
}

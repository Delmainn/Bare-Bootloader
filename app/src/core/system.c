#include "core/system.h"
#include <libopencm3/cm3/systick.h> // Header file for the systick timer
#include <libopencm3/cm3/vector.h>  // Header file for the vector table
#include <libopencm3/stm32/rcc.h>  // Header file for clock configuration (rcc clock)

// SysTick interrupt handler. This exact name is what libopencm3's vector
// table expects, so naming it sys_tick_handler wires it to the SysTick IRQ
// automatically.
static volatile uint64_t ticks = 0;

void sys_tick_handler(void) {
    ticks++;
}

static void rcc_setup(void) // This function is visible and available ONLY to this translation unit i.e., this C file + included Header files
 {
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);  // Set the CPU clock (the hardware that sets up that clock frquency (phase locked loop)) and frequency
}   

// Configure SysTick to interrupt at SYSTICK_FREQ given the CPU clock,
// enable the counter, and enable its interrupt so sys_tick_handler starts.
static void systick_setup(void) {
    systick_set_frequency(SYSTICK_FREQ,CPU_FREQ);
    systick_counter_enable();    // start counting
    systick_interrupt_enable(); // enable interrupt lol
}

// Reads ticks counts
uint64_t system_get_ticks(void){
    return ticks;
}

void system_setup(void) {
    rcc_setup();
    systick_setup();

}

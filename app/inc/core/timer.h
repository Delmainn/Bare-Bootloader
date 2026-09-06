#ifndef INC_TIMER_H
#define INC_TIMER_H

// Initializes TIM2 for PWM output on channel 1 (clock, mode, PWM mode, frequency)
void timer_setup(void);

// Sets PWM output duty cycle as a percentage (0.0 - 100.0)
void timer_pwm_set_duty_cycle(float duty_cycle);  //helper function for pwm

#endif //INC_TIMER_H

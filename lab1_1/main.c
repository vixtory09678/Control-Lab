/*
    robotics-za.blogspot.com
    https://github.com/vixtory09678/Common_PIC
*/

#include "lib/24FJ48GA002.h"
#include "lib/BL_Support.h"

#use delay(internal = 8 MHz, clock = 32MHz)
#PIN_SELECT U1RX = PIN_B12 // pin_b14
#PIN_SELECT U1TX = PIN_B13 // pin_b15
#use rs232(UART1, BAUD = 115200, XMIT = PIN_B13, RCV = PIN_B12)

#PIN_SELECT oc1 = PIN_B2

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void analogWrite(long duty, long freq)
{
    long TIME_PIRIOD = (2000000 / freq);
    duty = (duty < 1) ? 0 : duty;

    long duty_pwm = (duty / 100.0) * TIME_PIRIOD;
    printf("%d,%d,%d\r\n", duty_pwm, (duty / 100.0), TIME_PIRIOD);
    // duty_pwm = TI
    if (duty != 0)
    {
        duty_pwm = duty_pwm + 1;
    }
    setup_timer2(TMR_INTERNAL | TMR_DIV_BY_8, TIME_PIRIOD);
    setup_compare(1, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(1, duty_pwm);
}

void main(void)
{
    disable_interrupts(GLOBAL);
    enable_interrupts(GLOBAL);
    //setup_timer2(TMR_INTERNAL | TMR_DIV_BY_256, 625);
    //setup_compare(1, COMPARE_PWM | COMPARE_TIMER2);
    //set_pwm_duty(1, 312);
    analogWrite(100, 10000);
    while (TRUE)
    {
    }
}

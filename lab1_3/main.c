/*
    robotics-za.blogspot.com
    https://github.com/vixtory09678/Common_PIC
*/

#include "lib/24FJ48GA002.h"
#device ADC = 10
#include "lib/BL_Support.h"

#use delay(internal = 8 MHz, clock = 32MHz)
#PIN_SELECT U1RX = PIN_B12 // pin_b14
#PIN_SELECT U1TX = PIN_B13 // pin_b15
#use rs232(UART1, BAUD = 115200, XMIT = PIN_B13, RCV = PIN_B12)

#define IN1 PIN_B2
#define IN2 PIN_B3

#PIN_SELECT oc1 = IN1
#PIN_SELECT oc2 = IN2

int adc_data;

void init_adc()
{
    setup_adc_ports(sAN3, VSS_VDD);
    setup_adc(ADC_CLOCK);
    set_adc_channel(3);
    delay_us(10);
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void analogWrite_L(long duty, long freq)
{
    long TIME_PIRIOD = (2000000 / freq);
    duty = (duty < 1) ? 0 : duty;

    long duty_pwm = (duty / 100.0) * TIME_PIRIOD;
    // duty_pwm = TIME_PIRIOD - duty_pwm;

    if (duty != 0)
    {
        duty_pwm = duty_pwm + 1;
    }
    //printf("%d,%d,%d\r\n", duty_pwm, (duty / 100.0), TIME_PIRIOD);
    setup_timer2(TMR_INTERNAL | TMR_DIV_BY_8, TIME_PIRIOD);
    setup_compare(1, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(1, duty_pwm);
}

void printTimeFloat(float n)
{
    int intpart = (int)n;
    int decpart = (n - intpart) * 100;
    decpart = abs(decpart);
    if (n < 0.0f)
        printf("-%d.%d%d", abs(intpart), (decpart < 10) ? 0 : decpart / 10, decpart % 10);
    else
        printf("%d.%d%d", intpart, (decpart < 10) ? 0 : decpart / 10, decpart % 10);
}

void analogWrite_R(long duty, long freq)
{
    long TIME_PIRIOD = (2000000 / freq);
    duty = (duty < 1) ? 0 : duty;

    long duty_pwm = (duty / 100.0) * TIME_PIRIOD;
    // duty_pwm = TIME_PIRIOD - duty_pwm;

    if (duty != 0)
    {
        duty_pwm = duty_pwm + 1;
    }
    setup_timer2(TMR_INTERNAL | TMR_DIV_BY_8, TIME_PIRIOD);
    setup_compare(2, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(2, duty_pwm);
}

void driveMotor(int dir, int speed)
{
    switch (dir)
    {
    case 0:
        analogWrite_L(speed, 10000);
        analogWrite_R(0, 10000);
        break;
    case 1:
        analogWrite_L(0, 10000);
        analogWrite_R(speed, 10000);
        break;
    case 2:
        analogWrite_L(0, 10000);
        analogWrite_R(0, 10000);
        break;
    }
}

void main(void)
{
    disable_interrupts(GLOBAL);
    enable_interrupts(GLOBAL);

    //int duty = map(500, 0, 500, 0, 100);
    //driveMotor(0, abs(duty));
    init_adc();
    adc_data = 0;
    float Kp = 2;
    float Ki = 0;
    float Kd = 0.5;
    int s = 0;
    int p = 0;
    int theta_d = 512;
    float u = 0;
    while (TRUE)
    {
        adc_data = read_adc();

        //delay_ms(500);
        int e = theta_d - adc_data;
        s = s + e;
        if (abs(e) > 2)
        {
            u = Kp * e + Ki * s + Kd * (e - p);
        }
        else
        {
            u = 0;
        }
        boolean direct = (e < 0) ? 1 : 0;
        if (u > 1000)
        {
            u = 1000;
        }
        int speed = map(abs(u), 0, 1000, 0, 100);
        driveMotor(direct, speed);
        p = e;
        printf("AN3: %d ", read_adc());
        printTimeFloat(u);
        printf(" %d\r\n", e);
    }
}

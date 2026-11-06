#include "mbed.h"
PwmOut pwm(D6);
Ticker dutyTicker;
bool status = false;

void change() {
    if (status) 
    {
        pwm.write(0.3f);        // 30% duty cycle
    } 
    else 
    {
        pwm.write(0.7f);        // 70% duty cycle
    }
    status = !status;
}

int main() 
{
    pwm.period(2);      // 2s period (0.5Hz)
    pwm.write(0.25f);        // Start with 25% duty
    dutyTicker.attach(&change, 2.0f);       // Toggle every 2 seconds
    while(1){}      // Main program loop
}
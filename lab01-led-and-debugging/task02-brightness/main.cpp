#include "mbed.h"
PwmOut led(D5);  //led
DigitalIn bright_up(D10, PullDown);  //button 1
DigitalIn bright_down(D11, PullDown);  //button 2
float brightness = 0.1;  //duty cycle
void scan()  //scan the button state, increase/decrease the brightness of led
{
    while (bright_up == 1)  //need to be up
    {
        led.write(brightness);  //current brightness
        if (brightness == 1)  //if full
        {
            brightness = 1;  //maintain
        }
        else
        {
            brightness += 0.01;  //up
        }
        ThisThread::sleep_for(25ms);  //change pace
    }

    while (bright_down == 1)  //need to be down
    {
        led.write(brightness);
        if (brightness == 0)  //if empty
        {
            brightness = 0;
        }
        else
        {
            brightness -= 0.01;  //down
        }
        ThisThread::sleep_for(25ms);
    }
}
int main()
{
    led.period_us(1000);  //set up
    while(true)
    {
        scan();  //scan and record
        led.write(brightness);  //change
        ThisThread::sleep_for(10ms);  //change pace
    }
}
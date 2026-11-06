#include "mbed.h"
DigitalOut led(PA_5);               //pin D4
int main() {     
    while (true)                    //loop
    {         
        led = 1;                    //light on
        printf("LED should toggle\n");
        ThisThread::sleep_for(500); //stay for 500ms
        led = 0;                    //light out
        ThisThread::sleep_for(500); //stay for 500ms
    } 
}
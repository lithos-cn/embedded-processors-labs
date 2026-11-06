#include "mbed.h"

// SPI pin definitions
#define SPI1_MOSI A6
#define SPI1_MISO A5
#define SPI1_SCLK A4
#define SPI1_CS D12

// LED pin definitions for X and Y axis indication
#define LXG1 D11    // X-axis Green LED 1 (>0.5g)
#define LXG2 D9     // X-axis Green LED 2 (0.05-0.5g)
#define LXR1 D0     // X-axis Red LED 1 (-0.5 to -0.05g)
#define LXR2 D1     // X-axis Red LED 2 (<-0.5g)
#define LYG1 D6     // Y-axis Green LED 1 (>0.5g)
#define LYG2 A3     // Y-axis Green LED 2 (0.05-0.5g)
#define LYR1 A2     // Y-axis Red LED 1 (-0.5 to -0.05g)
#define LYR2 A0     // Y-axis Red LED 2 (<-0.5g)

// Initialize SPI and LED objects
SPI ADXL345(SPI1_MOSI, SPI1_MISO, SPI1_SCLK);
DigitalOut ADXL345_cs(SPI1_CS);
DigitalOut lxg1(LXG1), lxg2(LXG2), lxr1(LXR1), lxr2(LXR2);
DigitalOut lyg1(LYG1), lyg2(LYG2), lyr1(LYR1), lyr2(LYR2);

char buffer[6];
int16_t data[3];
float x, y, z;

// ADXL345 initialization function (same as Task 3)
void ADXL345_SPI_Initialise() {
    // ... (same initialization code as Task 3)
}

// Function to update LEDs based on X and Y acceleration values
void update_leds(float x, float y) {
    // Control X-axis LEDs based on acceleration threshold
    if (x >= 0.5) {
        lxg1 = 1; lxg2 = 0; lxr1 = 0; lxr2 = 0;  // Strong positive X
    } else if (x <= -0.5) {
        lxr2 = 1; lxr1 = 0; lxg1 = 0; lxg2 = 0;  // Strong negative X
    } else if (x > 0.05 && x < 0.5) {
        lxg2 = 1; lxg1 = 0; lxr1 = 0; lxr2 = 0;  // Moderate positive X
    } else if (x > -0.5 && x < -0.05) {
        lxr1 = 1; lxr2 = 0; lxg1 = 0; lxg2 = 0;  // Moderate negative X
    }

    // Control Y-axis LEDs based on acceleration threshold
    if (y >= 0.5) {
        lyg1 = 1; lyg2 = 0; lyr1 = 0; lyr2 = 0;  // Strong positive Y
    } else if (y <= -0.5) {
        lyr2 = 1; lyr1 = 0; lyg1 = 0; lyg2 = 0;  // Strong negative Y
    } else if (y > 0.05 && y < 0.5) {
        lyg2 = 1; lyg1 = 0; lyr1 = 0; lyr2 = 0;  // Moderate positive Y
    } else if (y > -0.5 && y < -0.05) {
        lyr1 = 1; lyr2 = 0; lyg1 = 0; lyg2 = 0;  // Moderate negative Y
    }
}

int main() {
    ADXL345_SPI_Initialise();
    
    while(1) {
        wait_us(200000);
        
        // Read accelerometer data (same as Task 3)
        ADXL345_cs = 0;
        ADXL345.write(0x80 | 0x40 | 0x32);
        
        for (int i = 0; i < 6; i++) {
            buffer[i] = ADXL345.write(0x00);
        }
        
        ADXL345_cs = 1;
        
        // Process data
        data[0] = buffer[1] << 8 | buffer[0];
        data[1] = buffer[3] << 8 | buffer[2];
        data[2] = buffer[5] << 8 | buffer[4];
        
        x = 0.004 * data[0];
        y = 0.004 * data[1];
        z = 0.004 * data[2];
        
        update_leds(x, y);  // Update LED states based on tilt
        
        printf("x = %+1.2f g\ty = %+1.2f g\tz = %+1.2f g\n\r", x, y, z);
    }
}
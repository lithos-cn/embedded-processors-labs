#include "mbed.h"

// SPI pin definitions
#define SPI1_MOSI A6
#define SPI1_MISO A5
#define SPI1_SCLK A4
#define SPI1_CS D12

// LED pin definitions for 8-directional control
#define LXG1 D11    // LED for North direction
#define LXG2 D9     // LED for Northeast direction
#define LXR1 D0     // LED for East direction
#define LXR2 D1     // LED for Southeast direction
#define LYG1 D6     // LED for South direction
#define LYG2 A3     // LED for Southwest direction
#define LYR1 A2     // LED for West direction
#define LYR2 A0     // LED for Northwest direction

// Initialize SPI and LED objects
SPI ADXL345(SPI1_MOSI, SPI1_MISO, SPI1_SCLK);
DigitalOut ADXL345_cs(SPI1_CS);
DigitalOut lxg1(LXG1), lxg2(LXG2), lxr1(LXR1), lxr2(LXR2);
DigitalOut lyg1(LYG1), lyg2(LYG2), lyr1(LYR1), lyr2(LYR2);

// Data storage variables
char buffer[6];     // Raw data buffer
int16_t data[3];    // 16-bit signed integers for X, Y, Z
float x, y, z;      // Floating point values in g-force units

// Function to initialize ADXL345 accelerometer
void ADXL345_SPI_Initialise() {
    ADXL345_cs = 1;                 // Deselect device initially
    ADXL345.format(8,3);            // 8-bit data, SPI mode 3
    ADXL345.frequency(2000000);     // Set clock to 2 MHz
    
    // Configure data format register (0x31)
    ADXL345_cs = 0;                 // Select device
    ADXL345.write(0x31);            // Point to data format register
    ADXL345.write(0x0B);            // Set ±16g range, 0.004g/LSB resolution
    ADXL345_cs = 1;                 // Deselect device
    
    // Configure power control register (0x2D)
    ADXL345_cs = 0;                 // Select device
    ADXL345.write(0x2D);            // Point to power control register
    ADXL345.write(0x08);            // Enable measurement mode
    ADXL345_cs = 1;                 // Deselect device
}

// Function for 8-directional LED control based on X-Y combination
void update_leds(float x, float y) {
    // Turn off all LEDs first to ensure only one LED is active
    lxg1 = lxg2 = lxr1 = lxr2 = lyg1 = lyg2 = lyr1 = lyr2 = 0;
    
    // 8-directional control based on X-Y quadrants and thresholds
    if (x > -0.5 && x < 0.5 && y < -0.5) {
        lxg1 = 1;  // North direction (Y negative, X neutral)
    } else if (x > 0.5 && y < -0.5) {
        lxg2 = 1;  // Northeast direction (X positive, Y negative)
    } else if (x > 0.5 && y > -0.5 && y < 0.5) {
        lxr1 = 1;  // East direction (X positive, Y neutral)
    } else if (x > 0.5 && y > 0.5) {
        lxr2 = 1;  // Southeast direction (X positive, Y positive)
    } else if (x > -0.5 && x < 0.5 && y > 0.5) {
        lyg1 = 1;  // South direction (Y positive, X neutral)
    } else if (x < -0.5 && y > 0.5) {
        lyg2 = 1;  // Southwest direction (X negative, Y positive)
    } else if (x < -0.5 && y > -0.5 && y < 0.5) {
        lyr1 = 1;  // West direction (X negative, Y neutral)
    } else if (x < -0.5 && y < -0.5) {
        lyr2 = 1;  // Northwest direction (X negative, Y negative)
    }
    // If both X and Y are within [-0.5, 0.5], no LED is lit (center/level position)
}

int main() {
    ADXL345_SPI_Initialise();       // Initialize accelerometer
    
    while(1) {
        wait_us(200000);            // Wait 200ms between readings
        
        // Read 6 bytes of acceleration data starting from register 0x32
        ADXL345_cs = 0;             // Select device
        ADXL345.write(0x80 | 0x40 | 0x32); // Read bit + Multi-byte + X0 address
        
        // Read 6 consecutive bytes (X0, X1, Y0, Y1, Z0, Z1)
        for (int i = 0; i < 6; i++) {
            buffer[i] = ADXL345.write(0x00);
        }
        
        ADXL345_cs = 1;             // Deselect device
        
        // Combine LSB and MSB for each axis (little-endian format)
        data[0] = buffer[1] << 8 | buffer[0];  // X-axis
        data[1] = buffer[3] << 8 | buffer[2];  // Y-axis
        data[2] = buffer[5] << 8 | buffer[4];  // Z-axis
        
        // Convert to actual g-force values
        x = 0.004 * data[0];
        y = 0.004 * data[1];
        z = 0.004 * data[2];
        
        update_leds(x, y);          // Update LED states for 8-directional control
        
        // Print formatted output to serial terminal
        printf("x = %+1.2f g\ty = %+1.2f g\tz = %+1.2f g\n\r", x, y, z);
    }
}
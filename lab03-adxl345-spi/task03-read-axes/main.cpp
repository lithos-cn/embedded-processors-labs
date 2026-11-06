#include "mbed.h"

// Define SPI pin connections
#define SPI1_MOSI A6    // Master Out Slave In
#define SPI1_MISO A5    // Master In Slave Out  
#define SPI1_SCLK A4    // Serial Clock
#define SPI1_CS D12     // Chip Select

// Initialize SPI interface and chip select pin
SPI ADXL345(SPI1_MOSI, SPI1_MISO, SPI1_SCLK);
DigitalOut ADXL345_cs(SPI1_CS);

// Data storage variables
char buffer[6];     // Raw data buffer for 6 bytes (2 bytes per axis)
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
        
        // Print formatted output to serial terminal
        printf("x = %+1.2f g\ty = %+1.2f g\tz = %+1.2f g\n\r", x, y, z);
    }
}
#include "mbed.h"

// I2C pin definitions for communication with TMP102
#define I2CSDA D0  // Serial Data line
#define I2CSCL D1  // Serial Clock line

// TMP102 register addresses according to datasheet
#define Temp_Reg 0x00    // Temperature register (read-only)
#define Config_Reg 0x01  // Configuration register
#define TLow_Reg 0x02    // Low temperature threshold register
#define THigh_Reg 0x03   // High temperature threshold register

// Initialize I2C interface and LED outputs
I2C TMP102(I2CSDA, I2CSCL);
DigitalOut Temp_High(LED1);    // LED for high temperature alert
DigitalOut Temp_Low(D12);      // LED for low temperature alert

// TMP102 sensor configuration and data variables
const int TMP102Address = 0x90;    // I2C slave address for TMP102
char ConfigRegisterTMP102[3];      // Buffer for configuration commands
char TemperatureRegister[2];       // Buffer for temperature data (2 bytes)
float Temperature;                 // Final calculated temperature in Celsius

// Function to configure TMP102 sensor for continuous conversion
void ConfigureTMP102() {
    ConfigRegisterTMP102[0] = Config_Reg;  // Point to configuration register
    ConfigRegisterTMP102[1] = 0x60;        // Config byte 1: normal operation
    ConfigRegisterTMP102[2] = 0xA0;        // Config byte 2: default settings
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);  // Send configuration
}

// Function to set low temperature threshold to 20°C
void Set_Thermostat_Temp_Low() {
    ConfigRegisterTMP102[0] = TLow_Reg;    // Point to low threshold register
    ConfigRegisterTMP102[1] = 0x14;        // 20°C converted to sensor format
    ConfigRegisterTMP102[2] = 0x00;        // Upper byte
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

// Function to set high temperature threshold to 22°C
void Set_Thermostat_Temp_High() {
    ConfigRegisterTMP102[0] = THigh_Reg;   // Point to high threshold register
    ConfigRegisterTMP102[1] = 0x16;        // 22°C converted to sensor format
    ConfigRegisterTMP102[2] = 0x00;        // Upper byte
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

int main() {
    unsigned short M;  // Main temperature bits
    char L;           // Additional precision bits

    // Initialize sensor configuration once
    ConfigureTMP102();
    Set_Thermostat_Temp_Low();   // Set temperature alert thresholds
    Set_Thermostat_Temp_High();

    // Point to temperature register for reading
    ConfigRegisterTMP102[0] = Temp_Reg;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 1);

    printf("Initiating Temperature Reading...\n\n");

    while(1) {
        // Read 2 bytes of temperature data from sensor
        TMP102.read(TMP102Address, TemperatureRegister, 2);

        // Convert raw bytes to temperature value
        M = TemperatureRegister[0] << 4;   // Shift MSB left by 4 bits
        L = TemperatureRegister[1] >> 4;   // Shift LSB right by 4 bits
        M = M + L;                         // Combine for 12-bit value
        Temperature = 0.0625 * M;          // Convert to Celsius (resolution: 0.0625°C)

        // Display raw sensor value and calculated temperature
        printf("Temperature = %u\n\n", M);
        printf("Temperature in Celsius = %.3f\n\n", Temperature);
        wait_us(1000000);  // Wait 1 second before next reading
    }
}

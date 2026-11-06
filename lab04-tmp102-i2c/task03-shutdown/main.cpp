#include "mbed.h"

// I2C pin definitions for TMP102 sensor
#define I2CSDA D0
#define I2CSCL D1

// TMP102 register addresses
#define Temp_Reg 0x00      // Temperature register
#define Config_Reg 0x01    // Configuration register
#define TLow_Reg 0x02      // Low temperature threshold register
#define THigh_Reg 0x03     // High temperature threshold register

// Initialize I2C interface and output pins
I2C TMP102(I2CSDA, I2CSCL);
DigitalOut Temp_High(LED1);    // LED indicator for high temperature
DigitalOut Temp_Low(D12);      // LED indicator for low temperature

// TMP102 sensor constants and variables
const int TMP102Address = 0x90;    // I2C slave address for TMP102
char ConfigRegisterTMP102[3];      // Buffer for configuration data
char TemperatureRegister[2];       // Buffer for temperature reading
float Temperature;                 // Calculated temperature in Celsius

// Function to configure TMP102 in shutdown mode
void ConfigureTMP102_Shutdown() {
    ConfigRegisterTMP102[0] = Config_Reg;  // Point to configuration register
    ConfigRegisterTMP102[1] = 0x61;        // Config byte 1: SD bit = 1 (shutdown mode enabled)
    ConfigRegisterTMP102[2] = 0xA0;        // Config byte 2: default settings
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);  // Write configuration
    printf("TMP102 configured in shutdown mode\n");
}

// Function to set low temperature threshold (20°C)
void Set_Thermostat_Temp_Low() {
    ConfigRegisterTMP102[0] = TLow_Reg;    // Point to low threshold register
    ConfigRegisterTMP102[1] = 0x14;        // Low byte for 20°C
    ConfigRegisterTMP102[2] = 0x00;        // High byte
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

// Function to set high temperature threshold (22°C)
void Set_Thermostat_Temp_High() {
    ConfigRegisterTMP102[0] = THigh_Reg;   // Point to high threshold register
    ConfigRegisterTMP102[1] = 0x16;        // Low byte for 22°C
    ConfigRegisterTMP102[2] = 0x00;        // High byte
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

int main() {
    // Initialize sensor configuration
    printf("Initializing TMP102 in shutdown mode...\n");
    
    // Configure TMP102 in shutdown mode
    ConfigureTMP102_Shutdown();
    Set_Thermostat_Temp_Low();   // Set temperature thresholds
    Set_Thermostat_Temp_High();
    
    printf("TMP102 is now in shutdown mode.\n");
    printf("Current consumption should be reduced to ~2μA\n");
    printf("Measure current with ammeter to verify shutdown mode.\n");
    
    // Main loop - sensor remains in shutdown mode
    while(1) {
        // In shutdown mode, sensor stops continuous conversion
        // Current consumption is minimized
        wait_us(1000000);  // 1 second delay
    }
}
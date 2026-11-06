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

// Function to configure TMP102 in normal continuous mode
void ConfigureTMP102_Normal() {
    ConfigRegisterTMP102[0] = Config_Reg;  // Point to configuration register
    ConfigRegisterTMP102[1] = 0x60;        // Config byte 1: SD bit = 0 (normal mode)
    ConfigRegisterTMP102[2] = 0xA0;        // Config byte 2: default settings
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);  // Write configuration
    printf("TMP102 configured in normal continuous mode\n");
}

// Function to configure TMP102 in shutdown mode
void ConfigureTMP102_Shutdown() {
    ConfigRegisterTMP102[0] = Config_Reg;  // Point to configuration register
    ConfigRegisterTMP102[1] = 0x61;        // Config byte 1: SD bit = 1 (shutdown mode)
    ConfigRegisterTMP102[2] = 0xA0;        // Config byte 2: default settings
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);  // Write configuration
    printf("TMP102 configured in shutdown mode\n");
}

// Function to set temperature thresholds
void Set_Temperature_Thresholds() {
    // Set low temperature threshold (20°C)
    ConfigRegisterTMP102[0] = TLow_Reg;
    ConfigRegisterTMP102[1] = 0x14;
    ConfigRegisterTMP102[2] = 0x00;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
    
    // Set high temperature threshold (22°C)
    ConfigRegisterTMP102[0] = THigh_Reg;
    ConfigRegisterTMP102[1] = 0x16;
    ConfigRegisterTMP102[2] = 0x00;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

int main() {
    printf("=== TMP102 Current Measurement Test ===\n\n");
    
    // Phase 1: Normal operation mode
    printf("Phase 1: Normal Continuous Mode\n");
    printf("--------------------------------\n");
    ConfigureTMP102_Normal();           // Configure in normal mode
    Set_Temperature_Thresholds();       // Set temperature thresholds
    
    printf("TMP102 is now in normal continuous conversion mode.\n");
    printf("Expected current consumption: ~24μA\n");
    printf("Please measure current with ammeter and record the value.\n");
    printf("Press any key to continue to shutdown mode test...\n\n");
    
    // Wait for 10 seconds to allow current measurement
    for(int i = 10; i > 0; i--) {
        printf("Normal mode test: %d seconds remaining\n", i);
        wait_us(1000000);  // 1 second delay
    }
    
    printf("\n");
    
    // Phase 2: Shutdown mode
    printf("Phase 2: Shutdown Mode\n");
    printf("----------------------\n");
    ConfigureTMP102_Shutdown();         // Configure in shutdown mode
    
    printf("TMP102 is now in shutdown mode.\n");
    printf("Expected current consumption: ~2μA\n");
    printf("Please measure current with ammeter and record the value.\n");
    printf("Energy saving calculation: ((24-2)/24) * 100%% = ~92%% power reduction\n\n");
    
    // Main loop - sensor remains in shutdown mode for measurement
    while(1) {
        printf("Shutdown mode active - measure current consumption\n");
        wait_us(5000000);  // 5 second delay for measurement
    }
}
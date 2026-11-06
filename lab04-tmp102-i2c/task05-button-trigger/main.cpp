#include "mbed.h"

// Same definitions as previous tasks
#define I2CSDA D0
#define I2CSCL D1
#define Temp_Reg 0x00
#define Config_Reg 0x01
#define TLow_Reg 0x02
#define THigh_Reg 0x03

I2C TMP102(I2CSDA, I2CSCL);
DigitalOut Temp_High(LED1);
DigitalOut Temp_Low(D12);
InterruptIn button(D3);     // External interrupt from push button

const int TMP102Address = 0x90;
char ConfigRegisterTMP102[3];
char TemperatureRegister[2];
float Temperature;
float change = 0;           // Flag to indicate button press
Timeout debounceTimeout;    // Timeout for button debouncing

// Function to configure TMP102 in shutdown mode (SD bit = 1)
void ConfigureTMP102SD1() {
    ConfigRegisterTMP102[0] = Config_Reg;
    ConfigRegisterTMP102[1] = 0x61;      // SD bit = 1 (shutdown mode enabled)
    ConfigRegisterTMP102[2] = 0xA0;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

// Function to configure TMP102 in normal mode (SD bit = 0)
void ConfigureTMP102SD0() {
    ConfigRegisterTMP102[0] = Config_Reg;
    ConfigRegisterTMP102[1] = 0x60;      // SD bit = 0 (normal operation)
    ConfigRegisterTMP102[2] = 0xA0;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

// Set temperature thresholds (same as previous tasks)
void Set_Thermostat_Temp_Low() {
    ConfigRegisterTMP102[0] = TLow_Reg;
    ConfigRegisterTMP102[1] = 0x14;
    ConfigRegisterTMP102[2] = 0x00;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

void Set_Thermostat_Temp_High() {
    ConfigRegisterTMP102[0] = THigh_Reg;
    ConfigRegisterTMP102[1] = 0x16;
    ConfigRegisterTMP102[2] = 0x00;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

// Debounce callback function to confirm button press
void debounceCallback() {
    if (button.read() == 1) {    // Verify button is still pressed after debounce
        change = 1;              // Set flag to trigger temperature reading
    }
}

// Interrupt service routine for button press
void SD() { 
    debounceTimeout.attach(&debounceCallback, 50ms);  // 50ms debounce delay
}

int main() {
    button.rise(&SD);    // Attach interrupt to rising edge (button press)
    
    while(1) {
        if(change == 1) {   // Button was pressed
            // Temporarily exit shutdown mode for one-shot conversion
            ConfigureTMP102SD0();
            
            unsigned short M;
            char L;
            
            // Point to temperature register and read data
            ConfigRegisterTMP102[0] = Temp_Reg;
            TMP102.write(TMP102Address, ConfigRegisterTMP102, 1);
            TMP102.read(TMP102Address, TemperatureRegister, 2);

            // Convert and display temperature
            M = TemperatureRegister[0] << 4;
            L = TemperatureRegister[1] >> 4;
            M = M + L;
            Temperature = 0.0625 * M;

            printf("Temperature = %u\n\n", M);
            printf("Temperature in Celsius = %.3f\n\n", Temperature);
            
            change = 0;  // Reset flag
        }
        else {
            // Return to shutdown mode to save power
            ConfigureTMP102SD1();
        }

        wait_us(1000000);  // 1 second delay
    }
}
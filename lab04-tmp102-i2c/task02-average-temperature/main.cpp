#include "mbed.h"

// Same pin and register definitions as Task 1
#define I2CSDA D0
#define I2CSCL D1
#define Temp_Reg 0x00
#define Config_Reg 0x01
#define TLow_Reg 0x02
#define THigh_Reg 0x03

I2C TMP102(I2CSDA, I2CSCL);
DigitalOut Temp_High(LED1);
DigitalOut Temp_Low(D12);

const int TMP102Address = 0x90;
char ConfigRegisterTMP102[3];
char TemperatureRegister[2];
float Temperature[10];      // Array to store 10 temperature readings
float TemperatureAdd;       // Sum of all temperature readings

// Configuration functions (same as Task 1)
void ConfigureTMP102() {
    ConfigRegisterTMP102[0] = Config_Reg;
    ConfigRegisterTMP102[1] = 0x60;
    ConfigRegisterTMP102[2] = 0xA0;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 3);
}

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

int main() {
    unsigned short M;
    char L;

    // Initialize sensor configuration
    ConfigureTMP102();
    Set_Thermostat_Temp_Low();
    Set_Thermostat_Temp_High();

    // Point to temperature register
    ConfigRegisterTMP102[0] = Temp_Reg;
    TMP102.write(TMP102Address, ConfigRegisterTMP102, 1);

    printf("Initiating Temperature Reading...\n\n");

    while(1) {
        TemperatureAdd = 0;  // Reset sum for new averaging cycle
        
        // Collect 10 temperature readings
        for (int i = 0; i < 10; i++) {
            // Read temperature data
            TMP102.read(TMP102Address, TemperatureRegister, 2);

            // Convert raw data to temperature
            M = TemperatureRegister[0] << 4;
            L = TemperatureRegister[1] >> 4;
            M = M + L;
            Temperature[i] = 0.0625 * M;
            
            // Add to running sum
            TemperatureAdd = Temperature[i] + TemperatureAdd;
            
            // Display individual reading
            printf("Temperature in Celsius = %.3f\n\n", Temperature[i]);
            wait_us(1000000);  // 1 second delay between readings
        }
        
        // Calculate and display average temperature
        printf("Average Temperature in Celsius = %.3f\n\n", TemperatureAdd/10);
    }
}
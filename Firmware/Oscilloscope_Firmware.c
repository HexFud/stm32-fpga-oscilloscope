#include <stdint.h>
#include <string.h>

#define SRAM_BASE_ADDRESS ((uint32_t)0x60000000) 
#define NUM_SAMPLES       1024

void HAL_Init(void) {}
void SystemClock_Config(void) {}
void MX_GPIO_Init(void) {}
void MX_SPI_Init(void) {}
void MX_FMC_Init(void) {}
void MX_LTDC_Init(void) {}
void MX_I2C_Init(void) {}

uint8_t waveform_buffer[NUM_SAMPLES];

typedef enum {
    STATE_INIT,
    STATE_ARM_FPGA,
    STATE_WAIT_TRIGGER,
    STATE_FETCH_DATA,
    STATE_PROCESS_AND_DRAW
} OscState;

OscState currentState = STATE_INIT;

void AFE_SetTriggerLevel(uint16_t millivolts) {
}

void AFE_SetGain(uint8_t scale_index) {
}

void FPGA_ArmTrigger(void) {
}

uint8_t FPGA_IsDataReady(void) {
    return 1;
}

void FetchDataFromSRAM(void) {
    memcpy(waveform_buffer, (uint8_t*)SRAM_BASE_ADDRESS, NUM_SAMPLES);
}

void LCD_DrawWaveform(uint8_t* data) {
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    
    MX_GPIO_Init();
    MX_SPI_Init();
    MX_FMC_Init();
    MX_LTDC_Init();
    MX_I2C_Init();

    AFE_SetGain(0);
    AFE_SetTriggerLevel(1500);

    while (1) {
        switch (currentState) {
            
            case STATE_INIT:
                currentState = STATE_ARM_FPGA;
                break;

            case STATE_ARM_FPGA:
                FPGA_ArmTrigger();
                currentState = STATE_WAIT_TRIGGER;
                break;

            case STATE_WAIT_TRIGGER:
                if (FPGA_IsDataReady()) {
                    currentState = STATE_FETCH_DATA;
                }
                break;

            case STATE_FETCH_DATA:
                FetchDataFromSRAM();
                currentState = STATE_PROCESS_AND_DRAW;
                break;

            case STATE_PROCESS_AND_DRAW:
                LCD_DrawWaveform(waveform_buffer);
                currentState = STATE_ARM_FPGA;
                break;
        }
    }
}

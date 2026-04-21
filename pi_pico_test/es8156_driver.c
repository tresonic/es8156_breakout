#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include <math.h>
#include "es8156.h"
#include "i2s.pio.h"

// I2C pins
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

// I2S pins for PIO
#define I2S_DATA_PIN 20
#define I2S_CLOCK_PIN_BASE 21 // 21 is Clock (BCLK), 22 is Word Select (LRCLK)

int main() {
    stdio_init_all();

    // Initialize I2C
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Initialize ES8156
    es8156_config_t esconfig = {
        .i2c_port = i2c0
    };

    printf("Initializing ES8156...\n");
    if (!es8156_init(&esconfig)) {
        printf("Failed to initialize ES8156!\n");
    } else {
        printf("ES8156 Initialized.\n");
        if (es8156_start(&esconfig)) {
            printf("ES8156 Started.\n");
        } else {
            printf("Failed to start ES8156.\n");
        }
    }

    // Initialize PIO for I2S output
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &i2s_program);
    i2s_program_init(pio, sm, offset, I2S_DATA_PIN, I2S_CLOCK_PIN_BASE);
    
    // Set clock divider for ~44.1kHz (adjust as needed based on sys clk)
    float div = (float)clock_get_hz(clk_sys) / (44100.0 * 32.0 * 2.0); // example for 32 bit clocks
    pio_sm_set_clkdiv(pio, sm, div);

    printf("I2S PIO configured.\n");

    uint32_t t = 0;
    while (true) {
        // Output audio samples here using pio_sm_put_blocking()
        int16_t sample = (int16_t)(32767.0f * sinf(2.0f * (float)M_PI * (float)t * 440.0f / 44100.0f));
        
        // Combine 16-bit left and right channels into a 32-bit word. 
        // MSB is sent first, so high 16 bits is left channel, low 16 bits is right channel.
        uint32_t sample32 = ((uint32_t)(uint16_t)sample << 16) | (uint16_t)sample;
        pio_sm_put_blocking(pio, sm, sample32);
        
        t++;
    }
}

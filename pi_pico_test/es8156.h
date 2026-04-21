#ifndef _ES8156_H_
#define _ES8156_H_

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// ES8156 I2C default address is either 0x08 or 0x09
// Usually, it's 0x08 when AD0 is GND, or 0x09 when AD0 is VDD
#define ES8156_ADDR 0x08

typedef struct {
    i2c_inst_t *i2c_port;
} es8156_config_t;

bool es8156_init(es8156_config_t *config);
bool es8156_start(es8156_config_t *config);
bool es8156_stop(es8156_config_t *config);
bool es8156_set_volume(es8156_config_t *config, uint8_t volume);

#endif // _ES8156_H_

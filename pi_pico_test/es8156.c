#include "es8156.h"
#include "es8156_reg.h"
#include <stdio.h>

static bool es8156_write_reg(es8156_config_t *config, uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    int ret = i2c_write_blocking(config->i2c_port, ES8156_ADDR, buf, 2, false);
    return ret == 2;
}

static bool es8156_read_reg(es8156_config_t *config, uint8_t reg, uint8_t *value) {
    int ret = i2c_write_blocking(config->i2c_port, ES8156_ADDR, &reg, 1, true);
    if (ret != 1) return false;
    ret = i2c_read_blocking(config->i2c_port, ES8156_ADDR, value, 1, false);
    return ret == 1;
}

bool es8156_init(es8156_config_t *config) {
    // Initial configuration
    bool ret = true;
    ret &= es8156_write_reg(config, ES8156_SCLK_MODE_REG02, 0x04);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS1_REG20, 0x2A);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS2_REG21, 0x3C);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS3_REG22, 0x00);
    ret &= es8156_write_reg(config, ES8156_ANALOG_LP_REG24, 0x07);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS4_REG23, 0x00);
    ret &= es8156_write_reg(config, ES8156_TIME_CONTROL1_REG0A, 0x01);
    ret &= es8156_write_reg(config, ES8156_TIME_CONTROL2_REG0B, 0x01);
    ret &= es8156_write_reg(config, ES8156_DAC_SDP_REG11, 0x00);
    ret &= es8156_write_reg(config, ES8156_VOLUME_CONTROL_REG14, 179); // volume 70%
    ret &= es8156_write_reg(config, ES8156_P2S_CONTROL_REG0D, 0x14); // I2S format
    ret &= es8156_write_reg(config, ES8156_MISC_CONTROL3_REG18, 0x00);
    ret &= es8156_write_reg(config, ES8156_CLOCK_ON_OFF_REG08, 0x3F);
    ret &= es8156_write_reg(config, ES8156_RESET_REG00, 0x02);
    ret &= es8156_write_reg(config, ES8156_RESET_REG00, 0x03);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS5_REG25, 0x20);
    return ret;
}

bool es8156_start(es8156_config_t *config) {
    bool ret = true;
    ret &= es8156_write_reg(config, ES8156_CLOCK_ON_OFF_REG08, 0x3F);
    ret &= es8156_write_reg(config, ES8156_MISC_CONTROL2_REG09, 0x00);
    ret &= es8156_write_reg(config, ES8156_MISC_CONTROL3_REG18, 0x00);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS5_REG25, 0x20);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS3_REG22, 0x00);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS2_REG21, 0x3C);
    ret &= es8156_write_reg(config, ES8156_EQ_CONTROL1_REG19, 0x20);
    return ret;
}

bool es8156_stop(es8156_config_t *config) {
    bool ret = true;
    ret &= es8156_write_reg(config, ES8156_VOLUME_CONTROL_REG14, 0x00);
    ret &= es8156_write_reg(config, ES8156_EQ_CONTROL1_REG19, 0x02);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS2_REG21, 0x1F);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS3_REG22, 0x02);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS5_REG25, 0x21);
    ret &= es8156_write_reg(config, ES8156_ANALOG_SYS5_REG25, 0xA1);
    ret &= es8156_write_reg(config, ES8156_MISC_CONTROL3_REG18, 0x01);
    ret &= es8156_write_reg(config, ES8156_MISC_CONTROL2_REG09, 0x02);
    ret &= es8156_write_reg(config, ES8156_MISC_CONTROL2_REG09, 0x01);
    ret &= es8156_write_reg(config, ES8156_CLOCK_ON_OFF_REG08, 0x00);
    return ret;
}

bool es8156_set_volume(es8156_config_t *config, uint8_t volume) {
    return es8156_write_reg(config, ES8156_VOLUME_CONTROL_REG14, volume);
}

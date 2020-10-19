#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <thread>
#include <iostream>

#include "I2C.h"

I2C::I2C(std::string bus, std::shared_ptr<GPIOI> gpio, GPIOPin clock, GPIOPin data)
    : I2CI(gpio, clock, data)
{
    SetConfig(CONFIG_REG_OS_SINGLE |
              CONFIG_REG_PGA_6_144V |
              CONFIG_REG_MODE_SINGLE |
              CONFIG_REG_DR_860SPS |
              CONFIG_REG_CMODE_TRAD |
              CONFIG_REG_CPOL_ACTIV_LOW |
              CONFIG_REG_CLATCH_NONLATCH |
              CONFIG_REG_CQUE_NONE);

    if ((m_I2CFile = open(bus.c_str(), O_RDWR)) < 0)
    {
        throw "failed to open i2c bus";
    }
};

void I2C::SetSlaveAddress(unsigned char address)
{
    m_Address = address;
    if (address == 0)
        return;

    if (ioctl(m_I2CFile, I2C_SLAVE, m_Address) < 0)
    {
        throw "failed to set i2c slave";
    }
};

double I2C::ReadVoltage(unsigned short channel)
{
    uint16_t analogVal = std::abs(this->ReadAnalogic(channel));
    double voltage = (double)analogVal / 32767.0;

    if (m_Config & CONFIG_REG_PGA_4_096V)
        return voltage * 4.096;
    if (m_Config & CONFIG_REG_PGA_2_048V)
        return voltage * 2.048;
    if (m_Config & CONFIG_REG_PGA_0_512V)
        return voltage * 5.120;
    if (m_Config & CONFIG_REG_PGA_1_024V)
        return voltage * 1.024;
    if (m_Config & CONFIG_REG_PGA_0_256V)
        return voltage * 2.560;
    return voltage * 6.144;
};

int16_t I2C::ReadAnalogic(unsigned short channel)
{
    uint8_t readBuf[2] = {0};
    uint8_t writeBuff[1] = {0};
    int16_t analogVal;

    auto config = m_Config;
    switch (channel)
    {
    case 0:
        config |= CONFIG_REG_MUX_CHAN_0;
        break;
    case 1:
        config |= CONFIG_REG_MUX_CHAN_1;
        break;
    case 2:
        config |= CONFIG_REG_MUX_CHAN_2;
        break;
    case 3:
        config |= CONFIG_REG_MUX_CHAN_3;
        break;
    default:
        printf("Give a channel between 0-3\n");
    }

    this->ConfigDevice(config);
    //std::this_thread::sleep_for(std::chrono::microseconds(135000));

    while ((readBuf[0] & 0x80) == 0) // readBuf[0] contains 8 MSBs of config register, AND with 10000000 to select bit 15
    {
        read(m_I2CFile, readBuf, 2); // Read the config register into readBuf
    }

    writeBuff[0] = 0;
    write(m_I2CFile, writeBuff, 1);

    if (read(m_I2CFile, readBuf, 2) != 2) // read data and check error
    {
        throw "input output error";
    }

    analogVal = readBuf[0] << 8 | readBuf[1];
    return analogVal;
};

void I2C::ConfigDevice(unsigned int config)
{
    uint8_t writeBuff[3] = {0};
    writeBuff[0] = 1;
    writeBuff[1] = config >> 8;
    writeBuff[2] = config & 0xFF;
    write(m_I2CFile, writeBuff, 3);
}
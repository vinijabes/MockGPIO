#pragma once
#include "I2CI.h"

class I2C : public I2CI
{
public:
    I2C(std::string bus, std::shared_ptr<GPIOI> gpio, GPIOPin clock, GPIOPin data);
    virtual ~I2C() {}

    virtual void SetSlaveAddress(unsigned char address) override;
    virtual double ReadVoltage(unsigned short channel) override;
    virtual int16_t ReadAnalogic(unsigned short channel) override;

private:
    void ConfigDevice(unsigned int config);

private:
    int m_I2CFile;
};
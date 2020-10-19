#pragma once

enum class GPIOMode {
    INPUT,
    OUTPUT,
    I2C = 7,
    UNKNOWN
};

enum class GPIOPin {
    GPIO2 = 8,
    GPIO3 = 9,
    GPIO4 = 7,
    GPIO5 = 21,
    GPIO6 = 22,
    GPIO7 = 11,
    GPIO8 = 10,
    GPIO9 = 13,
    GPIO10 = 12,
    GPIO11 = 14,
    GPIO12 = 26,
    GPIO13 = 23,
    GPIO14 = 15,
    GPIO15 = 16,
    GPIO16 = 27,
    GPIO17 = 0,
    GPIO18 = 1,
    GPIO19 = 24,
    GPIO20 = 28,
    GPIO21 = 29,
    GPIO22 = 3,
    GPIO23 = 4,
    GPIO24 = 10,
    GPIO25 = 6,
    GPIO26 = 25,
    GPIO27 = 2
};

class GPIOI 
{
public:
    static constexpr unsigned short HIGH = 1023;
    static constexpr unsigned short LOW = 0;

    virtual ~GPIOI() {}

    virtual void SetPinMode(GPIOPin pin, GPIOMode mode) = 0;

    virtual void DigitalWrite(GPIOPin pin, bool signal) = 0;
    virtual bool DigitalRead(GPIOPin pin) = 0;
    
    void Pulse(GPIOPin pin) 
    {
        this->DigitalWrite(pin, true);
        this->DigitalWrite(pin, false);
    }

    virtual void AnalogWrite(GPIOPin pin, short signal) = 0;
    virtual short AnalogRead(GPIOPin pin) = 0;
};
#pragma once
#include "GPIOI.h"
#include "wiringPi.h"

#undef INPUT
#undef OUTPUT

class GPIO : public GPIOI
{
public:
    GPIO() {
        wiringPiSetup();
    };

    virtual ~GPIO() {}

    virtual void SetPinMode(GPIOPin pin, GPIOMode mode) override {
        if(mode == GPIOMode::UNKNOWN || mode == GPIOMode::I2C) return;
        
        std::cout << "Setting pin " << (int)pin << " mode to " << (int)mode << std::endl;
        pinMode((int)pin, (int)mode);
    };

    virtual void DigitalWrite(GPIOPin pin, bool signal) override {
        digitalWrite((int)pin, signal);
    };

    virtual bool DigitalRead(GPIOPin pin) override {
        return digitalRead((int)pin);
    };

    virtual void AnalogWrite(GPIOPin pin, short signal) override {
        analogWrite((int)pin, signal);
    };

    virtual short AnalogRead(GPIOPin pin) override {
        return analogRead((int)pin);
    };
};
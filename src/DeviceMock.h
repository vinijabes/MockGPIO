#pragma once

#include <vector>
#include "MockGPIO.h"

class DeviceMock
{
public:
    DeviceMock(std::vector<GPIOPin> inputPins, std::vector<GPIOPin> outputPins)
        : m_InputPins(inputPins), m_OutputPins(outputPins)
    {
    }

    virtual void Loop() = 0;

protected:
    friend class MockGPIO;

    void SetController(MockGPIO *controller) { m_Controller = controller; }

    void DigitalWrite(GPIOPin pin, bool signal) { m_Controller->DigitalWriteDevice(pin, signal); }
    bool DigitalRead(GPIOPin pin) { return m_Controller->DigitalReadDevice(pin); }

    void AnalogWrite(GPIOPin pin, short signal) { m_Controller->AnalogWriteDevice(pin, signal); }
    short AnalogRead(GPIOPin pin) { return m_Controller->AnalogReadDevice(pin); }

protected:
    MockGPIO *m_Controller;

    std::vector<GPIOPin> m_InputPins;  //GPIO Input mode Pins
    std::vector<GPIOPin> m_OutputPins; //GPIO Output mode Pins
};
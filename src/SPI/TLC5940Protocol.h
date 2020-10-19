#pragma once

#include "Portugues/GPIO/SPI/SPI.h"

class TLC5940Protocol : public SPICommunicationProtocolInterface<uint16_t>
{
public:
    TLC5940Protocol(std::shared_ptr<GPIOI> gpio, GPIOPin xlatPin, GPIOPin vprgPin)
        : SPICommunicationProtocolInterface<uint16_t>(gpio), m_XLatPin(xlatPin), m_VPRGPin(vprgPin)
    {
        m_GPIO->SetPinMode(xlatPin, GPIOMode::OUTPUT);
        m_GPIO->SetPinMode(vprgPin, GPIOMode::OUTPUT);

        m_GPIO->DigitalWrite(vprgPin, true);
        m_GPIO->DigitalWrite(xlatPin, true);

        this->m_FirstCycle = true;
    }

    virtual void OnUpdateStart() override
    {
        if (this->m_FirstCycle)
        {
            m_GPIO->DigitalWrite(m_VPRGPin, false);
        }
    };

    virtual void OnUpdateFinish() override
    {
        m_GPIO->Pulse(m_XLatPin);
        if (this->m_FirstCycle)
        {
            m_FirstCycle = false;
        }
    };

protected:
    GPIOPin m_XLatPin;
    GPIOPin m_VPRGPin;

    bool m_FirstCycle;
};
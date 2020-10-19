#pragma once
#include <array>
#include <memory>

#include "Portugues/GPIO/GPIOI.h"

template <class T>
class SPICommunicationProtocolInterface
{
public:
    SPICommunicationProtocolInterface(std::shared_ptr<GPIOI> gpio)
        : m_GPIO(gpio)
    {
    }

    virtual void OnUpdateStart(){};
    virtual void OnUpdateFinish(){};

    virtual void BeforeBitSend(){};
    virtual void AfterBitSend(){};

protected:
    std::shared_ptr<GPIOI> m_GPIO;
};

template <unsigned int DeviceCount, unsigned int Channels, unsigned int BitCount, class T>
class SPI
{
public:
    SPI(
        std::shared_ptr<GPIOI> gpio,
        std::shared_ptr<SPICommunicationProtocolInterface<T>> protocol,
        GPIOPin sclkPin,
        GPIOPin mosiPin)
        : m_GPIO(gpio), m_Protocol(protocol), m_SCLKPin(sclkPin), m_MOSIPin(mosiPin)
    {
        m_GPIO->SetPinMode(sclkPin, GPIOMode::OUTPUT);
        m_GPIO->SetPinMode(mosiPin, GPIOMode::OUTPUT);
        m_GPIO->SetPinMode(GPIOPin::GPIO17, GPIOMode::OUTPUT);

        m_GPIO->DigitalWrite(sclkPin, false);
        m_GPIO->DigitalWrite(mosiPin, false);

        this->m_Channels.fill(0);
    }

    inline void SetChannel(unsigned int channel, T data) { m_Channels[channel] = data; }
    inline void SetAll(T data) 
    {
        for(size_t i = 0; i < Channels*DeviceCount; ++i) 
        {
            m_Channels[i] = data;
        }
    }

    void Update()
    {
        m_Protocol->OnUpdateStart();

        auto channelCounter = Channels * DeviceCount - 1;
        while (channelCounter)
        {
            for (int i = BitCount; i >= 0; i--)
            {
                m_Protocol->BeforeBitSend();

                bool value = this->GetBitValue(m_Channels[channelCounter], i);
                this->m_GPIO->DigitalWrite(m_MOSIPin, value);
                this->m_GPIO->DigitalWrite(GPIOPin::GPIO17, value);
                this->m_GPIO->Pulse(m_SCLKPin);

                m_Protocol->AfterBitSend();
            }

            --channelCounter;
        }

        m_GPIO->DigitalWrite(m_MOSIPin, false);
        m_GPIO->DigitalWrite(GPIOPin::GPIO17, false);
        m_Protocol->OnUpdateFinish();
    }

private:
    inline bool GetBitValue(T data, unsigned int pos)
    {
        return ((data & (1 << pos)) >> pos);
    }

protected:
    std::array<T, DeviceCount * Channels> m_Channels;
    std::shared_ptr<GPIOI> m_GPIO;
    std::shared_ptr<SPICommunicationProtocolInterface<T>> m_Protocol;

    GPIOPin m_SCLKPin;
    GPIOPin m_MOSIPin;
};
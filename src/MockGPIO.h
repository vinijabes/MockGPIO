#pragma once
#include "Pin.h"

#include <vector>
#include <atomic>
#include <mutex> // For std::unique_lock
#include <shared_mutex>

class DeviceMock;
class MockGPIO : public GPIOI
{
public:
    MockGPIO(int pinCount);
    virtual ~MockGPIO() {}

    virtual void SetPinMode(GPIOPin pin, GPIOMode mode) override;

    virtual void DigitalWrite(GPIOPin pin, bool signal) override;
    virtual bool DigitalRead(GPIOPin pin) override;

    virtual void AnalogWrite(GPIOPin pin, short signal) override;
    virtual short AnalogRead(GPIOPin pin) override;

    void AddDevice(std::shared_ptr<DeviceMock> device);

    void Start(unsigned int clock);
    void Stop();

protected:
    friend class DeviceMock;

    void DigitalWriteDevice(GPIOPin pin, bool signal);
    bool DigitalReadDevice(GPIOPin pin);

    void AnalogWriteDevice(GPIOPin pin, short signal);
    short AnalogReadDevice(GPIOPin pin);

private:
    Pin *GetPin(GPIOPin pin)
    {
        return &m_Pins[(int)pin];
    }

private:
    std::vector<Pin> m_Pins;
    std::vector<std::shared_ptr<DeviceMock>> m_Devices;

    std::shared_mutex m_Mutex;

    std::atomic<bool> m_Running;
};
#include "MockGPIO.h"
#include "DeviceMock.h"

#include <thread>

MockGPIO::MockGPIO(int pinCount)
{
    m_Pins.reserve(pinCount);

    for (int i = 0; i < pinCount; ++i)
    {
        m_Pins.push_back(Pin(i + 1));
    }
}

void MockGPIO::SetPinMode(GPIOPin pin, GPIOMode mode)
{
    auto p = this->GetPin(pin);

    std::lock_guard<std::shared_mutex> g(this->m_Mutex);
    p->mode = mode;
}

void MockGPIO::DigitalWrite(GPIOPin pin, bool signal)
{
    auto p = this->GetPin(pin);

    if (p->mode == GPIOMode::UNKNOWN || p->mode == GPIOMode::INPUT)
    {
        throw "pin mode is not in output mode";
    }

    std::lock_guard<std::shared_mutex> g(this->m_Mutex);
    if (signal)
    {
        p->value = GPIOI::HIGH;
    }
    else
    {
        p->value = GPIOI::LOW;
    }
}

bool MockGPIO::DigitalRead(GPIOPin pin)
{
    auto p = this->GetPin(pin);

    if (p->mode == GPIOMode::UNKNOWN || p->mode == GPIOMode::OUTPUT)
    {
        throw "pin mode is not in input mode";
    }

    std::shared_lock<std::shared_mutex> g(this->m_Mutex);
    return p->value > 279;
}

void MockGPIO::AnalogWrite(GPIOPin pin, short signal)
{
    auto p = this->GetPin(pin);

    if (p->mode == GPIOMode::UNKNOWN || p->mode == GPIOMode::INPUT)
    {
        throw "pin mode is not in output mode";
    }

    std::lock_guard<std::shared_mutex> g(this->m_Mutex);
    p->value = signal;
}

short MockGPIO::AnalogRead(GPIOPin pin)
{
    auto p = this->GetPin(pin);

    if (p->mode == GPIOMode::UNKNOWN || p->mode == GPIOMode::OUTPUT)
    {
        throw "pin mode is not in input mode";
    }

    std::shared_lock<std::shared_mutex> g(this->m_Mutex);
    return p->value;
}

void MockGPIO::DigitalWriteDevice(GPIOPin pin, bool signal)
{
    auto p = this->GetPin(pin);

    if (p->mode == GPIOMode::UNKNOWN || p->mode == GPIOMode::OUTPUT)
    {
        throw "pin mode is not in input mode";
    }

    std::lock_guard<std::shared_mutex> g(this->m_Mutex);
    if (signal)
    {
        p->value = GPIOI::HIGH;
    }
    else
    {
        p->value = GPIOI::LOW;
    }
}

bool MockGPIO::DigitalReadDevice(GPIOPin pin)
{
    auto p = this->GetPin(pin);

    if (p->mode == GPIOMode::UNKNOWN || p->mode == GPIOMode::INPUT)
    {
        throw "pin mode is not in output mode";
    }

    std::shared_lock<std::shared_mutex> g(this->m_Mutex);
    return p->value > 279;
}

void MockGPIO::AnalogWriteDevice(GPIOPin pin, short signal)
{
    auto p = this->GetPin(pin);

    if (p->mode == GPIOMode::UNKNOWN || p->mode == GPIOMode::OUTPUT)
    {
        throw "pin mode is not in input mode";
    }

    std::lock_guard<std::shared_mutex> g(this->m_Mutex);
    p->value = signal;
}

short MockGPIO::AnalogReadDevice(GPIOPin pin)
{
    auto p = this->GetPin(pin);

    if (p->mode == GPIOMode::UNKNOWN || p->mode == GPIOMode::INPUT)
    {
        throw "pin mode is not in output mode";
    }

    std::shared_lock<std::shared_mutex> g(this->m_Mutex);
    return p->value;
}

void MockGPIO::AddDevice(std::shared_ptr<DeviceMock> device)
{
    device->SetController(this);
    this->m_Devices.push_back(device);

    for (GPIOPin p : device->m_InputPins)
    {
        this->SetPinMode(p, GPIOMode::INPUT);
    }

    for (GPIOPin p : device->m_OutputPins)
    {
        this->SetPinMode(p, GPIOMode::OUTPUT);
    }
}

void MockGPIO::Start(unsigned int clock)
{
    m_Running = true;

    while (m_Running)
    {
        for (std::shared_ptr<DeviceMock> device : this->m_Devices)
        {
            device->Loop();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void MockGPIO::Stop()
{
    m_Running = false;
}
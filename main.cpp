#include <iostream>
#include <thread>
#include "src/GPIOI.h"
#include "src/MockGPIO.h"
#include "src/DeviceMock.h"

class TestDevice : public DeviceMock
{
public:
    TestDevice(std::vector<GPIOPin> inputPins, std::vector<GPIOPin> outputPins)
        : DeviceMock(inputPins, outputPins)
    {
    }

    virtual void Loop()
    {
        if (!this->DigitalRead(m_OutputPins[0]))
        {
            this->AnalogWrite(m_InputPins[0], m_Counter++);
        }
        else
        {
            this->AnalogWrite(m_InputPins[0], m_Counter--);
        }
    }

private:
    int m_Counter = 0;
};


int main()
{
    try
    {
        MockGPIO gpio(40);

        std::vector<GPIOPin> inputPins = {GPIOPin::GPIO4};
        std::vector<GPIOPin> outputPins = {GPIOPin::GPIO5};

        gpio.AddDevice(std::make_shared<TestDevice>(inputPins, outputPins));

        std::thread t([&]() {
            while (true)
            {
                std::cout << gpio.AnalogRead(GPIOPin::GPIO4) << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                if (gpio.AnalogRead(GPIOPin::GPIO4) >= 10)
                {
                    gpio.DigitalWrite(GPIOPin::GPIO4, GPIOI::HIGH);
                }
                else if (gpio.AnalogRead(GPIOPin::GPIO4) <= 0)
                {
                    gpio.DigitalWrite(GPIOPin::GPIO5, GPIOI::LOW);
                }
            }
        });

        gpio.Start(1000);
        gpio.Stop();
    }
    catch (const char *err)
    {
        std::cout << err << std::endl;
    }
}
#pragma once
#include "GPIOI.h"

struct Pin
{
    int number;
    unsigned short value;
    GPIOMode mode;

    Pin(int number)
        : number(number), value(0), mode(GPIOMode::UNKNOWN)
    {
    }
};
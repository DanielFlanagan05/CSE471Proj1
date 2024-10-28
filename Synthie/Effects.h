#pragma once
#include <vector>
#include "Effects.h"

class Effects
{
public:
    virtual ~Effects() = default;
    virtual void Process(double* input, double* output) = 0;
};

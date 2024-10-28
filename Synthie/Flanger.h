#pragma once
#include "Effects.h"
class Flanger :
    public Effects
{
public:

    Flanger();
    virtual ~Flanger();

    void Process(double* input, double* output) override;
    void SetParameters(double delay, double depth, double rate, double feedback);

private:
    double m_delay;
    double m_depth;
    double m_rate;
    double m_feedback;
    double m_phase;
    std::vector<double> m_buffer;
    int m_bufferSize;
    int m_writeIndex;


};


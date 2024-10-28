#include "stdafx.h"
#include "Chorus.h"
#include <cmath>
#include <cstring>

Chorus::Chorus() : m_depth(0.002), m_rate(1.0), m_mix(0.5), m_phase(0.0)
{

}

Chorus::~Chorus()
{
}

void Chorus::SetParameters(double delay, double range, double rate)
{
    
}

void Chorus::Process(double* inputFrame, double* outputFrame)
{
    double delay = m_depth * sin(2.0 * PI * m_phase);
    m_phase += m_rate / 44100.0; // Assuming 44100 sample rate
    if (m_phase >= 1.0)
    {
        m_phase -= 1.0;
    }

    for (int c = 0; c < 2; ++c)
    {
        double delayedSample = inputFrame[c] + delay; // Simple delay for demonstration
        outputFrame[c] = inputFrame[c] * (1.0 - m_mix) + delayedSample * m_mix;
    }
}

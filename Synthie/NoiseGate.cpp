#include "stdafx.h"
#include "NoiseGate.h"

NoiseGate::NoiseGate() : m_threshold(0.01), m_attack(0.01), m_release(0.1),
m_hold(0.05), m_range(0.0), m_envelope(0.0), m_holdCounter(0.0), m_gateOpen(false)

{

}

NoiseGate::~NoiseGate()
{
}

void NoiseGate::Process(double* inputFrame, double* outputFrame)
{
    double inputLevel = fabs(inputFrame[0]); // Assuming mono input for simplicity

    if (inputLevel > m_threshold)
    {
        m_gateOpen = true;
        m_holdCounter = m_hold;
    }
    else if (m_holdCounter > 0)
    {
        m_holdCounter -= 1.0 / 44100.0; // Assuming 44100 sample rate
    }
    else
    {
        m_gateOpen = false;
    }
    if (m_gateOpen)
    {
        m_envelope += (1.0 - m_envelope) * m_attack;
    }
    else
    {
        m_envelope += (0.0 - m_envelope) * m_release;
    }

    for (int c = 0; c < 2; ++c)
    {
        outputFrame[c] = inputFrame[c] * (m_envelope * (1.0 - m_range) + m_range);
    }
}
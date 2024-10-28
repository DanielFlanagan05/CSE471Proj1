#include "stdafx.h"
#include "Reverberation.h"
#include <cstring>

Reverberation::Reverberation() : m_reverbTime(1.0), m_mix(0.5), 
m_sampleRate(44100.0), m_writeIndex(0)
{
    m_bufferSize = static_cast<int>(m_sampleRate * m_reverbTime);
    m_buffer.resize(m_bufferSize, 0.0);
}

Reverberation::~Reverberation()
{
}       
void Reverberation::Process(double* inputFrame, double* outputFrame)
{
    for (int c = 0; c < 2; ++c)
    {
        double delayedSample = m_buffer[m_writeIndex];
        m_buffer[m_writeIndex] = inputFrame[c] + delayedSample * 0.5; // Simple feedback for demonstration

        outputFrame[c] = inputFrame[c] * (1.0 - m_mix) + delayedSample * m_mix;
    }

    m_writeIndex = (m_writeIndex + 1) % m_bufferSize;
}
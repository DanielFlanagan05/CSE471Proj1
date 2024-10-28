#include "stdafx.h"
#include "Flanger.h"
#include <cmath>
#include <vector>

Flanger::Flanger() : m_delay(0.0), m_depth(0.0), m_rate(0.0), 
m_feedback(0.0), m_phase(0.0), m_writeIndex(0)
{
    m_bufferSize = 44100; // 1 second buffer at 44.1kHz sample rate
    m_buffer.resize(m_bufferSize, 0.0);
}
Flanger::~Flanger()
{
}
void Flanger::SetParameters(double delay, double depth, double rate, double feedback)
{
    m_delay = delay;
    m_depth = depth;
    m_rate = rate;
    m_feedback = feedback;
}
void Flanger::Process(double* inputFrame, double* outputFrame)
{
    double lfoValue = (std::sin(2.0 * PI * m_phase) + 1.0) / 2.0; // LFO oscillates between 0 and 1
    double currentDelay = m_delay + m_depth * lfoValue;

    // Calculate the read index
    int readIndex = m_writeIndex - static_cast<int>(currentDelay * 44100.0 / 1000.0); // Convert ms to samples
    if (readIndex < 0) {
        readIndex += m_bufferSize;
    }

    // Get the delayed sample
    double delayedSample = m_buffer[readIndex];

    // Apply feedback
    double processedSample = *inputFrame + delayedSample * m_feedback;

    // Store the current sample in the delay buffer
    m_buffer[m_writeIndex] = *inputFrame + delayedSample * m_feedback;

    // Increment and wrap the write index
    m_writeIndex = (m_writeIndex + 1) % m_bufferSize;

    // Increment and wrap the LFO phase
    m_phase += m_rate / 44100.0; // Assuming a sample rate of 44100 Hz
    if (m_phase >= 1.0) {
        m_phase -= 1.0;
    }

    // Output the processed sample
    *outputFrame = processedSample;

}

#include "stdafx.h"
#include "ADSR.h"

CADSR::CADSR()
{
    m_attack = 0.05;
    m_decay = 0.05;
    m_sustainLevel = 0.8;
    m_release = 0.05;
    m_duration = 1.0; // Default duration
}

CADSR::~CADSR() {}

void CADSR::SetADSR(double attack, double decay, double sustainLevel, double release)
{
    m_attack = attack;
    m_decay = decay;
    m_sustainLevel = sustainLevel;
    m_release = release;
}

void CADSR::SetDuration(double duration)
{
    m_duration = duration;
}

void CADSR::Start()
{
    // Any initialization logic goes here if needed
}

bool CADSR::Generate(double time, double& envelope)
{
    double totalADSRTime = m_attack + m_decay + m_release;
    double sustainTime = m_duration - totalADSRTime;

    if (time < m_attack)
    {
        // Attack phase
        envelope = (time / m_attack);
    }
    else if (time < (m_attack + m_decay))
    {
        // Decay phase
        double t = time - m_attack;
        envelope = 1.0 - (1.0 - m_sustainLevel) * (t / m_decay);
    }
    else if (time < (m_duration - m_release))
    {
        // Sustain phase
        envelope = m_sustainLevel;
    }
    else if (time < m_duration)
    {
        // Release phase
        double t = time - (m_duration - m_release);
        envelope = m_sustainLevel * (1.0 - (t / m_release));
    }
    else
    {
        envelope = 0.0;
    }

    return time < m_duration;
}

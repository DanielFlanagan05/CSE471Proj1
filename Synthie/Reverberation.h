#pragma once
#include "Effects.h"
class Reverberation :
    public Effects
{
public:
    Reverberation();
    virtual ~Reverberation();

    void Process(double* input, double* output) override;
    void SetReverbTime(double reverbTime) { m_reverbTime = reverbTime; }

    void SetMix(double mix) { m_mix = mix; }

private:
    double m_reverbTime;
    double m_mix;
    double m_sampleRate;
    std::vector<double> m_buffer;
    int m_bufferSize;
    int m_writeIndex;

};


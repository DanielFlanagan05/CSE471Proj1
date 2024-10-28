#pragma once
#include "Effects.h"
class Chorus :
    public Effects
{
public:
    Chorus();
    virtual ~Chorus();

    void Process(double* input, double* output) override;
    void SetParameters(double delay, double range, double rate);

    void SetDepth(double depth) { m_depth = depth; }
    void SetRate(double rate) { m_rate = rate; }
    void SetMix(double mix) { m_mix = mix; }

private:
    double m_depth;
    double m_rate;
    double m_mix;
    double m_phase;
};


#pragma once
#include "Effects.h"
class NoiseGate :
    public Effects
{
public:
    NoiseGate();
    virtual ~NoiseGate();

    void Process(double* input, double* output) override;

    void SetThreshold(double threshold) {m_threshold = threshold;}

    void SetAttack(double attack) { m_attack = attack; }
    void SetRelease(double release) { m_release = release; }
    void SetHold(double hold) { m_hold = hold; }
    void SetRange(double range) { m_range = range; }


private:
    double m_threshold;
    double m_attack;
    double m_release;
    double m_hold;
    double m_range;
    double m_envelope;
    double m_holdCounter;
    bool m_gateOpen;
   
};


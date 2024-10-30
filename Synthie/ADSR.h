#pragma once

class CADSR
{
public:
    CADSR();
    ~CADSR();

    void SetADSR(double attack, double decay, double sustainLevel, double release);
    void SetDuration(double duration);
    void Start();
    bool Generate(double time, double& envelope);

private:
    double m_attack;
    double m_decay;
    double m_sustainLevel;
    double m_release;
    double m_duration;
};


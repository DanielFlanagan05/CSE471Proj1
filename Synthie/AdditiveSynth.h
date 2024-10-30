#pragma once
#include "AudioNode.h"
#include <vector>
#include "Instrument.h"
#include "SineWave.h"

class CAdditiveSynth : public CInstrument
{
public:
    CAdditiveSynth();
    CAdditiveSynth(double bpm);

    virtual ~CAdditiveSynth();

    virtual void Start() override;
    virtual bool Generate() override;
    std::vector<double> ParseHarmonics(const std::wstring& harmonicsStr);
    virtual void SetNote(CNote* note) override;


    void SetHarmonics(const std::vector<double>& harmonics);
    void SetFreq(double f) { m_freq = f; }


private:
    double m_freq;
    std::vector<CSineWave> m_sinewaves;
    double m_duration;
    double m_time; 
    double m_bpm;      

    // Used for glissando
    double m_startFreq;
    double m_endFreq;

	// Used for ASDR envelope
    double m_attack;
    double m_decay;
    double m_sustainLevel;
    double m_release;

    // Used for sound cross-fading
    std::vector<double> m_startHarmonics;
    std::vector<double> m_endHarmonics;
};
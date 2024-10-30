#pragma once
#include "Instrument.h"
#include <vector>

class CWavetableInstrument : public CInstrument {
public:
    CWavetableInstrument();
    virtual ~CWavetableInstrument();

    void Start() override;           
    bool Generate() override;        // Generate the next audio frame

    void SetAttackWaveform(const std::vector<double>& waveform);   // Set attack wave
    void SetSustainWaveform(const std::vector<double>& waveform);  // Set sustain wave
    void SetNote(CNote* note) override;    // Set note and frequency
    void SetPitch(double pitch);           // Adjust pitch

private:
    // Waveforms for attack and sustain
    std::vector<double> m_attackWaveform;
    std::vector<double> m_sustainWaveform;

    double m_sampleRate;  // Sample rate of the instrument
    double m_frequency;   // Frequency of the note
    double m_pitch;       // Pitch adjustment factor
    double m_time;        // Current time in playback
    int m_waveIndex;      // Current position in the waveform

    // ADSR envelope parameters
    double m_attackTime;
    double m_sustainLevel;
    double m_releaseTime;
    double m_duration;  // Total duration of the note
    bool m_releasing;   // Whether the note is in release phase

    double m_crossfadeAmount;  // Cross-fading between adjacent samples

    double GetNextSample();       
    double GenerateEnvelope();    
};

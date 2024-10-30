#include "stdafx.h"
#include "WavetableInstrument.h"
#include <cmath>
#include "Notes.h"

// Constructor initializing member variables
CWavetableInstrument::CWavetableInstrument()
    : m_sampleRate(44100), m_frequency(440), m_pitch(1.0), m_time(0),
      m_waveIndex(0), m_attackTime(0.1), m_sustainLevel(0.7), 
      m_releaseTime(0.2), m_duration(1.0), m_releasing(false),
      m_crossfadeAmount(0.5) {}

CWavetableInstrument::~CWavetableInstrument() {}

void CWavetableInstrument::SetAttackWaveform(const std::vector<double>& waveform) {
    m_attackWaveform = waveform;
}

void CWavetableInstrument::SetSustainWaveform(const std::vector<double>& waveform) {
    m_sustainWaveform = waveform;
}

void CWavetableInstrument::SetNote(CNote* note) {
    m_frequency = NoteToFrequency(note->GetName().c_str());
    m_time = 0;           // Reset time
    m_waveIndex = 0;       // Reset wave index
    m_releasing = false;   // Reset release phase
}

void CWavetableInstrument::SetPitch(double pitch) {
    m_pitch = pitch;  // Set the pitch adjustment factor
}

void CWavetableInstrument::Start() {
    m_time = 0;
    m_waveIndex = 0;   // Reset the wave index for each note
    m_releasing = false;
}

bool CWavetableInstrument::Generate() {
    double sample = GetNextSample();  // Retrieve the next sample
    double envelope = GenerateEnvelope();  // Apply ADSR envelope

    // Set stereo output with the sample and envelope applied
    m_frame[0] = sample * envelope;
    m_frame[1] = sample * envelope;

    // Advance time for the instrument
    m_time += GetSamplePeriod();

    // Determine if the note is still playing
    return m_time < (m_duration + m_releaseTime);
}

double CWavetableInstrument::GetNextSample() {
    double indexStep = m_frequency * m_pitch * m_attackWaveform.size() / m_sampleRate;

    double sample;
    if (m_time < m_attackTime) {
        // Use attack waveform during the attack phase
        sample = m_attackWaveform[static_cast<int>(m_waveIndex) % m_attackWaveform.size()];
    } else {
        // Use sustain waveform after the attack phase
        sample = m_sustainWaveform[static_cast<int>(m_waveIndex) % m_sustainWaveform.size()];
    }

    // Perform cross-fading between adjacent samples for smooth transitions
    double sampleA = sample;
    double sampleB = m_attackWaveform[(static_cast<int>(m_waveIndex) + 1) % m_attackWaveform.size()];
    double crossfade = (1.0 - m_crossfadeAmount) * sampleA + m_crossfadeAmount * sampleB;

    // Advance the wave index
    m_waveIndex += indexStep;

    return crossfade;
}

double CWavetableInstrument::GenerateEnvelope() {
    if (m_time < m_attackTime) {
        // Attack phase: Linearly increase the amplitude
        return m_time / m_attackTime;
    } else if (!m_releasing) {
        // Sustain phase: Hold the amplitude constant
        return m_sustainLevel;
    } else {
        // Release phase: Gradually reduce the amplitude
        double releaseProgress = (m_time - m_duration) / m_releaseTime;
        return m_sustainLevel * (1.0 - releaseProgress);
    }
}

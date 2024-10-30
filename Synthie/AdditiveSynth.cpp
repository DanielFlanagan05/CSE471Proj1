#include "stdafx.h"
#include "AdditiveSynth.h"
#include "Notes.h"
#include <sstream>

CAdditiveSynth::CAdditiveSynth()
{
	m_freq = 440.0;
}

CAdditiveSynth::CAdditiveSynth(double bpm)
{
    m_freq = 440.0;
    m_bpm = bpm;
}

CAdditiveSynth::~CAdditiveSynth() {}

void CAdditiveSynth::Start()
{
    m_time = 0; // Reset time at the start
    m_sinewaves.clear();
}



bool CAdditiveSynth::Generate()
{
    m_frame[0] = 0;
    m_frame[1] = 0;

    // Compute the envelope 
    double envelope = 1.0;
    double totalADSRTime = m_attack + m_decay + m_release;
    double sustainTime = m_duration - totalADSRTime;

    if (m_time < m_attack)
    {
        // Attack phase
        envelope = (m_time / m_attack);
    }
    else if (m_time < (m_attack + m_decay))
    {
        // Decay phase
        double t = m_time - m_attack;
        envelope = 1.0 - (1.0 - m_sustainLevel) * (t / m_decay);
    }
    else if (m_time < (m_duration - m_release))
    {
        // Sustain phase
        envelope = m_sustainLevel;
    }
    else if (m_time < m_duration)
    {
        // Release phase
        double t = m_time - (m_duration - m_release);
        envelope = m_sustainLevel * (1.0 - (t / m_release));
    }
    else
    {
        envelope = 0.0;
    }

	// Used in place of time / duration in case duration is 0
    double progress = (m_duration > 0.0) ? (m_time / m_duration) : 1.0;


    // Compute current frequency for glissando
    double freq = m_startFreq + (m_endFreq - m_startFreq) * progress;

    // Interpolate harmonic amplitudes for cross-fading
    size_t maxHarmonics = (m_startHarmonics.size() > m_endHarmonics.size()) ? m_startHarmonics.size() : m_endHarmonics.size();

    for (size_t i = 0; i < maxHarmonics; i++)
    {
        double startAmp = (i < m_startHarmonics.size()) ? m_startHarmonics[i] : 0.0;
        double endAmp = (i < m_endHarmonics.size()) ? m_endHarmonics[i] : 0.0;
        double amp = startAmp + (endAmp - startAmp) * progress;

        // Ensure the sinewaves vector has enough sine waves
        if (i >= m_sinewaves.size())
        {
            CSineWave sine;
            sine.SetSampleRate(GetSampleRate());
            sine.Start();
            m_sinewaves.push_back(sine);
        }

        // Set the frequency and amplitude for this harmonic
        m_sinewaves[i].SetFreq(freq * (i + 1)); // Multiply by (i + 1) for harmonics
        m_sinewaves[i].SetAmplitude(amp);

        // Generate the sine wave and add it to the frame
        m_sinewaves[i].Generate();
        m_frame[0] += m_sinewaves[i].Frame(0);
        m_frame[1] += m_sinewaves[i].Frame(1);
    }

    // Apply the envelope to the accumulated frame (if not applied earlier)
    m_frame[0] *= envelope;
    m_frame[1] *= envelope;

    // Update time and check if the note duration has been exceeded
    m_time += GetSamplePeriod();
    return m_time < m_duration;
}

// Method to parse harmonics from a string
std::vector<double> CAdditiveSynth::ParseHarmonics(const std::wstring& harmonicsStr)
{
    std::vector<double> harmonics;
    std::wstringstream ss(harmonicsStr);
    double harmonic;
    while (ss >> harmonic)
    {
        harmonics.push_back(harmonic);
    }
    return harmonics;
}


void CAdditiveSynth::SetNote(CNote* note)
{
    // Get attributes
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    note->Node()->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    m_startHarmonics.clear();
    m_endHarmonics.clear();

    // Default ADSR values
    m_attack = 0.05;
    m_decay = 0.05;
    m_sustainLevel = 0.8;
    m_release = 0.05;

    // Default frequencies
    m_startFreq = 440.0;
    m_endFreq = 440.0;

    // Loop over the list of attributes
    for (int i = 0; i < len; i++)
    {
        // Get attribute i
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        // Get the name and value of the attribute
        CComBSTR name;
        attrib->get_nodeName(&name);
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == L"freq" || name == L"startFreq")
        {
            value.ChangeType(VT_R8);
            m_startFreq = value.dblVal;
            m_endFreq = m_startFreq; // Default end frequency
        }
        else if (name == L"endFreq" || name == L"glissandoFreq")
        {
            value.ChangeType(VT_R8);
            m_endFreq = value.dblVal;
        }
        else if (name == L"note")
        {
            m_startFreq = NoteToFrequency(value.bstrVal);
            m_endFreq = m_startFreq;
        }
        else if (name == L"glissandoNote")
        {
            m_endFreq = NoteToFrequency(value.bstrVal);
        }
        else if (name == L"harmonics" || name == L"startHarmonics")
        {
            m_startHarmonics = ParseHarmonics(value.bstrVal);
            m_endHarmonics = m_startHarmonics;
        }
        else if (name == L"endHarmonics")
        {
            m_endHarmonics = ParseHarmonics(value.bstrVal);
        }
        else if (name == L"duration")
        {
            value.ChangeType(VT_R8);
            m_duration = value.dblVal * (60.0 / m_bpm);
        }
        else if (name == L"attack")
        {
            value.ChangeType(VT_R8);
            m_attack = value.dblVal;
        }
        else if (name == L"decay")
        {
            value.ChangeType(VT_R8);
            m_decay = value.dblVal;
        }
        else if (name == L"sustainLevel")
        {
            value.ChangeType(VT_R8);
            m_sustainLevel = value.dblVal;
        }
        else if (name == L"release")
        {
            value.ChangeType(VT_R8);
            m_release = value.dblVal;
        }
    }
}




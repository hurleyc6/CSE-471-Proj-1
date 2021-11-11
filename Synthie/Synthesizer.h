#pragma once
#include <list>
#include <string>
#include <vector>
#include <array>
#include "Instrument.h"
#include "Note.h"
#include "NoiseGating.h"
#include "Limiting.h"
#include "Chorus.h"
#include "Flange.h"
#include "msxml2.h"

class CSynthesizer
{
public:

    CSynthesizer(void);

    //! Number of audio channels
    int GetNumChannels() { return m_channels; }

    //! Sample rate in samples per second
    double GetSampleRate() { return m_sampleRate; }

    //! Sample period in seconds (1/samplerate)
    double GetSamplePeriod() { return m_samplePeriod; }

    //! Set the number of channels
    void SetNumChannels(int n) { m_channels = n; }

    //! Set the sample rate
    void SetSampleRate(double s) { m_sampleRate = s;  m_samplePeriod = 1.0 / s; }

    //! Start the synthesizer
    void Start();

    //! Generate one audio frame
    bool Generate(double*);

    //! Get the time since we started generating audio
    double GetTime() { return m_time; }

    void Clear(void);

    void OpenScore(CString& filename);

    //! Beats per minute 
    //double GetBPM() { return m_bpm; }

private:

    void XmlLoadNote(IXMLDOMNode* xml, std::wstring& instrument);

    void XmlLoadScore(IXMLDOMNode* xml);

    void XmlLoadInstrument(IXMLDOMNode* xml);

    void LoadWaveSamples();

    int	m_channels;

    double m_sampleRate;

    double m_samplePeriod;

    double m_time;

    double m_bpm;               //!< Beats per minute

    int m_beatspermeasure;      //!< Beats per measure

    double m_secperbeat;        //!< Seconds per beat

    int m_currentNote;          //!< The current note we are playing

    int m_measure;              //!< The current measure

    double m_beat;              //!< The current beat within the measure

    bool m_NG;

    bool m_LT;

    bool m_CS;

    bool m_FL;

    std::list<CInstrument*>  m_instruments;

    std::vector<CNote> m_notes;

    CNoiseGating m_noisegating;

    CLimiting m_limiting;

    CChorus m_chorus;

    CFlange m_flange;

    std::vector< std::vector<std::array<double, 2>> > m_wavesamples;

};

#pragma comment(lib, "msxml2.lib")
#pragma once
#include "Instrument.h"
#include <vector>
#include <array>


class CWavetable :
    public CInstrument
{
public:
    CWavetable() = delete;
    CWavetable(std::vector< std::vector<std::array<double, 2>> >* wavesamples);

    void SetNote(CNote* note) override;
    void Start() override;
    bool Generate() override;

    enum SampleType { Strummed, Plucked, Harmonic };

private:
    std::vector< std::vector<std::array<double, 2>> >* m_wavesamples;

    double m_freq;
    double m_playbackratio;

    double m_dur;
    int m_sampletype;

    double m_sampleindex1;
    double m_sampleindex2;
    double m_time;
};


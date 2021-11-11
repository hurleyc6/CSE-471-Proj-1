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

    enum State { Attack, IntoSustain, Sustain, IntoDecay, Decay };

private:
    std::vector< std::vector<std::array<double, 2>> >* m_wavesamples;

    double m_freq;
    double m_playbackratio;

    double m_dur;
    double m_transitionperiod;
    int m_mainsample;
    int m_attacksample;
    State m_playingstate;
    bool m_susfade;

    double m_sampleindex1;
    double m_sampleindex2;
    double m_time;

    double NextIndex(int sampleNum, double currentIndex);

    std::vector<std::array<double, 2>>& GetSample(int sampleNum) { return m_wavesamples->at(sampleNum); }
    double SampleTime(int sampleNum) { return GetSample(sampleNum).size() * GetSamplePeriod() / m_playbackratio; }

    std::array<double, 2>& GetSampleFrame(int sampleNum, int frameNum);
    double InterpolateSample(int sampleNum, double index, int channel);
    double InterpolateSamples(int sample1Num, double index1, int sample2Num, double index2, double fadeRatio, int channel);
};


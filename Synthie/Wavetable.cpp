#include "stdafx.h"
#include "Wavetable.h"
#include "Notes.h"
#include <string>
#include <sstream>
#include <vector>

// Each sample was recorded on the D string, which is D3
const double SAMPLE_FREQ = 146.832;
const int PLUCK = 0;
const int STRUM = 1;
const int SUSTAIN = 2;
const int HARMONIC = 3;
const int DECAY = 4;
const double SUSTAINLOOPTRANSITION = .013;


CWavetable::CWavetable(std::vector< std::vector<std::array<double, 2>> >* wavesamples)
{
	m_freq = SAMPLE_FREQ;
	m_attacksample = STRUM;
	m_mainsample = m_attacksample;
	m_time = 0;
	m_dur = 0;
	m_sampleindex1 = 0;
	m_sampleindex2 = 0;
	m_playbackratio = 1;
	m_transitionperiod = .05;
	m_playingstate = State::Attack;
	m_susfade = false;
	
	m_wavesamples = wavesamples;
}

void CWavetable::SetNote(CNote* note)
{
	m_note = note;

	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	for (int i = 0; i < len; i++)
	{

		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		CComBSTR name;
		attrib->get_nodeName(&name);

		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "dur")
		{

			value.ChangeType(VT_R8);
			m_dur = value.dblVal * GetBPM();

		}

		else if (name == "note")
		{
			m_freq = NoteToFrequency(value.bstrVal);
			m_playbackratio = m_freq / SAMPLE_FREQ;
		}

		else if (name == "type")
		{
			std::wstring enteredType = value.bstrVal;

			if (enteredType == L"harmonic")
			{
				m_attacksample = HARMONIC;
			}
			else if (enteredType == L"pluck")
			{
				m_attacksample = PLUCK;
			}
			else
			{
				// Default Strummed
				m_attacksample = STRUM;
			}

		}
	}
}


void CWavetable::Start()
{
	m_time = 0;
	m_sampleindex1 = 0;
	m_sampleindex2 = 0;
	m_playingstate = State::Attack;
	m_mainsample = m_attacksample;
	m_susfade = false;
}

double CWavetable::NextIndex(int sampleNum, double currentIndex)
{
	double increment = currentIndex + m_playbackratio;
	return fmod(increment, double(m_wavesamples->at(sampleNum).size()));
}

std::array<double, 2>& CWavetable::GetSampleFrame(int sampleNum, int frameNum)
{
	return GetSample(sampleNum)[frameNum];
}

double CWavetable::InterpolateSample(int sampleNum, double index, int channel)
{
	double interpRatio = index - int(index);
	int nextFrameI = (int(index) + 1) % m_wavesamples->at(sampleNum).size();

	return GetSampleFrame(sampleNum, int(index)).at(channel) * interpRatio + GetSampleFrame(sampleNum, nextFrameI).at(channel) * (1. - interpRatio);
}

double CWavetable::InterpolateSamples(int sample1Num, double index1, int sample2Num, double index2, double fadeRatio, int channel)
{
	double sample1Frame = InterpolateSample(sample1Num, index1, channel);
	double sample2Frame = InterpolateSample(sample2Num, index2, channel);
	return sample1Frame * fadeRatio + sample2Frame * (1. - fadeRatio);
}

bool CWavetable::Generate()
{
	//// Quick check to make sure we fade out
	//if (m_dur - m_time < m_transitionperiod)
	//{
	//	m_playingstate = State::Decay;
	//	m_sampleindex1 = 0
	//}

	// Based on playing state, make a 
	switch (m_playingstate)
	{
	case CWavetable::Attack:
		{
			m_frame[0] = InterpolateSample(m_attacksample, m_sampleindex1, 0);
			m_frame[1] = InterpolateSample(m_attacksample, m_sampleindex1, 1);

			if (m_time > SampleTime(m_attacksample) - m_transitionperiod)
			{
				m_playingstate = IntoSustain;
				m_sampleindex2 = 0;
			}
		}
		break;
	case CWavetable::IntoSustain:
		{
			double interpRatio = (SampleTime(m_attacksample) - m_time) / m_transitionperiod;
			m_frame[0] = InterpolateSamples(m_attacksample, m_sampleindex1, SUSTAIN, m_sampleindex2, interpRatio, 0);
			m_frame[1] = InterpolateSamples(m_attacksample, m_sampleindex1, SUSTAIN, m_sampleindex2, interpRatio, 1);

			if (m_time >= SampleTime(m_attacksample) - GetSamplePeriod())
			{
				m_playingstate = State::Sustain;
				m_mainsample = SUSTAIN;
				m_sampleindex1 = m_sampleindex2;
				m_sampleindex2 = 0;
			}

			m_sampleindex2 = NextIndex(SUSTAIN, m_sampleindex2);
		}
		break;
	case CWavetable::Sustain:
		{
			// Fade between loops if close to end
			if (m_sampleindex1 / double(GetSample(SUSTAIN).size()) > 1. - SUSTAINLOOPTRANSITION / SampleTime(SUSTAIN))
			{
				if (!m_susfade)
				{
					m_susfade = true;
					m_sampleindex2 = 0;
				}
				
			}
			else if (m_susfade)
			{
				// Normal loop should pick up where fade ends
				m_sampleindex1 = m_sampleindex2;
				m_susfade = false;
			}

			if (m_susfade)
			{
				double interpRatio = (1. - m_sampleindex1 / double(GetSample(SUSTAIN).size())) / (SUSTAINLOOPTRANSITION / SampleTime(SUSTAIN));
				interpRatio *= interpRatio;
				m_frame[0] = InterpolateSamples(SUSTAIN, m_sampleindex1, SUSTAIN, m_sampleindex2, interpRatio, 0);
				m_frame[1] = InterpolateSamples(SUSTAIN, m_sampleindex1, SUSTAIN, m_sampleindex2, interpRatio, 1);
			}
			else
			{
				m_frame[0] = InterpolateSample(SUSTAIN, m_sampleindex1, 0);
				m_frame[1] = InterpolateSample(SUSTAIN, m_sampleindex1, 1);
			}

			m_sampleindex2 = NextIndex(SUSTAIN, m_sampleindex2);

			if (m_time > m_dur - SampleTime(DECAY))
			{
				m_playingstate = State::IntoDecay;
				m_sampleindex2 = 0;
			}
		}
		break;
	case CWavetable::IntoDecay:
		{
			double interpRatio = 1. - (m_time - m_dur + SampleTime(DECAY)) / m_transitionperiod;
			m_frame[0] = InterpolateSamples(SUSTAIN, m_sampleindex1, DECAY, m_sampleindex2, interpRatio, 0);
			m_frame[1] = InterpolateSamples(SUSTAIN, m_sampleindex1, DECAY, m_sampleindex2, interpRatio, 1);

			m_sampleindex2 = NextIndex(DECAY, m_sampleindex2);

			if (m_time > m_dur - SampleTime(DECAY) + m_transitionperiod)
			{
				m_playingstate = State::Decay;
				m_mainsample = DECAY;
				m_sampleindex1 = m_sampleindex2;
				m_sampleindex2 = 0;
			}
		}
		break;
	case CWavetable::Decay:
		{
			double amplitude = (m_dur - m_time) / m_transitionperiod;
			amplitude = amplitude > 1. ? 1 : amplitude;
			m_frame[0] = amplitude * InterpolateSample(DECAY, m_sampleindex1, 0);
			m_frame[1] = amplitude * InterpolateSample(DECAY, m_sampleindex1, 1);
		}
		break;
	default:
		break;
	}

	m_sampleindex1 = NextIndex(m_mainsample, m_sampleindex1);
	m_time += GetSamplePeriod();
	return m_time < m_dur;
}

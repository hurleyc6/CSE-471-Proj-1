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


CWavetable::CWavetable(std::vector< std::vector<std::array<double, 2>> >* wavesamples)
{
	m_freq = SAMPLE_FREQ;
	m_sampletype = STRUM;
	m_time = 0;
	m_dur = 0;
	m_sampleindex1 = 0;
	m_sampleindex2 = 0;
	m_playbackratio = 1;
	
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
			m_dur = value.dblVal;

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
				m_sampletype = Harmonic;
			}
			else if (enteredType == L"pluck")
			{
				m_sampletype = Plucked;
			}
			else
			{
				// Default Strummed
				m_sampletype = Strummed;
			}

		}
	}
}


void CWavetable::Start()
{
	m_time = 0;
	m_sampleindex1 = 0;
	m_sampleindex2 = 0;
}

bool CWavetable::Generate()
{
	m_frame[0] = m_wavesamples->at(SUSTAIN)[m_sampleindex1].at(0);
	m_frame[1] = m_wavesamples->at(SUSTAIN)[m_sampleindex1].at(1);

	m_sampleindex1 += m_playbackratio;
	m_sampleindex1 = fmod( m_sampleindex1, double(m_wavesamples->at(SUSTAIN).size()) );
	m_time += GetSamplePeriod();
	return m_time < m_dur;
}

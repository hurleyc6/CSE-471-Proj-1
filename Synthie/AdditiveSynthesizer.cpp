#include "stdafx.h"
#include "AdditiveSynthesizer.h"
#include "Notes.h"
#include <string>
#include <sstream>
#include <vector>

CAdditiveSynthesizer::CAdditiveSynthesizer(void)
{

	m_start = 0.0;
	m_stop = 0.0;
	m_decay = 0.0;
	m_cfi = 0.0;
	m_cfo = 0.0;
	m_num = 1.0;

}

void CAdditiveSynthesizer::Start()
{

	m_wave.SetSampleRate(GetSampleRate());
	m_wave.SetDur(m_dur);
	m_wave.Start();
	m_time = 0;

}

bool CAdditiveSynthesizer::Generate()
{

	m_wave.Generate();
	m_frame[0] = m_wave.Frame(0);
	m_frame[1] = m_wave.Frame(1);
	double sub = m_dur - m_stop;
	double x = -1.0;
	double y = 1.0;
	
	if (m_num > 1.0)
	{
		x = 1.0;
	}

	if (m_start > m_time)
	{
		y = m_time / m_start;
	}

	else if (m_decay > m_time) 
	{
		double temp = m_decay - m_start;
		double diff = x * ((1.0 - m_num) / temp);
		y = (diff * m_time) + (1.0 - (diff * m_start));
	}

	else if (sub < m_time && m_stop != 0) 
	{
		if (m_num <= 0)
		{
			m_num = 1.0;
		}
		double pos = m_num / m_stop;
		double neg = -m_num / m_stop;
		y = m_dur * pos + m_time * neg;
	}

	else 
	{
		y = m_num;
	}

	m_frame[0] *= y;
	m_frame[1] *= y;

	m_time += GetSamplePeriod();

	return m_time < m_dur;

}

void CAdditiveSynthesizer::SetNote(CNote* note)
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
			SetDur(value.dblVal);

		}

		else if (name == "note")
		{

			double val = NoteToFrequency(value.bstrVal);
			SetFreq(val);

		}

		else if (name == "amp")
		{

			std::wstring s(value.bstrVal);
			std::string str(s.begin(), s.end());
			std::stringstream ss(str);
			std::string temp;
			double harmonics[8] = { 0 };

			int i = 0;
			while (std::getline(ss, temp, char(32))) 
			{
				harmonics[i++] = atof(temp.c_str());
			}

			SetAmp(harmonics[0]);

		}

		else if (name == "cfi")
		{

			value.ChangeType(VT_R8);
			double val = value.dblVal * m_dur;
			SetCFI(val);

		}

		else if (name == "cfo")
		{

			value.ChangeType(VT_R8);
			double val = value.dblVal * m_dur;
			SetCFO(val);

		}

		else if (name == "ADSR") 
		{
		
			std::wstring s(value.bstrVal);
			std::string str(s.begin(), s.end());
			std::stringstream ss(str);
			std::string temp;

			std::getline(ss, temp, char(32));
			auto num = atof(temp.c_str());
			m_start = num * m_dur;

			std::getline(ss, temp, char(32));
			num = atof(temp.c_str());
			m_decay = num * m_dur + m_start;

			std::getline(ss, temp, char(32));
			num = atof(temp.c_str());
			m_num = num;

			std::getline(ss, temp, char(32));
			num = atof(temp.c_str());
			m_stop = num * m_dur;

		}

		else if (name == "vibrato") 
		{

			std::wstring s(value.bstrVal);
			std::string str(s.begin(), s.end());
			std::stringstream ss(str);
			std::string temp;
			
			std::getline(ss, temp, char(32));
			auto num = atof(temp.c_str());
			m_wave.SetVR(num);

			std::getline(ss, temp, char(32));
			num = atof(temp.c_str());
			m_wave.SetVF(num);

		}
	}
}

void CAdditiveSynthesizer::SetDur(double d)
{

	m_dur = d * GetBPM();

}

void CAdditiveSynthesizer::SetAmp(double& a)
{

	m_wave.SetAmp(a);

}

void CAdditiveSynthesizer::SetFreq(double f)
{

	m_wave.SetFreq(f);

}

void CAdditiveSynthesizer::SetCFI(double d)
{

	m_start = d * GetBPM();

}

void CAdditiveSynthesizer::SetCFO(double d)
{

	m_stop = d * GetBPM();

}
#include "stdafx.h"
#include "Wavetable.h"
#include "Notes.h"
#include <string>
#include <sstream>
#include <vector>

CWavetable::CWavetable()
{
}

CWavetable::~CWavetable()
{
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


void CWavetable::Start()
{
}

bool CWavetable::Generate()
{
	return false;
}

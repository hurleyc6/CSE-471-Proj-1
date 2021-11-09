#include "stdafx.h"
#include "Chorus.h"
#include <cmath>

CChorus::CChorus(void)
{

	m_wet = 0.0;
	m_dry = 0.0;
	m_delay = 0.0;
	m_fract = 0.0;
	m_range = 0.0;
	m_rate = 0.0;
	m_threshold = 0;
	
	m_first.resize(200000);
	m_second.resize(200000);

}

void CChorus::Play(double* in, double* out)
{
	// There are 3 taps into the past
	// The base delay for the taps are as follows:
	//   tap 1: m_delay
	//   tap 2: 2/3 * m_delay
	//	 tap 3: 1/3 * m_delay
	//
	// The delays oscillate based on m_rate. 
	// m_rate is a low frequency in Hertz.
	// We will use LFO to describe the sinewave oscillating at the real rate
	// The real rate for each tap is as follows:
	//   tap 1: 0.9 * m_rate
	//   tap 2: m_rate
	//   tap 3: 1.1 * m_rate
	// 
	// m_range describes how far the tap delay can 
	// vary from the base delay as a proportion of the base delay.
	// m_range should be between 0 and 1.
	// 
	// The tap delay in seconds is then calculated as follows:
	//   (1 + LFO(t) * m_range) * (base delay)
	
	double baseAngle = 2. * PI * m_rate * m_threshold / m_fract;
	double lfo1 = sin(0.9 * baseAngle);
	double lfo2 = sin(baseAngle);
	double lfo3 = sin(1.1 * baseAngle);

	// Multiplying by sampling frequency converts delay to samples
	// Adding 0.5 effectively rounds the value
	int tapDelay1 = (1 + lfo1 * m_range) * m_delay * m_fract + 0.5;
	int tapDelay2 = (1 + lfo2 * m_range) * (2./3. * m_delay) * m_fract + 0.5;
	int tapDelay3 = (1 + lfo3 * m_range) * (1./3. * m_delay) * m_fract + 0.5;

	m_threshold = (1 + m_threshold) % 200000;
	m_first[m_threshold] = in[0];
	m_second[m_threshold] = in[1];

	int rdloc1 = (m_threshold - tapDelay1 + 200000) % 200000;
	int rdloc2 = (m_threshold - tapDelay2 + 200000) % 200000;
	int rdloc3 = (m_threshold - tapDelay3 + 200000) % 200000;

	out[0] = 0.6 * (in[0] + m_first[rdloc1] + m_first[rdloc2] + m_first[rdloc3]);
	out[0] *= m_wet;
	out[0] += m_dry * in[0];

	out[1] = 0.6 * (in[1] + m_second[rdloc1] + m_second[rdloc2] + m_second[rdloc3]);
	out[1] *= m_wet;
	out[1] += m_dry * in[1];

}

void CChorus::XmlLoad(IXMLDOMNode* xml)
{

	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
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

		if (name == "delay")

		{
			value.ChangeType(VT_R8);
			m_delay = value.dblVal;

		}

		else if (name == "wet")
		{

			value.ChangeType(VT_R8);
			m_wet = value.dblVal;

		}

		else if (name == "dry")
		{

			value.ChangeType(VT_R8);
			m_dry = value.dblVal;

		}

		else if (name == "range")
		{

			value.ChangeType(VT_R8);
			m_range = value.dblVal;

		}

		else if (name == "rate")
		{

			value.ChangeType(VT_R8);
			m_rate = value.dblVal;

		}
	}
}

void CChorus::SFract(double f)
{

	m_fract = f;

}
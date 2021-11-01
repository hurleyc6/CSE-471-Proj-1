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

	double s = sin(2 * PI * m_rate);
	double rd = m_range * m_delay;
	double diff = s * rd;
	double temp = diff + m_delay;
	int x = temp * m_fract + 0.5;

	m_threshold = (1 + m_threshold) % 200000;
	m_first[m_threshold] = in[0];
	m_second[m_threshold] = in[1];

	int newt = m_threshold + 200000;
	int size = x * 2;
	int rdloc = (newt - size) % 200000;

	out[0] = in[0] / 2 + m_first[rdloc] / 2;
	out[0] *= m_wet;
	out[0] += m_dry * in[0];

	out[1] = in[1] / 2 + m_second[rdloc] / 2;
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
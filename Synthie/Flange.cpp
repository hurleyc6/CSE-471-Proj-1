#include "stdafx.h"
#include "Flange.h"

CFlange::CFlange(void)
{

	m_wet = 0.0;
	m_dry = 0.0;
	m_delay = 0.0;
	m_fract = 0.0;
	m_range = 0.0;
	m_rate = 0.0;
	m_num = 1.0;
	m_threshold = 0;

	m_inputL.resize(200000);
	m_inputR.resize(200000);

}

void CFlange::Play(double* in, double* out)
{
	double baseAngle = 2. * PI * m_rate * m_threshold / m_fract;
	double lfo = sin(baseAngle);
	int tapDelay = (1 + lfo * m_range) * m_delay * m_fract + 0.5;

	m_threshold = (1 + m_threshold) % 200000;
	m_inputL[m_threshold] = in[0];
	m_inputR[m_threshold] = in[1];

	int rdloc = (m_threshold - tapDelay + 200000) % 200000;

	double temp1 = in[0] + m_inputL[rdloc] * m_num;
	double temp2 = in[1] + m_inputR[rdloc] * m_num;
	out[0] = m_wet * temp1;
	out[0] += m_dry * in[0];

	out[1] = m_wet * temp2;
	out[1] += m_dry * in[1];
}

void CFlange::XmlLoad(IXMLDOMNode* xml)
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

		else if (name == "num")
		{

			value.ChangeType(VT_R8);
			m_num = value.dblVal;

		}
	}
}

void CFlange::SFract(double f)
{

	m_fract = f;

}
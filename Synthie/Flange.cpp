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

	m_first.resize(200000);
	m_second.resize(200000);
	m_one.resize(200000);
	m_two.resize(200000);

}

void CFlange::Play(double* in, double* out)
{
	double diff = (m_range * m_delay) * sin(2 * PI * m_rate);
	double temp = m_delay + diff;

	m_threshold = (m_threshold + 1) % 200000;
	m_first[m_threshold] = in[0];
	m_second[m_threshold] = in[1];

	int size = int((temp * m_fract + 0.5)) * 2;
	int rdloc = (m_threshold + 200000 - size) % 200000;

	out[0] = in[0] / 3 + m_first[rdloc] / 3 + (m_one[rdloc] * m_num) / 3;
	out[0] *= m_wet;
	out[1] = in[1] / 3 + m_second[rdloc] / 3 + (m_two[rdloc] * m_num) / 3;
	out[1] *= m_wet;

	out[0] += in[0] * m_dry;
	out[1] += in[1] * m_dry;

	m_one[m_threshold] = out[0];
	m_two[m_threshold] = out[1];
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

		if (name == "wet")
		{

			value.ChangeType(VT_R8);
			m_wet = value.dblVal;

		}
		else if (name == "dry")
		{

			value.ChangeType(VT_R8);
			m_dry = value.dblVal;

		}
		else if (name == "delay")

		{
			value.ChangeType(VT_R8);
			m_delay = value.dblVal;

		}
		else if (name == "range")
		{

			value.ChangeType(VT_R8);
			m_range = value.intVal;

		}
		else if (name == "rate")
		{

			value.ChangeType(VT_R8);
			m_rate = value.intVal;

		}
		else if (name == "num")
		{

			value.ChangeType(VT_R8);
			m_num = value.intVal;

		}
	}
}

void CFlange::SFract(double f)
{
	m_fract = f;
}
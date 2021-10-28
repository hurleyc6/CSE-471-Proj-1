#include "stdafx.h"
#include "Limiting.h"

CLimiting::CLimiting(void)
{

	m_wet = 0.0;
	m_dry = 1.0;
	m_first = 1.0;
	m_second = 1.0;
	m_fract = 1.0;
	m_threshold = 0;

}

void CLimiting::Play(double* in, double* out)
{

	if (in[0] > m_threshold || in[0] < -m_threshold)
	{
		double diff = in[0] - m_threshold;
		double frame = (diff / m_fract) + m_threshold;
		out[0] = m_dry * in[0] + m_wet * frame;
	}
	else
	{
		out[0] = in[0];
	}

	if (in[1] > m_threshold || in[1] < -m_threshold)
	{
		double diff = in[1] - m_threshold;
		double frame = (diff / m_fract) + m_threshold;
		out[1] = m_dry * in[1] + m_wet * frame;
	}
	else
	{
		out[1] = in[1];
	}

}

void CLimiting::XmlLoad(IXMLDOMNode* xml)
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
		else if (name == "ratio")

		{
			value.ChangeType(VT_R8);
			m_fract = value.dblVal;

		}
		else if (name == "threshold")
		{

			value.ChangeType(VT_I4);
			m_threshold = value.intVal;

		}
	}
}
#include "stdafx.h"
#include "Limiting.h"

CLimiting::CLimiting(void)
{

	m_wet = 0.0;
	m_dry = 1.0;
	m_inputL = 1.0;
	m_inputR = 1.0;
	m_fract = 1.0;
	m_threshold = 0;

}

void CLimiting::Play(double* in, double* out)
{

	for (int i = 0; i < 2; i++)
	{

		if (in[i] > m_threshold || in[i] < -m_threshold)
		{

			double diff = in[i] - m_threshold;
			double x = diff / m_fract;
			double frame = x + m_threshold;
			double d = m_dry * in[i];
			double w = m_wet * frame;
			out[i] = d + w;

		}

		else
		{

			out[i] = in[i];

		}
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

		if (name == "threshold")
		{

			value.ChangeType(VT_I4);
			m_threshold = value.intVal;

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

		else if (name == "ratio")
		{

			value.ChangeType(VT_R8);
			m_fract = value.dblVal;

		}
	}
}
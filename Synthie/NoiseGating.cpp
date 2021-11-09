#include "stdafx.h"
#include "NoiseGating.h"

CNoiseGating::CNoiseGating(void)
{

	m_wet = 0.0;
	m_dry = 0.0;
	m_inputL = 1.0;
	m_inputR = 1.0;
	m_threshold = 0;

}

void CNoiseGating::Play(double* in, double* out)
{
	for (int i = 0; i < 2; i++)
	{

		if (i == 0)
		{

			if ((in[i] < m_threshold) && (in[i] > -m_threshold))
			{

				m_inputL -= .005;
				if (m_inputL < 0)
				{

					m_inputL = 0;

				}
					
			}

			else
			{

				m_inputL += .005;
				if (m_inputL > 1)
				{

					m_inputL = 1;

				}
					
			}

			double w = m_wet * in[i] * m_inputL;
			double d = m_dry * in[i];
			out[i] = d + w;

		}

		else if (i == 1)
		{

			if ((in[i] < m_threshold) && (in[i] > -m_threshold))
			{

				m_inputR -= .005;
				if (m_inputR < 0)
				{

					m_inputR = 0;

				}
					
			}

			else
			{

				m_inputR += .005;
				if (m_inputR > 1)
				{

					m_inputR = 1;

				}
					
			}

			double w = m_wet * in[i] * m_inputR;
			double d = m_dry * in[i];
			out[i] = d + w;

		}
	}
}

void CNoiseGating::XmlLoad(IXMLDOMNode* xml)
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
	}
}
#pragma once

class CNoiseGating
{
public:

	CNoiseGating(void);

	void Play(double* in, double* out);

	void XmlLoad(IXMLDOMNode* xml);

private:

	double m_wet;

	double m_dry;

	double m_inputL;

	double m_inputR;

	int m_threshold;

};
#pragma once

class CLimiting
{
public:

	CLimiting(void);

	void Play(double* in, double* out);

	void XmlLoad(IXMLDOMNode* xml);

private:

	double m_wet;

	double m_dry;

	double m_inputL;

	double m_inputR;

	double m_fract;

	int m_threshold;

};
#pragma once

#include <vector>

class CChorus
{
public:

	CChorus(void);

	void Play(double* in, double* out);

	void XmlLoad(IXMLDOMNode* xml);

	void SFract(double f);

private:

	double m_delay;

	double m_wet;

	double m_dry;

	double m_fract;

	double m_range;

	double m_rate;
	
	int m_threshold;

	std::vector<double> m_first;

	std::vector<double> m_second;

};
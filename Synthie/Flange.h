#pragma once

#include <vector>

class CFlange
{
public:

	CFlange(void);

	void Play(double* in, double* out);

	void XmlLoad(IXMLDOMNode* xml);

	void SFract(double f);

private:

	double m_wet;

	double m_dry;

	double m_delay;

	double m_fract;

	double m_range;

	double m_rate;

	double m_num;

	int m_threshold;

	std::vector<double> m_first;

	std::vector<double> m_second;

	std::vector<double> m_one;

	std::vector<double> m_two;

};
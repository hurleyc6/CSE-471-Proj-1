#include "stdafx.h"
#include "AdditiveWaves.h"
#include <iostream>
#include <cmath>

short check(double num)
{

	if (num > 30000)
	{
		return 30000;
	}

	if (num < -30000)
	{
		return -30000;
	}

	return short(num);

}

CAdditiveWaves::CAdditiveWaves(void)
{

	m_freq = 440.0;
	m_amp1[0] = 1.0;
	m_amp2 = 1000.0;
	m_vr = 0.0;
	m_vf = 0.0;
	
	for (int i = 1; i < 8; i++)
	{
		m_amp1[i] = 0.0;
	}

}

void CAdditiveWaves::Start()
{

	val = 0;
	int temp = m_dur * GetSampleRate();
	sound = new short[temp];
	CAdditiveWaves::GenerateWaves();

}

bool CAdditiveWaves::Generate()
{

	m_frame[0] = m_frame[1] = sound[val++];
	return true;

}

void CAdditiveWaves::GenerateWaves()
{

	int temp = m_dur * GetSampleRate();
	double ttemp = 1.0 / GetSampleRate();
	double first = 0;
	double second = 0;
	double time = 0;
	
	int i;
	for (i = 0; i < temp; i++, time += ttemp)
	{

		double x = 0;
		for (float j = 1.0; j < 8.0 && (j * m_freq) <= 22050; j++)
		{
			double s = sin(j * first);
			double a = m_amp1[int(j - 1)];
			x += (m_amp2 * a * s);
		}

		double y = 2 * PI;
		double z = m_vf * sin(second) + m_freq;
		first += (y * z) / GetSampleRate();
		second += (y * m_vr) / GetSampleRate();
		sound[i] = check(x);

	}
}

void CAdditiveWaves::SetDur(double d)
{

	m_dur = d;

}

void CAdditiveWaves::SetAmp(double& a)
{

	memcpy(&m_amp1, &a, sizeof(double) * 8);

}

void CAdditiveWaves::SetFreq(double f)
{

	m_freq = f;

}

void CAdditiveWaves::SetVR(double d)
{

	m_vr = d;

}

void CAdditiveWaves::SetVF(double d)
{

	m_vf = d;

}
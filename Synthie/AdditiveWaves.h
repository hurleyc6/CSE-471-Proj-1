#pragma once
#include "AudioNode.h"

class CAdditiveWaves : public CAudioNode
{
public:

	CAdditiveWaves(void);

	virtual void Start();

	virtual bool Generate();

	void GenerateWaves();

	void SetDur(double d);

	void SetAmp(double& a);

	void SetFreq(double f);

	void SetVR(double d);

	void SetVF(double d);

private:

	int val;

	short* sound;

	double m_dur;

	double m_freq;

	double m_amp1[8];

	double m_amp2;
	
	double m_vr;

	double m_vf;

};
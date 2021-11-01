#pragma once
#include "Instrument.h"
#include "AdditiveWaves.h"

class CAdditiveSynthesizer : public CInstrument
{
public: 

	CAdditiveSynthesizer(void);

	virtual void Start();

	virtual bool Generate();

	virtual void SetNote(CNote* note);

	void SetDur(double d);

	void SetAmp(double& a);

	void SetFreq(double f);

	void SetCFI(double d);

	void SetCFO(double d);

private:

	double m_start;

	double m_stop;
	
	double m_decay;

	double m_cfi;

	double m_cfo;

	double m_num;

	double m_dur;

	double m_time;

	CAdditiveWaves m_wave;

};
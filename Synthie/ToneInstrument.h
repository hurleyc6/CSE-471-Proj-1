#pragma once
#include "Instrument.h"
#include "SineWave.h"
#include "AR.h"

class CToneInstrument : public CInstrument
{
public:

    CToneInstrument();

    virtual void Start();

    virtual bool Generate();

    virtual void SetNote(CNote* note);

    void SetFreq(double f);

    void SetAmplitude(double a);

    void SetDuration(double d);

private:

    CSineWave m_sinewave;

    double m_dur;

    double m_time;

    double m_stop;

    double m_start;

    //CAR m_ar;

};
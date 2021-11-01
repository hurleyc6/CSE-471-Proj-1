#include "stdafx.h"
#include "ToneInstrument.h"
#include "Notes.h"

//const double BEATS = 60.0;

CToneInstrument::CToneInstrument(void)
{

    m_dur = 0.1;
    m_start = .05;
    m_stop = .05;

}

void CToneInstrument::Start()
{

    m_sinewave.SetSampleRate(GetSampleRate());
    m_sinewave.Start();
    m_time = 0;

    // Tell the AR object it gets its samples from 
    // the sine wave object.
    /*m_ar.SetSource(&m_sinewave);
    m_ar.SetSampleRate(GetSampleRate());
    m_ar.Start();*/

}

bool CToneInstrument::Generate()
{

    m_sinewave.Generate();
    m_frame[0] = m_sinewave.Frame(0);
    m_frame[1] = m_sinewave.Frame(1);

    double x = 0;
    double y = m_dur * GetBPM();
    if (m_start > m_time)
    {

        double f = 1.0 / m_start;
        x = m_time * f;
        m_frame[0] *= x;
        m_frame[1] *= x;

    }

    else if (m_time > y - m_stop)
    {

        double fp = (1.0 / m_stop) * y;
        double fn = (-1.0 / m_stop) * m_time;
        x = fp + fn;
        m_frame[0] *= x;
        m_frame[1] *= x;

    }

    m_time += GetSamplePeriod();
    return m_time < (m_dur* GetBPM());

    //return m_time < m_duration;

    /*bool valid = m_ar.Generate();
    m_frame[0] = m_ar.Frame(0);
    m_frame[1] = m_ar.Frame(1);

    m_time += GetSamplePeriod();
    return valid;*/

}

void CToneInstrument::SetNote(CNote* note)
{

    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    note->Node()->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Loop over the list of attributes
    for (int i = 0; i < len; i++)
    {

        // Get attribute i
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        // Get the name of the attribute
        CComBSTR name;
        attrib->get_nodeName(&name);

        // Get the value of the attribute.  A CComVariant is a variable
        // that can have any type. It loads the attribute value as a
        // string (UNICODE), but we can then change it to an integer 
        // (VT_I4) or double (VT_R8) using the ChangeType function 
        // and then read its integer or double value from a member variable.
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "dur")
        {

            value.ChangeType(VT_R8);
            SetDuration(value.dblVal);
            //m_ar.SetDuration((BEATS / m_bpm) * value.dblVal);

        }

        else if (name == "note")
        {

            SetFreq(NoteToFrequency(value.bstrVal));

        }
    }
}

void CToneInstrument::SetFreq(double f)
{

    m_sinewave.SetFreq(f);

}

void CToneInstrument::SetAmplitude(double a)
{

    m_sinewave.SetAmplitude(a);

}

void CToneInstrument::SetDuration(double d)
{

    m_dur = d;

}
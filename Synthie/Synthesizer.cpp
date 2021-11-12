#include "stdafx.h"
#include "Synthesizer.h"
#include "Instrument.h"
#include "ToneInstrument.h"
#include "AdditiveSynthesizer.h"
#include "AdditiveWaves.h"
#include "Wavetable.h"
#include "xmlhelp.h"
#include <algorithm>
#include <cmath>
#include <audio/DirSoundSource.h>

using namespace std;

const static int CHORUS = 0;
const static int FLANGE = 1;
const static int NOISEGATING = 2;
const static int LIMITING = 3;

CSynthesizer::CSynthesizer(void)
{

    CoInitialize(NULL);
	m_channels = 2;
	m_sampleRate = 44100.;
	m_samplePeriod = 1 / m_sampleRate;
	m_time = 0;

    m_bpm = 120;
    m_secperbeat = 1/(m_bpm/60);
    m_beatspermeasure = 4;

    m_chorus.SFract(m_sampleRate);
    m_flange.SFract(m_sampleRate);

    m_NG = false;
    m_LT = false;
    m_CS = false;
    m_FL = false;

    LoadWaveSamples();
}

void CSynthesizer::Start(void)
{

    m_instruments.clear();
    m_currentNote = 0;
    m_measure = 0;
    m_beat = 0;
    m_time = 0;

}

bool CSynthesizer::Generate(double* frame)
{

    //
    // Phase 1: Determine if any notes need to be played.
    //

    while (m_currentNote < (int)m_notes.size())
    {

        // Get a pointer to the current note
        CNote* note = &m_notes[m_currentNote];

        // If the measure is in the future we can't play
        // this note just yet.
        if (note->Measure() > m_measure)
            break;

        // If this is the current measure, but the
        // beat has not been reached, we can't play
        // this note.
        if (note->Measure() == m_measure && note->Beat() > m_beat)
            break;

        //
        // Play the note!
        //

        // Create the instrument object
        CInstrument* instrument = NULL;
        if (note->Instrument() == L"ToneInstrument")
        {

            instrument = new CToneInstrument();

        }
        else if (note->Instrument() == L"AdditiveSynth")
        {

            instrument = new CAdditiveSynthesizer();

        }
        else if (note->Instrument() == L"Wavetable")
        {
            instrument = new CWavetable(&m_wavesamples);
        }

        // Configure the instrument object
        if (instrument != NULL)
        {

            instrument->SetSampleRate(GetSampleRate());
            //instrument->Synthes(this);
            instrument->SetBPM(m_secperbeat);
            instrument->SetNote(note);
            instrument->Start();

            m_instruments.push_back(instrument);

        }

        m_currentNote++;
    }

    //
    // Phase 2: Clear all channels to silence 
    //

    for (int c = 0; c < GetNumChannels(); c++)
    {

        frame[c] = 0;

    }

    //
    // Phase 3: Play an active instruments
    //

    //
    // We have a list of active (playing) instruments.  We iterate over 
    // that list.  For each instrument we call generate, then add the
    // output to our output frame.  If an instrument is done (Generate()
    // returns false), we remove it from the list.
    //

    for (list<CInstrument*>::iterator node = m_instruments.begin(); node != m_instruments.end(); )
    {

        // Since we may be removing an item from the list, we need to know in 
        // advance, what is after it in the list.  We keep that node as "next"
        list<CInstrument*>::iterator next = node;
        next++;

        // Get a pointer to the allocated instrument
        CInstrument* instrument = *node;

        // Call the generate function
        if (instrument->Generate())
        {

            // If we returned true, we have a valid sample.  Add it 
            // to the frame.
            for (int c = 0; c < GetNumChannels(); c++)
            {

                frame[c] += instrument->Frame(c);

            }

            double newFrame[2];
            newFrame[0] = 0;
            newFrame[1] = 0;

            double cframe[2];
            if (instrument->HasEffect(CHORUS))
            {
                m_chorus.Play(frame, cframe);
                newFrame[0] += cframe[0];
                newFrame[1] += cframe[1];
            }

            double fframe[2];
            if (instrument->HasEffect(FLANGE))
            {
                m_flange.Play(frame, fframe);
                newFrame[0] += fframe[0];
                newFrame[1] += fframe[1];
            }

            double nframe[2];
            if (instrument->HasEffect(NOISEGATING))
            {
                m_noisegating.Play(frame, nframe);
                newFrame[0] += nframe[0];
                newFrame[1] += nframe[1];
            }

            double lframe[2];
            if (instrument->HasEffect(LIMITING))
            {
                m_limiting.Play(frame, lframe);
                newFrame[0] += lframe[0];
                newFrame[1] += lframe[1];
            }

            if (instrument->GetEffectCount() > 0)
            {
                frame[0] = newFrame[0];
                frame[1] = newFrame[1];
            }
        }
        else
        {

            // If we returned false, the instrument is done.  Remove it
            // from the list and delete it from memory.
            m_instruments.erase(node);
            delete instrument;

        }

        // Move to the next instrument in the list
        node = next;
    }

    //
    // Phase 4: Advance the time and beats
    //

    // Time advances by the sample period
    m_time += GetSamplePeriod();

    // Beat advances by the sample period divided by the 
    // number of seconds per beat.  The inverse of seconds
    // per beat is beats per second.
    m_beat += GetSamplePeriod() / m_secperbeat;

    // When the measure is complete, we move to
    // a new measure.  We might be a fraction into
    // the new measure, so we subtract out rather 
    // than just setting to zero.
    if (m_beat > m_beatspermeasure)
    {

        m_beat -= m_beatspermeasure;
        m_measure++;

    }

    //
    // Phase 5: Determine when we are done
    //

    // We are done when there is nothing to play.  We'll put something more 
    // complex here later.
    return !m_instruments.empty() || m_currentNote < (int)m_notes.size();

}

void CSynthesizer::Clear(void)
{

    m_instruments.clear();
    m_notes.clear();

}

void CSynthesizer::OpenScore(CString& filename)
{

    Clear();

    //
    // Create an XML document
    //

    CComPtr<IXMLDOMDocument>  pXMLDoc;
    bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
        IID_IXMLDOMDocument, (void**)&pXMLDoc));
    if (!succeeded)
    {

        AfxMessageBox(L"Failed to create an XML document to use");
        return;

    }

    // Open the XML document
    VARIANT_BOOL ok;
    succeeded = SUCCEEDED(pXMLDoc->load(CComVariant(filename), &ok));
    if (!succeeded || ok == VARIANT_FALSE)
    {

        AfxMessageBox(L"Failed to open XML score file");
        return;

    }

    //
    // Traverse the XML document in memory!!!!
    // Top level tag is <score>
    //

    CComPtr<IXMLDOMNode> node;
    pXMLDoc->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {

        // Get the name of the node
        CComBSTR nodeName;
        node->get_nodeName(&nodeName);

        if (nodeName == "score")
        {

            XmlLoadScore(node);

        }
    }

    sort(m_notes.begin(), m_notes.end());

}

void CSynthesizer::XmlLoadScore(IXMLDOMNode* xml)
{

    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
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

        if (name == L"bpm")
        {

            value.ChangeType(VT_R8);
            m_bpm = value.dblVal;
            m_secperbeat = 1 / (m_bpm / 60);

        }
        else if (name == L"beatspermeasure")
        {

            value.ChangeType(VT_I4);
            m_beatspermeasure = value.intVal;

        }
    }

    CComPtr<IXMLDOMNode> node;
    xml->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {

        // Get the name of the node
        CComBSTR name;
        node->get_nodeName(&name);

        if (name == L"instrument")
        {

            XmlLoadInstrument(node);
            m_CS = false;
            m_FL = false;
            m_NG = false;
            m_LT = false;
            
        }
    }
}

void CSynthesizer::XmlLoadInstrument(IXMLDOMNode* xml)
{

    wstring instrument = L"";

    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
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

        // Get the value of the attribute.  
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "instrument")
        {

            instrument = value.bstrVal;

        }

        else if (name == "chorus")
        {
            value.ChangeType(VT_I4);
            if (value.intVal == 1)
            {
                m_CS = true;
            }
        }

        else if (name == "flange")
        {
            value.ChangeType(VT_I4);
            if (value.intVal == 1)
            {
                m_FL = true;
            }
        }

        else if (name == "noisegating")
        {
            value.ChangeType(VT_I4);
            if (value.intVal == 1) 
            {
                m_NG = true;
            }
        }

        else if (name == "limiting")
        {
            value.ChangeType(VT_I4);
            if (value.intVal == 1) 
            {
                m_LT = true;
            }
        }
    }

    CComPtr<IXMLDOMNode> node;
    xml->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {

        // Get the name of the node
        CComBSTR name;
        node->get_nodeName(&name);

        if (name == L"note")
        {

            XmlLoadNote(node, instrument);

        }
    }
}

void CSynthesizer::LoadWaveSamples()
{
    const vector<string> sampleFilenames{
        "pluck.wav",
        "strum.wav",
        "sustain.wav",
        "harmonic.wav",
        "decay.wav"
    };

    for (const string& filename : sampleFilenames)
    {
        CDirSoundSource wavein;

        if (!wavein.Open(filename.c_str()))
        {
            AfxMessageBox(L"Failed to open WAV source");
            return;
        }

        int numSampleFrames = wavein.NumSampleFrames();

        m_wavesamples.push_back(std::vector<std::array<double, 2>>());

        vector<std::array<double, 2>>& wavetable = m_wavesamples.back();
        wavetable.resize(numSampleFrames);

        for (int i = 0; i < numSampleFrames; i++)
        {
            short frame[2];
            wavein.ReadFrame(frame);

            // Convert to the double format we use
            wavetable[i][0] = double(frame[0]) / 32767. * 2;
            wavetable[i][1] = double(frame[1]) / 32767. * 2;
        }

        wavein.Close();
    }
}

void CSynthesizer::XmlLoadNote(IXMLDOMNode* xml, std::wstring& instrument)
{

    if (instrument == L"Chorus")
    {
        m_chorus.XmlLoad(xml);
    }

    else if (instrument == L"Flange")
    {
        m_flange.XmlLoad(xml);
    }

    else if (instrument == L"NoiseGate")
    {
        m_noisegating.XmlLoad(xml);
    }

    else if (instrument == L"Limiter")
    {
        m_limiting.XmlLoad(xml);
    }

    m_notes.push_back(CNote());
    m_notes.back().XmlLoad(xml, instrument);

    if (m_CS)
    {
        m_notes.back().SendEffect(CHORUS);
    }

    if (m_FL)
    {
        m_notes.back().SendEffect(FLANGE);
    }

    if (m_NG) 
    {
        m_notes.back().SendEffect(NOISEGATING);
    }
        
    if (m_LT)
    {
        m_notes.back().SendEffect(LIMITING);
    }

}
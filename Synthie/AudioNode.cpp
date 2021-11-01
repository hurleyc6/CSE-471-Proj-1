#include "stdafx.h"
#include "AudioNode.h"

CAudioNode::CAudioNode(void)
{

	//m_bpm = 120;
    m_frame[0] = 0;
    m_frame[1] = 0;
    m_sampleRate = 44100;
    m_samplePeriod = 1.0 / 44100.0;

}

//CAudioNode::CAudioNode(double BPM)
//{
//
//	m_bpm = BPM;
//	m_frame[0] = 0;
//	m_frame[1] = 0;
//	m_sampleRate = 44100;
//	m_samplePeriod = 1.0 / 44100.0;
//
//}
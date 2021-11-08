#include "stdafx.h"
#include "Instrument.h"

CInstrument::CInstrument()
{

	for (int i = 0; i < 4; i++)
	{

		m_effects[i] = false;

	}

}

//void CInstrument::Synthes(CSynthesizer* syn)
//{
//
//	m_synthesizer = syn;
//
//}

bool CInstrument::HasEffect(int effect)
{

	return m_effects[effect];

}

void CInstrument::SendEffect(int effect)
{

	m_effects[effect] = true;

}

int CInstrument::GetEffectCount()
{

	int count = 0;
	for (int i = 0; i < 4; i++)
	{

		if (m_effects[i]) 
		{

			count++;

		}
	}
	return count;

}
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

bool CInstrument::HEffect(int effect)
{

	return m_effects[effect];

}

void CInstrument::SEffect(int effect)
{

	m_effects[effect] = true;

}

int CInstrument::GetEffect()
{

	int count = 0;
	for (int i = 0; i < 4; i++)
	{

		if (m_effects[i]) 
		{

			count++;

		}

		else
		{
			continue;
		}
	}
	return count;

}
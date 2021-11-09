#include "stdafx.h"
#include "Instrument.h"

CInstrument::CInstrument()
{
	m_note = nullptr;
}

//void CInstrument::Synthes(CSynthesizer* syn)
//{
//
//	m_synthesizer = syn;
//
//}

bool CInstrument::HasEffect(int effect)
{

	return m_note != nullptr && m_note->HasEffect(effect);

}

int CInstrument::GetEffectCount()
{

	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (HasEffect(i))
		{
			count++;
		}
	}
	return count;

}
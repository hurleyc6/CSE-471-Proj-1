#pragma once
#include "AudioNode.h"
#include "Note.h"

class CSynthesizer;

class CInstrument : public CAudioNode
{
public:

	CInstrument(void);

	virtual void SetNote(CNote* note) = 0;

	void Synthes(CSynthesizer* syn);

	bool HEffect(int effect);

	void SEffect(int effect);

	int GetEffect();

private:

	CSynthesizer* m_synthesizer = nullptr;

	bool m_effects[4];

};
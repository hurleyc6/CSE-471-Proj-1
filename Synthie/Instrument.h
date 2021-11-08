#pragma once
#include "AudioNode.h"
#include "Note.h"

//class CSynthesizer;

class CInstrument : public CAudioNode
{
public:

	CInstrument(void);

	virtual void SetNote(CNote* note) = 0;

	//void Synthes(CSynthesizer* syn);

	bool HasEffect(int effect);

	void SendEffect(int effect);

	int GetEffectCount();

private:

	//CSynthesizer* m_synthesizer = nullptr;

	bool m_effects[4];

};
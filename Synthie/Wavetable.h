#pragma once
#include "Instrument.h"
class CWavetable :
    public CInstrument
{
public:
    CWavetable();
    ~CWavetable();

    void SetNote(CNote* note) override;
    void Start() override;
    bool Generate() override;
};


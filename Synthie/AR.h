#pragma once
#include "AudioNode.h"

class CAR : public CAudioNode
{
public:

	CAR();

	virtual void Start();

	virtual bool Generate();

	void SetSource(CAudioNode* const& node) { m_node = node; }

	void SetDuration(double d) { m_duration = d; }

private:

	CAudioNode* m_node;

	double m_attack;

	double m_release;

	double m_duration;

	double m_time;

};
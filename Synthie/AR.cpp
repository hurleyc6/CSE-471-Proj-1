#include "stdafx.h"
#include "AR.h"

CAR::CAR()
{

	m_attack = 0.05;
	m_release = 0.05;
	m_duration = 0.1;

}

void CAR::Start()
{

	m_node->SetSampleRate(GetSampleRate());
	m_node->Start();
	m_time = 0;

}

bool CAR::Generate()
{

	m_node->Generate();
	double time1 = m_duration - m_release;
	double time2 = m_duration - m_time;
	double t_frame0 = m_node->Frame(0) * m_time;
	double t_frame1 = m_node->Frame(1) * m_time;
	double d_frame0 = m_node->Frame(0) * time2;
	double d_frame1 = m_node->Frame(1) * time2;

	if (m_time < m_attack)
	{

		m_frame[0] = t_frame0 / m_attack;
		m_frame[1] = t_frame1 / m_attack;

	}
	else if (m_time > time1)
	{

		m_frame[0] = d_frame0 / m_release;
		m_frame[1] = d_frame1 / m_release;

	}
	else if (m_time > m_attack || m_time < time1)
	{

		m_frame[0] = m_node->Frame(0);
		m_frame[1] = m_node->Frame(1);

	}

	m_time += GetSamplePeriod();

	return m_time < m_duration;

}
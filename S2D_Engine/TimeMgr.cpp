#include "TimeMgr.h"

#include <stdio.h>

void TimeMgr::init()
{
	QueryPerformanceCounter(&m_llPrevCount);
	QueryPerformanceFrequency(&m_llFrequency);
}

void TimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;

	m_llPrevCount = m_llCurCount;
	++m_iCallCount;
	m_dAcc = m_dDT;

	if (m_dAcc >= 1.) {
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;
	}
}

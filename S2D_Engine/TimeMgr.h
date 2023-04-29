#pragma once

#include <Windows.h>

#define SINGLE(type) static type* GetInst()\
					{\
						static type mgr;\
						return &mgr;\
					}

class TimeMgr
{
public:
	SINGLE(TimeMgr);

private:
	LARGE_INTEGER m_llCurCount;
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llFrequency;

	double m_dDT;
	double m_dAcc;
	UINT m_iCallCount;
	UINT m_iFPS;

public:
	void init();
	void update();

public:
	double GetDT() {
		return m_dDT;
	}
	float GetFDT() {
		return (float)m_dDT;
	}


};


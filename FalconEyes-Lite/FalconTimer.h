/**
@class Span Time Class
@brief	
@remark	
-	���� 1
     : ��ü ���� -> ���� ���� �������� Reset() ȣ�� -> ���������� Elapse ȣ�� -> Elapse ���ϰ� ���
-	���� 2
	 : ��ü ���� -> ���� ���� �������� Reset() ȣ�� -> ���������� Elapse ȣ�� -> GetSpan ���ϰ� ���
@author	�����(Kyeong Kyu - Seon)
@date	2019/5/9  9:07
*/
#pragma once
#include <ctime>
class FalconTimer
{
public:
	FalconTimer() {}
	~FalconTimer() {}
private:
	clock_t m_clkStart = 0;
	clock_t m_clkEnd = 0;
	clock_t m_clkSpan = 0;
public:
	void Reset()
	{
		m_clkStart = clock();
		m_clkSpan = 0;
	}
	clock_t Elapse()
	{
		m_clkEnd = clock();
		m_clkSpan = m_clkEnd - m_clkStart;
		return m_clkSpan;
	}

	clock_t ElapseSec()
	{
		m_clkEnd = clock();
		m_clkSpan = m_clkEnd - m_clkStart;
		return static_cast<clock_t>(m_clkSpan / (double)CLOCKS_PER_SEC);
	}

	clock_t GetSpan()
	{
		return m_clkSpan;
	}

	clock_t GetSpanSec()
	{
		return static_cast<clock_t>(m_clkSpan / (double)CLOCKS_PER_SEC);
	}
};


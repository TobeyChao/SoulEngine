#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<mmsystem.h>
#include "CpuUse.h"
#pragma comment(lib, "Pdh.lib")

namespace Soul
{
	CPUUse::CPUUse()
		:
		mCpuUsage(0),
		mCanReadCPU(true),
		mLastSampleTime(0)
	{}
	void CPUUse::Initialize()
	{
		//����һ�����CPUʹ�õĲ�ѯ����
		PDH_STATUS status = PdhOpenQuery(NULL, 0, &mQueryHandle);
		if (status != ERROR_SUCCESS)
		{
			mCanReadCPU = false;
		}
		//���ü������������ϵͳ������CPU
		status = PdhAddCounter(mQueryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &mCounterHandle);
		if (status != ERROR_SUCCESS)
		{
			mCanReadCPU = false;
		}
		mLastSampleTime = GetTickCount64();
	}
	void CPUUse::ShutDown()
	{
		if (mCanReadCPU)
		{
			PdhCloseQuery(mQueryHandle);
		}
	}
	void CPUUse::Frame()
	{
		PDH_FMT_COUNTERVALUE value;
		if (mCanReadCPU)
		{
			if (GetTickCount64() > (mLastSampleTime + 1000))
			{
				mLastSampleTime = GetTickCount64();
				PdhCollectQueryData(mQueryHandle);
				PdhGetFormattedCounterValue(mCounterHandle, PDH_FMT_LONG, NULL, &value);
				mCpuUsage = value.longValue;
			}
		}
	}
	int CPUUse::GetCpuPerentage()
	{
		return mCanReadCPU ? (int)mCpuUsage : 0;
	}
}
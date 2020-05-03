#pragma once
#include <Pdh.h>
namespace Soul
{
	class CPUUse
	{
	public:
		CPUUse();
		void Initialize();
		void ShutDown();
		void Frame();
		int GetCpuPerentage();
	private:
		bool mCanReadCPU;
		HQUERY mQueryHandle;
		HCOUNTER mCounterHandle;
		unsigned long long mLastSampleTime;	//���Ĳ���ʱ��
		unsigned long long mCpuUsage;		//CPUʹ����
	};
}
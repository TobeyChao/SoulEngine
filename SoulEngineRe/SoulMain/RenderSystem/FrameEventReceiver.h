#pragma once
namespace Soul
{
	class FrameEventReceiver
	{
	public:
		FrameEventReceiver() {};
		virtual ~FrameEventReceiver() {}
		// ��֡�¼�������ɺ󷵻�true�����������Ϣ������false��������Ϣ
		virtual bool FrameStarted() { return true; }
		virtual bool FrameUpdated() { return true; }
		virtual bool FrameEnd() { return true; }
	};
}
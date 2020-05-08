#pragma once
#include "SoulPCH.h"
namespace Soul
{
	class Launcher : public Singleton<Launcher>
	{
	public:
		explicit Launcher(const std::string& moduleConfig);
		~Launcher();
		virtual RenderWindow* Initialize(const std::string& initConfig);

		void AddFrameEventReceiver(FrameEventReceiver* frameEvtReceiver);
		void RemoveFrameEventReceiver(FrameEventReceiver* frameEvtReceiver);
		// ���У���main��������
		void Run();
		// ��Ⱦ֡���ڲ���Ϸѭ��ÿ֡����
		bool RenderFrame();
		// ֪ͨ֡��ʼ
		bool FireFrameStarted();
		// ����֡���ڲ���Ϸѭ��ÿ֡����
		bool UpdateFrame();
		// ֪ͨ�������
		bool FireFrameUpdated();
		// ֪ͨ֡��Ⱦ���
		bool FireFrameEnded();
		// ������Ⱦ����
		RenderWindow* CreateRenderWindow(const json& config);
		// ��������������
		SceneManager* CreateSceneManager();
		/***********AudioSystem Start************/
		// ������Ƶϵͳ
		void AddAudioSystem(IAudio* audioSystem);
		// ��ȡ��ǰ����Ƶϵͳ
		IAudio* GetActiveAudioSystem() const;
		// ��ȡ��Ƶϵͳͨ������
		IAudio* GetAudioSystemByName(std::string audioSystemName);
		/************AudioSystem End*************/
		/***********RenderSystem Start************/
		void AddRenderSystem(RenderSystem* renderSystem);
		RenderSystem* GetActiveRenderSystem() const;
		RenderSystem* GetRenderSystemByName(std::string name);
		/***********RenderSystem End**************/
	private:
		/***********Module Start**************/
		void LoadModules(const std::string& moduleConfig);
		// �����������е���
		void UnLoadModules();
		void AddModule(Module* module);
		void RemoveModule(Module* module);
		/************Module End***************/
		// ͬ��֡�¼�������
		void SyncFrameEventReceiver();

	private:
		// ���ص������е�ģ��
		std::vector<Module*> mModules;
		// ��Ⱦϵͳ�б���initialize��������䣬��module_config.json�ļ�ָ��
		std::vector<RenderSystem*> mRenderSystems;
		// ��ǰ�������Ⱦϵͳ����Initialize�����и�ֵ����boot_config.json�ļ�ָ��
		RenderSystem* mActiveRenderSystem;
		// ֡�¼�������
		std::set<FrameEventReceiver*> mFrameEventReceivers;
		std::set<FrameEventReceiver*> mRemovedFrameEventReceivers;
		std::set<FrameEventReceiver*> mAddedFrameEventReceiver;
		// ��Ƶϵͳ�б�
		std::vector<IAudio*> mAudioSystems;
		// ��ǰ�������Ƶϵͳ
		IAudio* mActiveAudioSystems;
		// ��Ƶ��Դ������
		SoundResourceManager* mSoundResourceManager;
		// ��Դ�������
		ResourceGroupManager* mResourceGroupManager;
		// ģ������
		std::string mModuleConfig;
		// ������Ϸѭ��
		bool mQuit;
	};
}

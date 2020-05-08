#include "Launcher.h"

#include "../RenderSystem/DirectX11/D3D11RenderModule.h"
#include "../AudioSystem/DSound/DSoundModule.h"
#include "../AudioSystem/XAudio/XAudioModule.h"

#include "Audio/SoundResourceManager.h"
#include "RenderSystem/FrameEventReceiver.h"
#include "Scene/SceneManager.h"
#include <locale>

namespace Soul
{
	template<> Launcher* Singleton<Launcher>::mSingleton = nullptr;

	Launcher::Launcher(const std::string& moduleConfig)
		:
		mActiveRenderSystem(nullptr),
		mActiveAudioSystems(nullptr),
		mResourceGroupManager(nullptr),
		mSoundResourceManager(nullptr),
		mQuit(false)
	{
		if (!moduleConfig.empty())
		{
			LoadModules(moduleConfig);
		}
	}

	Launcher::~Launcher()
	{
		UnLoadModules();
		// ����mSoundResourceManager
		if (mSoundResourceManager)
		{
			delete mSoundResourceManager;
			mSoundResourceManager = nullptr;
		}
		// ����mResourceGroupManager
		if (mResourceGroupManager)
		{
			delete mResourceGroupManager;
			mResourceGroupManager = nullptr;
		}
	}

	RenderWindow* Launcher::Initialize(const std::string& initConfig)
	{
		mResourceGroupManager = new ResourceGroupManager();
		std::wifstream i(initConfig);
		std::locale china("zh_CN.UTF-8");
		i.imbue(china);
		std::wstringstream buffer;
		buffer << i.rdbuf();
		std::wstring contents(buffer.str());
		json j = json::parse(contents);
		i.close();
		// 1.��ʼ����Ⱦϵͳ
		if (j.contains("render_system"))
		{
			std::string renderSystemName = j["render_system"];
			// ��û�м������Ⱦϵͳ���ߵ�ǰ����Ⱦϵͳ����Ҫ�趨����Ⱦϵͳ��ʱ�����ü������Ⱦϵͳ
			if (!mActiveRenderSystem || mActiveRenderSystem->GetRenderSystemName() != renderSystemName)
			{
				mActiveRenderSystem = GetRenderSystemByName(renderSystemName);
			}
		}
		assert(mActiveRenderSystem != nullptr);
		mActiveRenderSystem->Initialize(initConfig);
		const auto config = GetActiveRenderSystem()->GetRenderWindowDescRef();
		RenderWindow* win = CreateRenderWindow(config);

		// 2.��ʼ����Ƶϵͳ
		if (j.contains("audio_system"))
		{
			std::string audioSystemName = j["audio_system"];
			// ��û�м������Ⱦϵͳ���ߵ�ǰ����Ⱦϵͳ����Ҫ�趨����Ⱦϵͳ��ʱ�����ü������Ⱦϵͳ
			if (!mActiveAudioSystems || mActiveAudioSystems->GetAudioSystemName() != audioSystemName)
			{
				mActiveAudioSystems = GetAudioSystemByName(audioSystemName);
			}
		}
		mSoundResourceManager = new SoundResourceManager();
		assert(mActiveAudioSystems != nullptr);
		mActiveAudioSystems->Initialize();

		return win;
	}

	void Launcher::AddFrameEventReceiver(FrameEventReceiver* frameEvtReceiver)
	{
		mAddedFrameEventReceiver.insert(frameEvtReceiver);
		mRemovedFrameEventReceivers.erase(frameEvtReceiver);
	}

	void Launcher::RemoveFrameEventReceiver(FrameEventReceiver* frameEvtReceiver)
	{
		mRemovedFrameEventReceivers.insert(frameEvtReceiver);
		mAddedFrameEventReceiver.erase(frameEvtReceiver);
	}

	void Launcher::SyncFrameEventReceiver()
	{
		for (auto i = mRemovedFrameEventReceivers.begin(); i != mRemovedFrameEventReceivers.end(); ++i)
		{
			mFrameEventReceivers.erase(*i);
		}
		mRemovedFrameEventReceivers.clear();

		for (auto i = mAddedFrameEventReceiver.begin(); i != mAddedFrameEventReceiver.end(); ++i)
		{
			mFrameEventReceivers.insert(*i);
		}
		mAddedFrameEventReceiver.clear();
	}

	void Launcher::Run()
	{
		if (!mActiveRenderSystem)
		{
			return;
		}

		mQuit = false;

		while (!mQuit)
		{
			if (!RenderFrame())
			{
				break;
			}
		}
	}

	bool Launcher::RenderFrame()
	{
		if (!FireFrameStarted())
		{
			return false;
		}
		if (!UpdateFrame())
		{
			return false;
		}
		return FireFrameEnded();
	}

	bool Launcher::FireFrameStarted()
	{
		SyncFrameEventReceiver();
		for (auto it = mFrameEventReceivers.begin(); it != mFrameEventReceivers.end(); it++)
		{
			if (!(*it)->FrameStarted())
			{
				return false;
			}
		}
		return true;
	}

	bool Launcher::UpdateFrame()
	{
		mActiveRenderSystem->UpdateRenderTarget();
		bool result = FireFrameUpdated();
		mActiveRenderSystem->SwapRenderTargetBuffer();
		return result;
	}

	bool Launcher::FireFrameUpdated()
	{
		SyncFrameEventReceiver();
		for (auto it = mFrameEventReceivers.begin(); it != mFrameEventReceivers.end(); it++)
		{
			if (!(*it)->FrameUpdated())
			{
				return false;
			}
		}
		return true;
	}

	bool Launcher::FireFrameEnded()
	{
		SyncFrameEventReceiver();
		for (auto it = mFrameEventReceivers.begin(); it != mFrameEventReceivers.end(); it++)
		{
			if (!(*it)->FrameEnd())
			{
				return false;
			}
		}
		return true;
	}

	void Launcher::AddAudioSystem(IAudio* audioSystem)
	{
		mAudioSystems.push_back(audioSystem);
	}

	IAudio* Launcher::GetActiveAudioSystem() const
	{
		return mActiveAudioSystems;
	}

	IAudio* Launcher::GetAudioSystemByName(std::string audioSystemName)
	{
		if (audioSystemName.empty())
		{
			return nullptr;
		}
		for (auto it = mAudioSystems.begin(); it != mAudioSystems.end(); it++)
		{
			if ((*it)->GetAudioSystemName() == audioSystemName)
			{
				return *it;
			}
		}
		return nullptr;
	}

	void Launcher::AddRenderSystem(RenderSystem* renderSystem)
	{
		mRenderSystems.push_back(renderSystem);
	}

	RenderSystem* Launcher::GetActiveRenderSystem() const
	{
		return mActiveRenderSystem;
	}

	RenderSystem* Launcher::GetRenderSystemByName(std::string name)
	{
		if (name.empty())
		{
			return nullptr;
		}
		for (auto it = mRenderSystems.begin(); it != mRenderSystems.end(); it++)
		{
			if ((*it)->GetRenderSystemName() == name)
			{
				return *it;
			}
		}
		return nullptr;
	}

	RenderWindow* Launcher::CreateRenderWindow(const json& config)
	{
		RenderWindow* win;
		win = mActiveRenderSystem->CreateRenderWindow(config);
		return win;
	}

	SceneManager* Launcher::CreateSceneManager()
	{
		return new SceneManager();
	}

	void Launcher::LoadModules(const std::string& moduleConfig)
	{
		std::ifstream i(moduleConfig);
		json j;
		i >> j;
		i.close();
		if (j.find("modules") != j.end())
		{
			std::vector<std::string> modules = j["modules"];
			std::for_each(modules.begin(), modules.end(), [this](const std::string& module) {
				if (module == "dx11_render_system")
				{
					AddModule(new D3D11RenderModule());
				}
				else if (module == "dsound")
				{
					AddModule(new DSoundModule());
				}
				else if (module == "xaudio2")
				{
					AddModule(new XAudioModule());
				}
				else
				{

				}
				});
		}
	}

	void Launcher::UnLoadModules()
	{
		for (auto it = mModules.rbegin(); it != mModules.rend(); it++)
		{
			(*it)->Uninitialize();
			delete (*it);
		}
		mModules.clear();
	}

	void Launcher::AddModule(Module* module)
	{
		mModules.push_back(module);
		module->Initialize();
	}

	void Launcher::RemoveModule(Module* module)
	{
		auto it = std::find(mModules.begin(), mModules.end(), module);
		if (it != mModules.end())
		{
			module->Uninitialize();
			mModules.erase(it);
		}
	}
}
#pragma once
#include "RenderWindow.h"
#include "RenderParameter.h"
#include "../Core/SColorf.h"
#include "../Resource/ResourceGroupManager.h"
#include "../Resource/TextureManager.h"
#include "GPUBufferManager.h"
#include "ShaderManager.h"

namespace Soul
{
	class RenderSystem
	{
	public:
		RenderSystem();
		virtual ~RenderSystem();
		// ��ʼ����������Ҫ������ø�ֵ
		virtual void Initialize(const std::string& initConfig);
		[[nodiscard]] virtual const std::string& GetRenderSystemName() const = 0;
		virtual RenderWindow* CreateRenderWindow(const json& config) = 0;
		// ��ȡ��Ⱦ��������
		json& GetRenderWindowDescRef();
		// ����RenderTarget
		virtual void SetRenderTarget(RenderTarget* renderTarget);
		void AddRenderTarget(RenderTarget* renderTarget);
		void RemoveRenderTarget(const std::string& name);
		RenderTarget* GetRenderTargetByName(const std::string& name) const;
		// ����Viewport
		virtual void SetViewport(Viewport* viewport) = 0;
		// ����Render Target����SAdmin����
		void UpdateRenderTarget();
		// ����Buffer
		void SwapRenderTargetBuffer();
		// ��������
		virtual void SetTexture(size_t slot, ITexture* texture) = 0;
		// ���û��
		virtual void SetBlendType(const BlendType& bt) = 0;
		// ���ù�դ��
		virtual void SetRasterizerType(const RasterizerType& rt) = 0;
		// �������ģ��
		virtual void SetDepthStencilType(const DepthStencilType& dst) = 0;

		virtual void SetStencilRef(UINT stencilRef) = 0;

		// ������ɫ��
		virtual void BindShader(Shader* shader) = 0;

		virtual void Render(const RenderParameter& rp);
	protected:
		// ����
		json mConfig;
		// ���е�RenderTargert
		std::map<std::string, RenderTarget*> mRenderTargets;
		// ��ǰ�����RenderTarget
		RenderTarget* mActiveRenderTarget;
		// �ӿ�
		Viewport* mActiveViewport;
		// ��ɫ��������
		ShaderManager* mShaderManager;
		// ���������
		TextureManager* mTextureManager;
		// GPUBuffer������
		GPUBufferManager* mGpuBufferManager;
	};
}
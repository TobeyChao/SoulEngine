#pragma once
#include "D3D11RenderSystem.h"
#include "D3D11RenderTarget.h"

namespace Soul
{
	class D3D11Texture final : public ITexture
	{
	public:
		D3D11Texture(D3D11Device& device, const std::wstring& filePath);
		~D3D11Texture();
		[[nodiscard]] ID3D11ShaderResourceView* GetTextureSRV() const;
	private:
		Microsoft::WRL::ComPtr<ID3D11Resource> mTex;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureSRV;
		D3D11Device& mD3DDevice;
	};

	class D3D11RenderTexture : public ITexture, public RenderTarget, public D3D11RenderTarget
	{
	public:
		explicit D3D11RenderTexture(D3D11Device& device, const std::wstring& textureName, const Core::SDimension2& imageSize, bool generateMips = false);
		~D3D11RenderTexture();

		void Clear() override
		{
			mD3DDevice.GetDeviceContext()->ClearRenderTargetView(mOutputTextureRTV.Get(), mClearColor.Get());
			mD3DDevice.GetDeviceContext()->ClearDepthStencilView(mOutputTextureDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}

		void SwapBuffers() override
		{

		}

		ID3D11RenderTargetView* GetRenderTargetView() const override
		{
			return mOutputTextureRTV.Get();
		}

		ID3D11DepthStencilView* GetDepthStencilView() const override
		{
			return mOutputTextureDSV.Get();
		}

		ID3D11ShaderResourceView* GetOutputTexture()
		{
			return mOutputTextureSRV.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mOutputTextureSRV;// ����������Ӧ����ɫ����Դ��ͼ
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mOutputTextureRTV;	// ����������Ӧ����ȾĿ����ͼ
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mOutputTextureDSV;	// ����������õ����/ģ����ͼ
		bool mGenerateMips; // �Ƿ�����mipmap��
		D3D11Device& mD3DDevice;
	};
}

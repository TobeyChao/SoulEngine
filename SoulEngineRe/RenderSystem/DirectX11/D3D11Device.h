#pragma once
#include "D3D11PCH.h"
#include <cassert>
namespace Soul
{
	class D3D11Device
	{
	public:
		D3D11Device();
		~D3D11Device();
		HRESULT Initialize();
		ID3D11Device* operator->() const
		{
			assert(mDevice);
			return mDevice.Get();
		}
		inline IDXGIFactory1* GetDXGIFactory()
		{
			return mDXGIFactory.Get();
		}
		inline IDXGIAdapter* GetDXGIAdapter()
		{
			return mDXGIAdapter.Get();
		}
		inline ID3D11DeviceContext* GetDeviceContext()
		{
			return mDeviceContext.Get();
		}
	private:
		D3D11Device(const D3D11Device& device) = delete;
		const D3D11Device& operator=(D3D11Device& device) = delete;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device>			mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mDeviceContext;
		Microsoft::WRL::ComPtr<IDXGIFactory1>			mDXGIFactory;
		Microsoft::WRL::ComPtr<IDXGIAdapter1>			mDXGIAdapter;
	};
}
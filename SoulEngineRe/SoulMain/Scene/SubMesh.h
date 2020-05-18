#pragma once
#include "../SoulPCH.h"
#include "PrimitiveTypes.h"
#include "../RenderSystem/RenderParameter.h"
#include "../RenderSystem/GPUBufferManager.h"

namespace Soul
{
	enum class SIMPLE_GAMEOBJECT
	{
		SG_CUBE,
		SG_SPHERE,
		SG_PLANE,
		SG_CYLINDER,
		SG_LINE3D,
		SG_POINT3D
	};
	struct Material
	{
		Core::SVector4 ambient;
		Core::SVector4 diffuse;
		Core::SVector4 specular; // w = ���淴��ǿ��
		Core::SVector4 reflect;
	};
	class SubMesh
	{
	public:
		SubMesh(const std::string& name);

		virtual ~SubMesh();

		RenderParameter& GetRenderParameter()
		{
			return *mRenderParameter;
		}

		// ���㻺��
		virtual void InitializeBuffer();

		template<typename VertexType>
		void CreateBuffer(const GPU_BUFFER_TYPE& gbtVertex)
		{
			VertexType* vertices = new VertexType[mRenderParameter->mVertexCount];
			//��䶥��
			for (size_t i = 0; i < (*mOringinMeshData).Vertices.size(); ++i)
			{
				if (VertexType::hasPos)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::PosStartByte, sizeof(Core::SVector3),
						&((*mOringinMeshData).Vertices[i].Position), sizeof(Core::SVector3));
				}
				if (VertexType::hasTex)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::TexStartByte, sizeof(Core::SVector2),
						&((*mOringinMeshData).Vertices[i].TexCoord), sizeof(Core::SVector2));
					;
				}
				if (VertexType::hasNor)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::NorStartByte, sizeof(Core::SVector3),
						&((*mOringinMeshData).Vertices[i].Normal), sizeof(Core::SVector3));
				}
				if (VertexType::hasCol)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::ColStartByte, sizeof(Core::SVector4),
						&((*mOringinMeshData).Vertices[i].Color), sizeof(Core::SVector4));
				}
				if (VertexType::hasTan)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::TanStartByte, sizeof(Core::SVector3),
						&((*mOringinMeshData).Vertices[i].TangentU), sizeof(Core::SVector3));
				}
			}
			mRenderParameter->mSizePerVertex = sizeof(VertexType);
			//�������㻺��
			mRenderParameter->mVertexBuffer =
				GPUBufferManager::GetInstance().
				CreateGPUBuffer(gbtVertex,
					vertices,
					sizeof(VertexType) * mRenderParameter->mVertexCount);
			//������������
			mRenderParameter->mIndexBuffer =
				GPUBufferManager::GetInstance().
				CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_INDEX,
					&(*mOringinMeshData).Indices[0],
					sizeof(unsigned int) * mRenderParameter->mIndicesCount);
			delete[] vertices;
			vertices = nullptr;
		}

		virtual void UpdateBuffer() {};

		void EnableShadow(bool enableShadow)
		{
			mEnableShadow = enableShadow;
		}

		bool IsEnableShadow()
		{
			return mEnableShadow;
		}

		void EnableReflect(bool enableReflect)
		{
			mEnableReflect = enableReflect;
		}

		bool IsEnableReflect()
		{
			return mEnableReflect;
		}

		// ����
		void PushTexture(ITexture* texture)
		{
			mTextures.push_back(texture);
		}

		// Լ��
		// ��һλΪ��ͨ��ͼ
		// �ڶ�λΪ������ͼ
		std::vector<ITexture*>& GetTextures()
		{
			return mTextures;
		}

		void ClearTexture()
		{
			mTextures.clear();
		}

		// Shader
		void SetShader(Shader* shader)
		{
			mShader = shader;
		}

		Shader* GetShader()
		{
			return mShader;
		}

		// ����
		Material* GetMaterial() const
		{
			return mMaterial;
		}

		void SetMaterial(Material* newMaterial)
		{
			if (mMaterial)
			{
				delete mMaterial;
				mMaterial = nullptr;
			}
			mMaterial = newMaterial;;
		}

		// Mesh
		MeshData* GetOriginalMeshDataPtr()
		{
			return mOringinMeshData;
		}

		// ���ģ��
		void SetDepthStencil(const DepthStencilType& dst, UINT stencilRef)
		{
			mUseDepthStencil = true;
			mDepthStencilType = dst;
			mStencilRef = stencilRef;
		}

		bool UseDepthStencil()
		{
			return mUseDepthStencil;
		}

		UINT GetStencilRef()
		{
			return mStencilRef;
		}

		const DepthStencilType& GetDepthStencilType()
		{
			return mDepthStencilType;
		}

		// ��դ��
		void SetRasterizer(const RasterizerType& dst)
		{
			mUseRasterizer = true;
			mRasterizerType = dst;
		}

		bool UseRasterizer()
		{
			return mUseRasterizer;
		}

		const RasterizerType& GetRasterizerType()
		{
			return mRasterizerType;
		}

		// ���
		void SetBlend(const BlendType& dst)
		{
			mUseBlend = true;
			mBlendType = dst;
		}

		bool UseBlend()
		{
			return mUseBlend;
		}

		const BlendType& GetBlendType()
		{
			return mBlendType;
		}

		void SetParent(GameObject* gameObject)
		{
			mParent = gameObject;
		}

		GameObject* GetParent()
		{
			return mParent;
		}

	protected:
		std::string mName;

		bool mEnableShadow;
		bool mEnableReflect;

		Core::SMatrix4x4 mShadowMat;
		Core::SVector4 mShadowPlane;

		// �Ƿ��Զ�����
		bool mUseBlend;
		BlendType mBlendType;

		// �Ƿ��Զ����դ��
		bool mUseRasterizer;
		RasterizerType mRasterizerType;

		// �Ƿ��Զ������ģ��
		bool mUseDepthStencil;
		DepthStencilType mDepthStencilType;
		UINT mStencilRef;

		// ��Ҫ��Ⱦ�Ķ��㻺��
		RenderParameter* mRenderParameter;
		// Texture ���ܻ��õ���������
		std::vector<ITexture*> mTextures;
		// Bind Shader
		Shader* mShader;
		// ����
		Material* mMaterial;
		// ԭʼ��������
		MeshData* mOringinMeshData;
		// ������Ϸ����
		GameObject* mParent;
	};
}
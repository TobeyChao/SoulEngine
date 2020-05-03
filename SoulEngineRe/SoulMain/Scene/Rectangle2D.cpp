#include "Rectangle2D.h"
#include "../RenderSystem/ShaderManager.h"
#include "../RenderSystem/RenderParameter.h"
#include "../RenderSystem/GPUBufferManager.h"
#include "PrimitiveTypes.h"

namespace Soul
{
	Rectangle2D::Rectangle2D(std::string name, const Core::SDimension2& size)
		:
		SubMesh(name),
		mSize(size)
	{
		//����Shader
		Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Color");
		SetShader(shader);
	}

	void Rectangle2D::SetShader(Shader* shader)
	{
		SubMesh::SetShader(shader);
		mRenderParameter->mVertexCount = 4u;
		mRenderParameter->mIndicesCount = 6u;
		//���嶥��
		ColorTextureVertex* vertices = new ColorTextureVertex[4u];
		vertices[0] = ColorTextureVertex(-(float)mSize.width / 2.0f, -(float)mSize.height / 2.0f, -1.0f, 0.0f, 1.0f, 1.0, 1.0, 1.0, 1.0f);
		vertices[1] = ColorTextureVertex(-(float)mSize.width / 2.0f, (float)mSize.height / 2.0f, -1.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0, 1.0f);
		vertices[2] = ColorTextureVertex((float)mSize.width / 2.0f, (float)mSize.height / 2.0f, -1.0f, 1.0f, 0.0f, 1.0, 1.0, 1.0, 1.0f);
		vertices[3] = ColorTextureVertex((float)mSize.width / 2.0f, -(float)mSize.height / 2.0f, -1.0f, 1.0f, 1.0f, 1.0, 1.0, 1.0, 1.0f);
		//��������
		unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
		mRenderParameter->mSizePerVertex = sizeof(ColorTextureVertex);
		//�������㻺��
		mRenderParameter->mVertexBuffer =
			GPUBufferManager::GetInstance().
			CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_VERTEX,
				vertices,
				sizeof(ColorTextureVertex) * mRenderParameter->mVertexCount);
		//������������
		mRenderParameter->mIndexBuffer =
			GPUBufferManager::GetInstance().
			CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_INDEX,
				&indices[0],
				sizeof(unsigned int) * mRenderParameter->mIndicesCount);
	}

	//void SRectangle2D::Render(const Scene::ISceneNode * sceneNode)
	//{
	//	//��Ⱦ����-1-����װ��׶�
	//	UINT stride = sizeof(ColorTextureVertex);
	//	UINT offset = 0u;
	//	ID3D11DeviceContext *deviceContext =
	//		((Graphics::CD3D11Driver*)sceneNode->GetSceneManager()->GetGraphicsDriver())->GetD3D11DeviceContext();
	//	Scene::ISceneNodeCamera* camera = sceneNode->GetSceneManager()->GetActiveCamera();
	//	//���ö��㻺��
	//	deviceContext->IASetVertexBuffers(0u, 1u, &mVertBuffer, &stride, &offset);
	//	//������������
	//	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0u);
	//	//���û�Ԫ����
	//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	//�ر�z������
	//	((Graphics::CD3D11Driver*)sceneNode->GetSceneManager()->GetGraphicsDriver())->TurnOffZBuffer();
	//	//��Ⱦscenenode
	//	mShader->RenderNode(mIndicesCount, sceneNode->GetAbsoluteTransformation(), camera->GetViewMatrix(true), camera->GetProjectionMatrix(true), mMaterial);
	//	//����z������
	//	((Graphics::CD3D11Driver*)sceneNode->GetSceneManager()->GetGraphicsDriver())->TurnOnZBuffer();
	//}
}

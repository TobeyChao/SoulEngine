#include "ParticalList.h"

namespace Soul
{
	ParticalList::ParticalList(const std::string& name)
		:
		SubMesh(name)
	{
	}
	void ParticalList::InitializeBuffer()
	{
		if (!mShader)
		{
			return;
		}
		mRenderParameter->mVertexCount = (unsigned int)(*mOringinMeshData).Vertices.size();
		mRenderParameter->mIndicesCount = (unsigned int)(*mOringinMeshData).Indices.size();
		CreateBuffer<ColorTextureVertex>(GPU_BUFFER_TYPE::GBT_VERTEX_DYNAMIC);
	}
	void ParticalList::UpdateBuffer()
	{
		ColorTextureVertex* vertices = new ColorTextureVertex[mRenderParameter->mVertexCount];
		//��䶥��
		for (size_t i = 0; i < (*mOringinMeshData).Vertices.size(); ++i)
		{
			vertices[i].Position = (*mOringinMeshData).Vertices[i].Position;
			vertices[i].Color = (*mOringinMeshData).Vertices[i].Color;
			vertices[i].TexCoord = (*mOringinMeshData).Vertices[i].TexCoord;
		}
		mRenderParameter->mVertexBuffer->UpdateBuffer(vertices, sizeof(ColorTextureVertex) * mRenderParameter->mVertexCount);
		delete[] vertices;
		vertices = nullptr;
	}
}
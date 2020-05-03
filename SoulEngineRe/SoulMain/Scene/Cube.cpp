#include "Cube.h"
#include "../RenderSystem/ShaderManager.h"
#include "../RenderSystem/GPUBufferManager.h"
#include "../RenderSystem/RenderParameter.h"
#include "GeometryGenerator.h"
#include "PrimitiveTypes.h"

namespace Soul
{
	Cube::Cube(std::string name)
		:
		SubMesh(name)
	{
		// Ĭ�ϲ���
		mMaterial = new Material();
		mMaterial->ambient = { 0.7250f, 0.7100f, 0.6800f, 1.0f };
		mMaterial->diffuse = { 0.7250f, 0.7100f, 0.6800f, 1.0f };
		mMaterial->specular = { 0.5f, 0.5f, 0.5f, 5.0f };
		// Ĭ����ɫ��
		Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Basic");
		SetShader(shader);
	}
}

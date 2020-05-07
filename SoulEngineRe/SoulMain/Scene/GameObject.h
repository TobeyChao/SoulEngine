#pragma once
#include "../SoulPCH.h"
#include "SubMesh.h"

namespace Soul
{
	class GameObject
	{
	public:
		explicit GameObject(const std::string& name);

		virtual ~GameObject();
		
		void SetSceneNodeBelongsTo(SceneNode* sceneNode) { mSceneNodeBelongsTo = sceneNode; }

		SceneNode* GetSceneNodeBelongsTo() const { return mSceneNodeBelongsTo; }

		void PushSubMesh(SubMesh* subMesh)
		{
			mSunMeshes.push_back(subMesh);
			subMesh->SetParent(this);
		}

		std::vector<SubMesh*>& GetAllSubMesh() { return mSunMeshes; }

		SubMesh* GetSubMesh(const unsigned subMeshIndex)
		{
			if (subMeshIndex < mSunMeshes.size())
			{
				return mSunMeshes[subMeshIndex];
			}
			return nullptr;
		}

		// ��ȡ��Χ�У�Ĭ�ϲ���������任
		BoundingBox GetBoundingBox(bool local = false) const;
		
		// ����SubMesh������Χ��
		void BuildBoundingBox();
		
	protected:
		std::string mName;
		// ��SceneManager����
		SceneNode* mSceneNodeBelongsTo;
		// ��Χ��
		BoundingBox* mBoundingBox;
		// ����������ͷ�
		std::vector<SubMesh*> mSunMeshes;
	};
}
#pragma once
#include "../SoulPCH.h"
#include "SubMesh.h"

namespace Soul
{
	class GameObject
	{
	public:
		GameObject(const std::string& name);

		virtual ~GameObject();
		
		void SetSceneNodeBelongsTo(SceneNode* sceneNode)
		{
			mSceneNodeBelongsTo = sceneNode;
		}

		SceneNode* GetSceneNodeBelongsTo()
		{
			return mSceneNodeBelongsTo;
		}

		void PushSubMesh(SubMesh* subMesh)
		{
			mSunMeshs.push_back(subMesh);
			subMesh->SetParent(this);
		}

		std::vector<SubMesh*>& GetAllSubMesh()
		{
			return mSunMeshs;
		}

		SubMesh* GetSubMesh(unsigned subMeshIndex)
		{
			if (subMeshIndex < mSunMeshs.size())
			{
				return mSunMeshs[subMeshIndex];
			}
			return nullptr;
		}

	protected:
		std::string mName;
		// ��SceneManager����
		SceneNode* mSceneNodeBelongsTo;
		// ����������ͷ�
		std::vector<SubMesh*> mSunMeshs;
	};
}
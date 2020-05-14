#include "GameObject.h"
#include "BoundingBox.h"
#include "SceneNodeCamera.h"

namespace Soul
{
	GameObject::GameObject(const std::string& name)
		:
		mName(name),
		mSceneNodeBelongsTo(nullptr),
		mBoundingBox(nullptr)
	{
	}

	GameObject::~GameObject()
	{
		for (auto it : mSunMeshes)
		{
			auto sm = it;
			if (sm)
			{
				delete sm;
				sm = nullptr;
			}
		}
		mSunMeshes.clear();
	}
	BoundingBox GameObject::GetBoundingBox(bool local) const
	{
		// û�а󶨵ĳ���NodeҲ�᷵�ؾֲ�����ϵ��Χ��
		if (local || !mSceneNodeBelongsTo)
		{
			return *mBoundingBox;
		}
		BoundingBox out;
		if (mBoundingBox)
		{
			mBoundingBox->Transform(out, mSceneNodeBelongsTo->GetAbsoluteTransformation());
		}
		return out;
	}
	void GameObject::BuildBoundingBox()
	{
		// ûMesh������
		if (mSunMeshes.empty())
		{
			return;
		}
		// MeshӦ����������С������
		mBoundingBox = new BoundingBox();
		const auto meshData = mSunMeshes[0]->GetOriginalMeshDataPtr();
		BoundingBox::CreateFromPoints(*mBoundingBox, meshData->Min, meshData->Max);
	}
}
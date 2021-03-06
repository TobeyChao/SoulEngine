#include "SceneNodeLight.h"
#include "SceneManager.h"
#include "Light.h"

namespace Soul
{
	SceneNodeLight::SceneNodeLight(SceneNode* parent, SceneManager* mgr, size_t id)
		:
		SceneNode(parent, mgr, id)
	{}

	void SceneNodeLight::OnRegisterSceneNode()
	{
		if (mSceneManager->RegisterNode(this, E_SCENENODE_TYPES::EST_LIGHT))
			mIsRegister = true;
		SceneNode::OnRegisterSceneNode();
	}

	void SceneNodeLight::ProcessVisibleGameObject()
	{
		UpdateAbsolutePosition();
		for (auto& gameObject : mAttachedGameObject)
		{
			Light* light = dynamic_cast<Light*>(gameObject);
			if (light)
			{
				mSceneManager->EnqueueLightQueue(light);
			}
		}
	}
}

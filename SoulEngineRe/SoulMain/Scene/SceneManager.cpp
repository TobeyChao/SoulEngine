#include "SceneManager.h"
#include "SceneNodeCamera.h"
#include "GameObject.h"

#include "SubMesh.h"
#include "Cube.h"
#include "Sphere.h"
#include "Plane.h"
#include "Line3D.h"
#include "Point3D.h"

#include "Light.h"

#include "../Launcher.h"
#include "../RenderSystem/RenderSystem.h"
#include "../Resource/ObjLoader.h"
#include "GeometryGenerator.h"

namespace Soul
{
	SceneManager::SceneManager()
		:
		SceneNode(nullptr, nullptr),
		mActiveCamera(nullptr),
		mRenderSystem(nullptr),
		mActiveViewport(nullptr)
	{
		mSceneManager = this;
		const auto admin = Launcher::GetInstancePtr();
		if (admin)
		{
			mRenderSystem = Launcher::GetInstance().GetActiveRenderSystem();
		}
	}

	SceneManager::~SceneManager()
	{
		mCameraNodeList.clear();
		mSkyBoxNodeList.clear();
		mLightNodeList.clear();
		mSolidNodeList.clear();
	}

	SceneNodeCamera* SceneManager::AddCameraSceneNode(SceneNode* parent,
		const Core::SVector3& position, const Core::SVector3& lookAt, size_t id, bool makeActive)
	{
		if (!parent)
			parent = this;

		const auto node = new SceneNodeCamera(parent, this, id, position, lookAt);

		if (makeActive)
			SetActiveCamera(node);

		return node;
	}

	SceneNode* SceneManager::AddSkyBoxSceneNode(ITexture* texture, SceneNode* parent, size_t id)
	{
		return nullptr;
	}

	SceneNode* SceneManager::GetRootSceneNode()
	{
		return this;
	}

	SceneNode* SceneManager::GetSceneNodeFromId(int id, SceneNode* start)
	{
		return nullptr;
	}

	SceneNode* SceneManager::GetSceneNodeFromName(const std::wstring& name, SceneNode* start)
	{
		return nullptr;
	}

	SceneNodeCamera* SceneManager::GetActiveCamera() const
	{
		return mActiveCamera;
	}

	void SceneManager::SetActiveCamera(SceneNodeCamera* camera)
	{
		mActiveCamera = camera;
	}

	unsigned int SceneManager::RegisterNode(SceneNode* node, E_SCENENODE_TYPES pass)
	{
		unsigned int taken = 0u;
		switch (pass)
		{
		case E_SCENENODE_TYPES::EST_SKYBOX:
			mSkyBoxNodeList.push_back(node);
			taken = 1;
			break;
		case E_SCENENODE_TYPES::EST_CAMERA:
			taken = 1;
			for (size_t i = 0; i != mCameraNodeList.size(); ++i)
			{
				if (mCameraNodeList[i] == node)
				{
					taken = 0;
					break;
				}
			}
			if (taken)
			{
				mCameraNodeList.push_back(node);
			}
			break;
		case E_SCENENODE_TYPES::EST_SOLID:
			mSolidNodeList.push_back(node);
			taken = 1;
			break;
		case E_SCENENODE_TYPES::EST_LIGHT:
			mLightNodeList.push_back(node);
			taken = 1;
			break;
		default:
			break;
		}
		return taken;
	}

	GameObject* SceneManager::CreateGameObject()
	{
		return CreateGameObject("GameObject");
	}

	GameObject* SceneManager::CreateGameObject(const std::string& name)
	{
		return new GameObject(name);
	}

	GameObject* SceneManager::CreateGameObject(const std::string& name, SIMPLE_GAMEOBJECT simpleGameObject)
	{
		GameObject* newGameObject = nullptr;
		SubMesh* subMesh = nullptr;
		GeometryGenerator geoGen;
		switch (simpleGameObject)
		{
		case SIMPLE_GAMEOBJECT::SG_CUBE:
			newGameObject = new GameObject(name);
			subMesh = new Cube(name);
			newGameObject->PushSubMesh(subMesh);
			geoGen.CreateBox(1.f, 1.f, 1.f, *subMesh->GetOriginalMeshDataPtr());
			subMesh->InitializeBuffer();
			break;
		case SIMPLE_GAMEOBJECT::SG_PLANE:
			newGameObject = new GameObject(name);
			subMesh = new Plane(name);
			newGameObject->PushSubMesh(subMesh);
			geoGen.CreateGrid(1.f, 1.f, 2, 2, *subMesh->GetOriginalMeshDataPtr());
			subMesh->InitializeBuffer();
			break;
		case SIMPLE_GAMEOBJECT::SG_SPHERE:
			newGameObject = new GameObject(name);
			subMesh = new Sphere(name);
			newGameObject->PushSubMesh(subMesh);
			geoGen.CreateSphere(1.f, 60, 30, *subMesh->GetOriginalMeshDataPtr());
			subMesh->InitializeBuffer();
			break;
		case SIMPLE_GAMEOBJECT::SG_LINE3D:
			newGameObject = new GameObject(name);
			subMesh = new Line3D(name, { -1, 0, 0 }, { 1, 0, 0 });
			newGameObject->PushSubMesh(subMesh);
			break;
		case SIMPLE_GAMEOBJECT::SG_POINT3D:
			newGameObject = new GameObject(name);
			subMesh = new Point3D(name);
			newGameObject->PushSubMesh(subMesh);
			break;
		}
		return newGameObject;
	}

	GameObject* SceneManager::CreateGameObject(const std::string& name, const std::wstring& meshFilePath)
	{
		ObjLoader loader;
		loader.LoadObjModel(meshFilePath);
		auto& subMeshes = loader.GetSubMeshes();
		if (subMeshes.empty())
		{
			loader.Clean();
			return nullptr;
		}
		GameObject* newGameObject = new GameObject(name);
		for (auto it : subMeshes)
		{
			it->SetShader(ShaderManager::GetInstance().GetShaderByName(L"Basic"));
			it->InitializeBuffer();
			newGameObject->PushSubMesh(it);
		}
		loader.Clean();
		return newGameObject;
	}

	Light* SceneManager::CreateLight(const std::string& name, LIGHT_TYPE lightType)
	{
		Light* light = nullptr;
		switch (lightType)
		{
		case LIGHT_TYPE::LT_DIRECTIONAL:
			light = new Light(name, lightType);
			break;
		case LIGHT_TYPE::LT_POINT:
			light = new Light(name, lightType);
			break;
		case LIGHT_TYPE::LT_SPOT:
			light = new Light(name, lightType);
			break;
		default:
			break;
		}
		return light;
	}

	void SceneManager::SetViewport(Viewport* viewport)
	{
		mActiveViewport = viewport;
		mRenderSystem->SetViewport(viewport);
	}

	void SceneManager::Render()
	{
	}

	void SceneManager::DrawAll(SceneNodeCamera* camera, Viewport* viewport)
	{
		SetActiveCamera(camera);

		mActiveViewport = viewport;

		SetViewport(viewport);

		OnRegisterSceneNode();

		mRenderSystem->Clear({ 0.14f, 0.14f, 0.152f, 1.0f });

		for (auto& i : mLightNodeList)
		{
			i->Render();
		}

		for (auto& i : mSolidNodeList)
		{
			i->Render();
		}

		while (!mAllAttachedGameObject.empty())
		{
			SubMesh* sm = mAllAttachedGameObject.front();
			RenderParameter* rp = sm->GetRenderParameter();
			//����SceneNode����Shader����ر���
			Shader* shader = sm->GetShader();
			if (shader)
			{
				// �任����
				shader->SetWorld(sm->GetParent()->GetSceneNodeBelongsTo()->GetAbsoluteTransformation());
				shader->SetWorldViewProj(
					sm->GetParent()->GetSceneNodeBelongsTo()->GetAbsoluteTransformation() *
					GetActiveCamera()->GetViewMatrix() *
					GetActiveCamera()->GetProjectionMatrix());
				// ����
				if (sm->GetTextures().size() > 0)
				{
					shader->SetUseTexture(true);
					auto& textures = sm->GetTextures();
					for (size_t i = 0; i < textures.size(); i++)
					{
						mRenderSystem->SetTexture(i, textures[i]);
					}
				}
				else
				{
					shader->SetUseTexture(false);
				}
				// �ƹ�
				int slotD = 0;
				int slotP = 0;
				int slotS = 0;
				for (size_t i = 0; i < mLightList.size(); i++)
				{
					LIGHT_TYPE lt = mLightList[i]->GetType();
					if (lt == LIGHT_TYPE::LT_DIRECTIONAL)
					{
						DirectionalLight dl;
						dl.Ambient = mLightList[i]->GetAmbient();
						dl.Diffuse = mLightList[i]->GetDiffuse();
						dl.Specular = mLightList[i]->GetSpecular();
						dl.Direction = mLightList[i]->GetDirection();
						shader->SetDirectinalLight(slotD++, dl);
					}
					else if (lt == LIGHT_TYPE::LT_POINT)
					{
						PointLight pl;
						pl.Ambient = mLightList[i]->GetAmbient();
						pl.Diffuse = mLightList[i]->GetDiffuse();
						pl.Specular = mLightList[i]->GetSpecular();
						pl.Att = mLightList[i]->GetAtt();
						pl.Position = mLightList[i]->GetPosition();
						pl.Range = mLightList[i]->GetRange();
						shader->SetPointLight(slotP++, pl);
					}
					else if (lt == LIGHT_TYPE::LT_SPOT)
					{
						SpotLight sl;
						sl.Spot = mLightList[i]->GetSpot();
						sl.Ambient = mLightList[i]->GetAmbient();
						sl.Diffuse = mLightList[i]->GetDiffuse();
						sl.Specular = mLightList[i]->GetSpecular();
						sl.Att = mLightList[i]->GetAtt();
						sl.Position = mLightList[i]->GetPosition();
						sl.Range = mLightList[i]->GetRange();
						sl.Direction = mLightList[i]->GetDirection();
						shader->SetSpotLight(slotS++, sl);
					}
				}
				shader->SetPointLightNum(slotP);
				shader->SetDirectinalLightNum(slotD);
				shader->SetSpotLightNum(slotS);

				// ����
				shader->SetMaterial(*(sm->GetMaterial()));
				mRenderSystem->BindShader(shader);
			}
			else
			{
				mRenderSystem->BindShader(nullptr);
			}
			mRenderSystem->Render(*rp);
			mAllAttachedGameObject.pop();
		}
		mLightList.clear();
	}

	void SceneManager::Clear()
	{
		RemoveAll();
	}
}
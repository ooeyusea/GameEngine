#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "../util/Macros.h"

namespace lly {

	Scene::Scene()
		: _root(nullptr),
		  _curCameraIndex(0)
	{
		_root = new GameObject();
	}

	Scene::~Scene()
	{
		SAFE_DELETE(_root);

		for (auto& camera : _cameras)
		{
			delete camera;
		}
		_cameras.clear();
	}

	void Scene::onEnter()
	{
	}

	void Scene::onExit()
	{
	}

	void Scene::run()
	{
		this->updateBefore();

		this->update();

		this->updateAfter();
	}

	void Scene::updateBefore()
	{
	}

	void Scene::update()
	{
	}

	void Scene::updateAfter()
	{
	}

	void Scene::addChild(GameObject* child)
	{
		ASSERT(child != nullptr);

		_root->addChild(child);
	}

	void Scene::removeChild(GameObject* child)
	{
		ASSERT(child != child);

		_root->removeChild(child);
	}

	void Scene::addCamera(Camera* camera)
	{
		ASSERT(camera != nullptr);

		_cameras.push_back(camera);
	}

	void Scene::setCurCamera(unsigned int curCameraIndex)
	{
		ASSERT(curCameraIndex < _cameras.size());

		_curCameraIndex = curCameraIndex;
	}

	Camera* Scene::getCurCamera()
	{
		return _cameras[_curCameraIndex];
	}
}

#include "SceneManager.h"
#include "Scene.h"
#include "../util/Macros.h"

namespace lly {

	SceneManager* SceneManager::getInstance()
	{
		static SceneManager s_sceneManager;
		return &s_sceneManager;
	}

	SceneManager::SceneManager()
		: _scene(nullptr)
	{
	}

	SceneManager::~SceneManager()
	{
		SAFE_DELETE(_scene);
	}

	void SceneManager::replaceScene(Scene* scene)
	{
		ASSERT(scene != nullptr);


		_scene = scene;
	}

	void SceneManager::run()
	{
		if (_scene != nullptr)
		{
		}
	}
}
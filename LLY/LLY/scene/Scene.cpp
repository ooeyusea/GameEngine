#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "../util/Macros.h"
#include "../System.h"
#include "Light.h"
#include <algorithm>
#include "../render/RenderTarget.h"
#include "../resource/Mesh.h"
#include "../resource/MeshPart.h"

namespace lly {

	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{
	}

	bool Scene::init()
	{
		return true;
	}

	void Scene::release()
	{

	}

	void Scene::update(float elapse)
	{
		for (auto& root : _roots)
		{
			root->update(elapse);
			root->last_update();
		}
	}

	void Scene::render()
	{
		std::sort(_cameras.begin(), _cameras.end(), Camera::order);
		for (auto& camera : _cameras)
		{
			__render(camera);
		}

		for (auto& light : _lights)
			__render(light);
	}

	void Scene::add_node(Camera * camera)
	{
		auto itr = std::find(_cameras.begin(), _cameras.end(), camera);
		if (itr == _cameras.end())
			_cameras.push_back(camera);
	}

	void Scene::__render(Camera * camera)
	{
		camera->begin_render();
		for (auto& root : _roots)
		{
			root->visit([](Node * node){
				node->draw();
			});
		}
	}

	void Scene::__render(Light * light)
	{
		light->draw_light();
	}
}

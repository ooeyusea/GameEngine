#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include "render/Device.h"
#include "render/RenderCommandGroup.h"
#include <algorithm>
#include "scene/SceneManager.h"
#include "render/RenderTarget.h"
#include "resource/material/MaterialLoader.h"
#include "resource/material/Material.h"
#include "resource/MeshPart.h"
#include "resource/VertexDescription.h"
#include "resource/VertexBuffer.h"
#include "resource/IndexBuffer.h"
#include "resource/Mesh.h"
#include "scene/Entity.h"
#include "util/FuncUnitl.h"
#include <chrono>

namespace lly {
	System::System()
		: _device(nullptr)
	{
	}


	System::~System()
	{
	}

	System& System::init(const std::string& path)
	{
		if (!_device->init())
		{
			throw std::runtime_error("init device failed");
		}

		if (!_device->createWindow("Test", 640, 480))
		{
			throw std::runtime_error("create window failed");
		}

		_device->reg(*this);

		return *this;
	}

	System& System::set_device(Device * device)
	{ 
		_device = device;
		return *this;
	}

	void System::run()
	{
		auto material = _resource_manager.load_material("test.material");
		auto material_ptr = _resource_manager.get_material(material);
		material_ptr->set_current_tech("default");

		auto mesh = _resource_manager.load_mesh("zhaoyun_.fbx");
		auto mesh_ptr = _resource_manager.get_mesh(mesh);

		set_model_matrix(glm::mat4(1.0f));
		set_view_matrix(glm::lookAt(glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		set_projection_matrix(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f));

		float rotation_x = 0.f * M_PI / 180.0f; //0.0f;
		float rotation_y = -45.f * M_PI / 180.0f;
		float rotation_z = 0.f * M_PI / 180.0f;

		Entity entity;
		entity.add_mesh(mesh_ptr);
		entity.set_scale(0.1f, 0.1f, 0.1f);
		entity.set_position_y(-50.f);
		entity.play_animation("Take 001", true);

		auto last = std::chrono::system_clock::now();

		while (!_device->shouldClose())
		{
			auto now = std::chrono::system_clock::now();

			auto a = std::chrono::duration_cast<std::chrono::microseconds>(last.time_since_epoch()).count();
			auto b = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
			float elapse = (float)(b - a) / 1000000.0f;

			last = now;

			SceneManager::getInstance()->run();
			add_render_command_group(nullptr, ClearTarget::COLOR | ClearTarget::DEPTH);

			//rotation_x += 0.001f;
			//rotation_y += 0.002f;
			//rotation_z += 0.003f;

			entity.set_rotation(rotation_x, rotation_y, rotation_z);
			entity.update(elapse);

			entity.draw();

			draw();
			_device->run();
		}
	}

	void System::exit()
	{
		_device->exit();
	}

	void System::add_render_command_group(RenderTarget* target, int clear)
	{
		RenderCommandGroup * group = new RenderCommandGroup();
		if (group)
		{
			group->set_target(target);
			group->set_clear(clear);

			_commandGroups.push_back(group);
		}
	}

	void System::add_render_command(RenderCommand* command)
	{
		if (!_commandGroups.empty())
		{
			(*_commandGroups.rbegin())->add(command);
		}
		else
		{
			throw std::runtime_error("add command but no command group");
		}
	}

	void System::set_model_matrix(const glm::mat4& m)
	{
		_model = m;
		_model_view_projection = _projection * _view * _model;
	}

	void System::set_view_matrix(const glm::mat4& m)
	{
		_view = m;
		_model_view_projection = _projection * _view * _model;
	}

	void System::set_projection_matrix(const glm::mat4& m)
	{
		_projection = m;
		_model_view_projection = _projection * _view * _model;
	}

	void System::draw()
	{
		for (auto& group : _commandGroups)
		{
			group->sort();
		}
		std::sort(_commandGroups.begin(), _commandGroups.end(), [](RenderCommandGroup* a, RenderCommandGroup * b){
			return *a < *b;
		});

		for (auto& group : _commandGroups)
		{
			group->draw(*_device);
			delete group;
		}

		_commandGroups.clear();
	}
}

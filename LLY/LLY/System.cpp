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
#include "resource/RenderBuffer.h"
#include "render/LightingCommand.h"
#include "resource/Program.h"
#include "scene/Scene.h"
#include "resource/loader/SceneLoader.h"

namespace lly {
	System::System()
		: _device(nullptr)
		, _width(0)
		, _height(0)
		, _lighting(nullptr)
		, _texture1(nullptr)
		, _texture2(nullptr)
		, _texture3(nullptr)
		, _texture4(nullptr)
		, _index(nullptr)
		, _gbuffer(nullptr)
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

		_width = 640;
		_height = 480;
		if (!_device->createWindow("Test", _width, _height))
		{
			throw std::runtime_error("create window failed");
		}

		_device->reg(*this);

		float vertices[4 * 3] = {
			-1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
		};
		int indices[6] = {
			0, 1, 2, 0, 2, 3
		};

		VertexBuffer * vb = new VertexBuffer;
		vb->load_data((const char*)vertices, 12 * sizeof(float), lly::VertexUsage::STATIC_DRAW);
		lly::VertexDescription desc(lly::DrawType::TRIANGLES, 6);
		desc.add(lly::VertexAttribPosition::POSITION, 3, lly::DataType::FLOAT, false, 0, 0);
		_index = new IndexBuffer;
		_index->load_indices((const char*)indices, 6 * sizeof(int));
		_index->gen(vb, desc);

		auto lighting = _resource_manager.load_program("lighting.vsh", "lighting.fsh");
		_lighting = _resource_manager.get_program(lighting);

		_gbuffer = new RenderTarget;

		auto texture1 = _resource_manager.create_texture_2d("##GBUFFER_OUT0##", _width, _height, false, ColorFormat::RGBA8888, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::NEAREST, TextureMagFilter::NEAREST);
		_texture1 = _resource_manager.get_texture_2d(texture1);
		_gbuffer->add_render_texture(_texture1);

		auto texture2 = _resource_manager.create_texture_2d("##GBUFFER_OUT1##", _width, _height, false, ColorFormat::RGBA8888, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::NEAREST, TextureMagFilter::NEAREST);
		_texture2 = _resource_manager.get_texture_2d(texture2);
		_gbuffer->add_render_texture(_texture2);

		auto texture3 = _resource_manager.create_texture_2d("##GBUFFER_OUT2##", _width, _height, false, ColorFormat::RGBA8888, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::NEAREST, TextureMagFilter::NEAREST);
		_texture3 = _resource_manager.get_texture_2d(texture3);
		_gbuffer->add_render_texture(_texture3);

		auto texture4 = _resource_manager.create_texture_2d("##GBUFFER_OUT3##", _width, _height, false, ColorFormat::R32, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::NEAREST, TextureMagFilter::NEAREST);
		_texture4 = _resource_manager.get_texture_2d(texture4);
		_gbuffer->add_render_texture(_texture4);

		RenderBuffer * render_buffer = new RenderBuffer();
		render_buffer->create(_width, _height, RenderBuffer::Usage::DEPTH);
		_gbuffer->set_render_buffer(render_buffer);

		if (!_gbuffer->create())
		{
			throw std::runtime_error("create gbuffer failed");
		}

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

		loader::SceneLoader loader;
		Scene * scene = loader.load_from("test.scene");

		auto last = std::chrono::system_clock::now();
		while (!_device->shouldClose())
		{
			auto now = std::chrono::system_clock::now();

			auto a = std::chrono::duration_cast<std::chrono::microseconds>(last.time_since_epoch()).count();
			auto b = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
			float elapse = (float)(b - a) / 1000000.0f;

			last = now;

			scene->update(elapse);
			scene->render();

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
			group->set_target(target == nullptr ? _gbuffer : target);
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

	void System::add_lighting_command(LightingCommand* command)
	{
		_lighting_commands.push_back(command);
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

		_device->begin(nullptr, ClearTarget::COLOR | ClearTarget::DEPTH);
		_device->use_program(_lighting);
		_device->setUniformWithTex("g_buffer1", _texture1, false, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::NEAREST, TextureMagFilter::NEAREST);
		_device->setUniformWithTex("g_buffer2", _texture2, false, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::NEAREST, TextureMagFilter::NEAREST);
		_device->setUniformWithTex("g_buffer3", _texture3, false, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::NEAREST, TextureMagFilter::NEAREST);
		_device->setUniformWithTex("g_buffer4", _texture4, false, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::NEAREST, TextureMagFilter::NEAREST);
		auto inverse_viewproject_matrix = glm::inverse(_projection * _view);
		_device->setUniformWithMatrix4fv("inverse_viewproject", &inverse_viewproject_matrix[0][0], 1);
		_device->setUniformWith3f("u_eye", _eye.x, _eye.y, _eye.z);

		_device->set_blend(true, { BlendFactor::ONE, BlendFactor::ONE });
		for (auto& command : _lighting_commands)
		{
			command->draw(*_device);
			_index->draw(*_device);
		}
		_device->end(nullptr);

		for (auto& command : _lighting_commands)
			delete command;
		_lighting_commands.clear();
	}
}

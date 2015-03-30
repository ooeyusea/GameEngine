#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <string>
#include <vector>
#include "util/StringTable.h"
#include "resource/ResourceManager.h"
#include <glm/glm.hpp>

namespace lly {

	class Device;
	class RenderCommandGroup;
	class RenderCommand;
	class RenderTarget;

	class System
	{
	public:
		System();
		~System();

		System& init(const std::string& path);
		System& set_device(Device * device);
		void run();
		void exit();

		void add_render_command_group(RenderTarget*, int);
		void add_render_command(RenderCommand*);

		void set_model_matrix(const glm::mat4& m);
		void set_view_matrix(const glm::mat4& m);
		void set_projection_matrix(const glm::mat4& m);

		const glm::mat4& get_model_matrix() const { return _model; }
		const glm::mat4& get_view_matrix() const { return _view; }
		const glm::mat4& get_projection_matrix() const { return _projection; }
		const glm::mat4& get_mvp_matrix() const { return _model_view_projection; }

		void set_matrix_palette(std::vector<glm::mat4>&& matrix_palette) { std::swap(_matrix_palette, matrix_palette); }
		const std::vector<glm::mat4>& get_matrix_palette() const { return _matrix_palette; }

		lly_util::StringTable& get_string_table() { return _string_table; }
		ResourceManager& get_resource_manager() { return _resource_manager; }

		static System& instance()
		{
			static System g_instance;
			return g_instance;
		}

	private:
		void draw();

	private:
		Device * _device;
		std::vector<RenderCommandGroup*> _commandGroups;
		lly_util::StringTable _string_table;
		ResourceManager _resource_manager;

		glm::mat4 _model;
		glm::mat4 _view;
		glm::mat4 _projection;
		glm::mat4 _model_view_projection;
		std::vector<glm::mat4> _matrix_palette;
	};

}

#endif //SYSTEM_H_

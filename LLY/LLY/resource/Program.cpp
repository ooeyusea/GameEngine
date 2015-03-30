#include "Program.h"
#include <iostream>
#include "Texture2D.h"
#include "Texture3D.h"
#include "TextureCube.h"
#include "../util/FileUtil.h"
#include "../render/Device.h"
#include "../util/Macros.h"

namespace lly {

	bool compileShader(GLuint& id, GLenum type, const GLchar* source)
	{
		GLint status;

		if (!source)
		{
			return false;
		}

		id = glCreateShader(type);

		const GLchar *sources[] = {
			source,
		};
		glShaderSource(id, sizeof(sources) / sizeof(*sources), sources, nullptr);

		glCompileShader(id);

		glGetShaderiv(id, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			GLsizei length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			GLchar* src = (GLchar *)malloc(sizeof(GLchar)* length);

			glGetShaderInfoLog(id, length, &length, src);

			std::cerr << src << std::endl;

			free(src);

			id = 0;
			return false;
		}
		return (status == GL_TRUE);
	}

	Program::Program()
		: _program(0)
		, _vetex(0)
		, _fragmet(0)
		, _geometry(0)
		, _tessellation_control(0)
		, _tessellation_evaluation(0)
	{
	}

	Program::~Program()
	{
		if (_vetex)
		{
			glDeleteShader(_vetex);
			_vetex = 0;
		}

		if (_fragmet)
		{
			glDeleteShader(_fragmet);
			_fragmet = 0;
		}

		if (_geometry)
		{
			glDeleteShader(_geometry);
			_geometry = 0;
		}

		if (_tessellation_control)
		{
			glDeleteShader(_tessellation_control);
			_tessellation_control = 0;
		}

		if (_tessellation_evaluation)
		{
			glDeleteShader(_tessellation_evaluation);
			_tessellation_evaluation = 0;
		}

		if (_program)
		{
			glDeleteProgram(_program);
		}
	}

	void Program::use()
	{
		if (_program)
		{
			glUseProgram(_program);
		}
	}

	bool Program::add_vetex_shader(const char* source)
	{ 
		return compileShader(_vetex, GL_VERTEX_SHADER, source);
	}

	bool Program::add_fragment_shader(const char* source)
	{ 
		return compileShader(_fragmet, GL_FRAGMENT_SHADER, source);
	}

	bool Program::add_geometry_shader(const char* source)
	{ 
		return compileShader(_geometry, GL_GEOMETRY_SHADER, source);
	}

	bool Program::add_tessellation_control_shader(const char* source)
	{ 
		return compileShader(_tessellation_control, GL_TESS_CONTROL_SHADER, source);
	}

	bool Program::add_tessellation_evaluation_shader(const char* source)
	{ 
		return compileShader(_tessellation_evaluation, GL_TESS_EVALUATION_SHADER, source);
	}

	bool Program::link()
	{
		_program = glCreateProgram();

		if (_vetex)
		{
			glAttachShader(_program, _vetex);
		}

		if (_fragmet)
		{
			glAttachShader(_program, _fragmet);
		}

		if (_geometry)
		{
			glAttachShader(_program, _geometry);
		}

		if (_tessellation_control)
		{
			glAttachShader(_program, _tessellation_control);
		}

		if (_tessellation_evaluation)
		{
			glAttachShader(_program, _tessellation_evaluation);
		}

		glLinkProgram(_program);

		if (_vetex)
		{
			glDeleteShader(_vetex);
			_vetex = 0;
		}

		if (_fragmet)
		{
			glDeleteShader(_fragmet);
			_fragmet = 0;
		}

		if (_geometry)
		{
			glDeleteShader(_geometry);
			_geometry = 0;
		}

		if (_tessellation_control)
		{
			glDeleteShader(_tessellation_control);
			_tessellation_control = 0;
		}

		if (_tessellation_evaluation)
		{
			glDeleteShader(_tessellation_evaluation);
			_tessellation_evaluation = 0;
		}

		parse_uniforms();

		return true;
	}

	void Program::parse_uniforms()
	{
		GLint uniform_count = 0;
		glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &uniform_count);
		if (uniform_count > 0)
		{
			GLint length;
			glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
			if (length > 0)
			{
				GLchar* uniformName = (GLchar*)alloca(length + 1);
				int bind = 0;
				for (int i = 0; i < uniform_count; ++i)
				{
					ProgramUniform uniform;
					
					glGetActiveUniform(_program, i, length, nullptr, &uniform.size, &uniform.type, uniformName);
					uniformName[length] = '\0';

					char* c = strrchr(uniformName, '[');
					if (c)
					{
						*c = '\0';
					}

					uniform.name = std::string(uniformName);
					uniform.location = glGetUniformLocation(_program, uniformName);

					if (uniform.type == GL_SAMPLER_2D || uniform.type == GL_SAMPLER_3D || uniform.type == GL_SAMPLER_CUBE)
						uniform.bind = bind++;

					_uniforms[uniform.name] = uniform;
				}
			}
		}
	}

	GLint Program::get_location(const std::string& name, UniformType type)
	{
		auto itr = _uniforms.find(name);
		if (itr != _uniforms.end())
		{
			return itr->second.location;
		}

		return -1;
	}

	std::tuple<GLint, GLint> Program::get_texture_loaction(const std::string& name, UniformType type)
	{
		auto itr = _uniforms.find(name);
		if (itr != _uniforms.end())
		{
			if ((itr->second.type == GL_SAMPLER_2D && type == UniformType::TEXTURE2D) || 
				(itr->second.type == GL_SAMPLER_3D && type == UniformType::TEXTURE3D) || 
				(itr->second.type == GL_SAMPLER_CUBE && type == UniformType::TEXTURECUBE))
			return std::tuple<GLint, GLint>(itr->second.location, itr->second.bind);
		}

		return std::tuple<GLint, GLint>(-1, -1);
	}
}

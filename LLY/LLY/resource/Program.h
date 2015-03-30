#ifndef PROGRAM_H_
#define PROGRAM_H_
#include "../util/LLYIncludes.h"
#include <string>
#include <unordered_map>
#include <tuple>
#include "Resource.h"

namespace lly {
	class Texture2D;
	class Texture3D;
	class TextureCube;
	class Device;

	class Program : public Resource
	{
		struct ProgramUniform
		{
			std::string name;
			GLint size;
			GLenum type;
			GLint location;
			int bind;
		};

	public:

		enum class UniformType
		{
			I1,
			I2,
			I3,
			I4,
			IV1,
			IV2,
			IV3,
			IV4,
			F1,
			F2,
			F3,
			F4,
			FV1,
			FV2,
			FV3,
			FV4,
			MATRIX2,
			MATRIX3,
			MATRIX4,
			TEXTURE2D,
			TEXTURE3D,
			TEXTURECUBE,
		};

		Program();
		~Program();

		void use();

		bool add_vetex_shader(const char* source);
		bool add_fragment_shader(const char* source);
		bool add_geometry_shader(const char* source);
		bool add_tessellation_control_shader(const char* source);
		bool add_tessellation_evaluation_shader(const char* source);
		bool link();

		GLint get_location(const std::string& name, UniformType type);
		std::tuple<GLint, GLint> get_texture_loaction(const std::string& name, UniformType type);

	private:
		void parse_uniforms();

	private:
		GLuint _program;
		GLuint _vetex;
		GLuint _fragmet;
		GLuint _geometry;
		GLuint _tessellation_control;
		GLuint _tessellation_evaluation;

		std::unordered_map<std::string, ProgramUniform> _uniforms;
	};
}

#endif //PROGRAM_H_



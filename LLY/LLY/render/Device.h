#ifndef LLYDEVICE_H_
#define LLYDEVICE_H_
#include "../util/LLYIncludes.h"
#include <unordered_map>

namespace lly {
	class System;
	class Input;
	class RenderTarget;
	class Program;
	class Texture2D;
	class Texture3D;
	class TextureCube;

	class Device
	{
		struct UniformCache
		{
			void * ptr;
			int size;

			UniformCache() : ptr(nullptr), size(0) {}

			bool update(void * p, int s);
			void clear();
		};

		struct Texture2DCache
		{
			int bind;
			GLuint tex;
			bool mipmap;
			TextureWrap s;
			TextureWrap t;
			TextureMinFilter min;
			TextureMagFilter mag;
		};

		struct Texture3DCache
		{
			int bind;
			GLuint tex;
			bool mipmap;
			TextureWrap s;
			TextureWrap t;
			TextureWrap r;
			TextureMinFilter min;
			TextureMagFilter mag;
		};

	public:
		Device();
		~Device();

		bool init();
		void release();

		bool createWindow(const std::string& name, int width, int height);

		void reg(Input&);
		bool shouldClose();
		void run();
		void exit();

		void begin(RenderTarget * target, int clear);
		void flush();
		void end(RenderTarget * target);

		void use_program(Program * program);
		void set_blend(bool enable, BlendFunc func);
		void set_alpha_test(bool enable, AlphaTestFunc func, float factor);
		void set_depth_test(bool enable, bool write, DepthFunc func);
		void set_cull_face(bool enable, CullFaceSide side, FrontFace front);
		void set_stencil_test(bool enable, unsigned int write_mask, StencilFunc func, StencilOp op);

		void clear_cache();

		void setUniformWith1i(const std::string& name, GLint i1);
		void setUniformWith2i(const std::string& name, GLint i1, GLint i2);
		void setUniformWith3i(const std::string& name, GLint i1, GLint i2, GLint i3);
		void setUniformWith4i(const std::string& name, GLint i1, GLint i2, GLint i3, GLint i4);
		void setUniformWith1iv(const std::string& name, GLint* ints, unsigned int numberOfArrays);
		void setUniformWith2iv(const std::string& name, GLint* ints, unsigned int numberOfArrays);
		void setUniformWith3iv(const std::string& name, GLint* ints, unsigned int numberOfArrays);
		void setUniformWith4iv(const std::string& name, GLint* ints, unsigned int numberOfArrays);
		void setUniformWith1f(const std::string& name, GLfloat f1);
		void setUniformWith2f(const std::string& name, GLfloat f1, GLfloat f2);
		void setUniformWith3f(const std::string& name, GLfloat f1, GLfloat f2, GLfloat f3);
		void setUniformWith4f(const std::string& name, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);
		void setUniformWith1fv(const std::string& name, GLfloat* floats, unsigned int numberOfArrays);
		void setUniformWith2fv(const std::string& name, GLfloat* floats, unsigned int numberOfArrays);
		void setUniformWith3fv(const std::string& name, GLfloat* floats, unsigned int numberOfArrays);
		void setUniformWith4fv(const std::string& name, GLfloat* floats, unsigned int numberOfArrays);
		void setUniformWithMatrix2fv(const std::string& name, GLfloat* matrixArray, unsigned int numberOfMatrices);
		void setUniformWithMatrix3fv(const std::string& name, GLfloat* matrixArray, unsigned int numberOfMatrices);
		void setUniformWithMatrix4fv(const std::string& name, GLfloat* matrixArray, unsigned int numberOfMatrices);
		void setUniformWithTex(const std::string& name, Texture2D* texture, bool mipmap, TextureWrap s, TextureWrap t, TextureMinFilter min, TextureMagFilter mag);
		void setUniformWithTex(const std::string& name, Texture3D* texture, bool mipmap, TextureWrap s, TextureWrap t, TextureWrap r, TextureMinFilter min, TextureMagFilter mag);
		void setUniformWithTex(const std::string& name, TextureCube* texture);

	private:
		GLFWwindow* _window;

		Program* _current_program;

		bool _enable_blend;
		BlendFunc _blend_func;

		bool _enable_alpha_test;
		AlphaTestFunc _alpha_test_func;
		float _alpha_test_factor;

		bool _enable_depth_test;
		bool _enable_depth_write;
		DepthFunc _depth_func;

		bool _enable_cull_face;
		CullFaceSide _cull_face_side;
		FrontFace _front_face;

		bool _enable_stencil_test;
		unsigned int _stencil_write_mask;
		StencilFunc _stencil_func;
		StencilOp _stencil_op;

		std::unordered_map<std::string, UniformCache> _caches;
	};
}

#endif //LLYDEVICE_H_

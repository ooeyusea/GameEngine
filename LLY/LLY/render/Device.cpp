#include "Device.h"
#include "../System.h"
#include <iostream>
#include "../util/FuncUnitl.h"
#include "../resource/Program.h"
#include "RenderTarget.h"
#include "../resource/Texture2D.h"
#include "../resource/Texture3D.h"
#include "../resource/TextureCube.h"
#include "../util/Macros.h"

namespace lly {

	bool glew_dynamic_binding()
	{
		const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

		// If the current opengl driver doesn't have framebuffers methods, check if an extension exists
		if (glGenFramebuffers == nullptr)
		{
			if (strstr(gl_extensions, "ARB_framebuffer_object"))
			{
				glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");
				glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
				glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
				glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
				glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
				glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
				glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebuffer");
				glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
				glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
				glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
				glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
				glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)wglGetProcAddress("glFramebufferTexture1D");
				glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
				glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3D");
				glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
				glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
				glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
			}
			else
			if (strstr(gl_extensions, "EXT_framebuffer_object"))
			{
				glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbufferEXT");
				glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbufferEXT");
				glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
				glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffersEXT");
				glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorageEXT");
				glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
				glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebufferEXT");
				glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebufferEXT");
				glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
				glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffersEXT");
				glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
				glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
				glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
				glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
				glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
				glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
				glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmapEXT");
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	bool Device::UniformCache::update(void * p, int s)
	{
		if (ptr == nullptr)
		{
			ptr = malloc(s);
			size = s;
			memcpy(ptr, p, s);
		}
		else if (size != s)
		{
			ptr = realloc(ptr, s);
			size = s;
			memcpy(ptr, p, s);
		}
		else if (memcmp(ptr, p, s) != 0)
		{
			memcpy(ptr, p, s);
		}
		else
			return false;

		return true;
	}

	void Device::UniformCache::clear()
	{
		LLY_SAFE_DELETE(ptr);
	}

	Device::Device()
		: _window(nullptr)
		, _current_program(nullptr)
		, _enable_blend(false)
		, _blend_func({ BlendFactor::ONE, BlendFactor::ZERO })
		, _enable_alpha_test(false)
		, _alpha_test_func(AlphaTestFunc::ALWAYS)
		, _alpha_test_factor(0.0f)
		, _enable_depth_test(true)
		, _enable_depth_write(true)
		, _depth_func(DepthFunc::LEQUAL)
		, _enable_cull_face(false)
		, _cull_face_side(CullFaceSide::BACK)
		, _front_face(FrontFace::CCW)
		, _enable_stencil_test(false)
		, _stencil_write_mask(0xff)
		, _stencil_func({ StencilFuncType::ALWAYS, 0, 0xff })
		, _stencil_op({ StencilOpType::KEEP, StencilOpType::KEEP, StencilOpType::KEEP })
	{
		glfwSetErrorCallback([](int error, const char* description){
			std::cerr << description << std::endl;
		});

		glfwInit();
	}


	Device::~Device()
	{
		clear_cache();

		glfwTerminate();
	}

	bool Device::init()
	{
		return true;
	}

	void Device::release()
	{

	}

	bool Device::createWindow(const std::string& name, int width, int height)
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);

		_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if (!_window)
			return false;
		glfwMakeContextCurrent(_window);

		GLenum GlewInitResult = glewInit();
		if (GLEW_OK != GlewInitResult)
			return false;

		if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader)
		{
			return false;
		}

		if (!glewIsSupported("GL_VERSION_4_0"))
		{
			return false;
		}

		if (!glew_dynamic_binding())
		{
			return false;
		}

		glClearDepth(1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		return true;
	}


	void Device::reg(System& system)
	{
		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int modify){
			//system.getInput.dispatch(llye::toMouseButtonEvent(button, action, modify));
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y){
			//system.getInput.dispatch(llye::toMouseMoveEvent(x, y));
		});
		glfwSetScrollCallback(_window, [](GLFWwindow* window, double x, double y){
			//system.getInput.dispatch(llye::toMouseScrollEvent(x, y));
		});
		glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int character){
			
		});
		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			//system.getInput.dispatch(llye::toKeyEvent(key, scancode, action, mods));
		});
		glfwSetWindowPosCallback(_window, [](GLFWwindow* windows, int x, int y){

		});
		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int w, int h){

		});
		glfwSetWindowSizeCallback(_window, [](GLFWwindow *window, int width, int height){
		
		});
		glfwSetWindowIconifyCallback(_window, [](GLFWwindow* window, int iconified){
		
		});
	}

	bool Device::shouldClose()
	{
		return (glfwWindowShouldClose(_window) != 0);
	}

	void Device::run()
	{
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	void Device::exit()
	{
		glfwSetWindowShouldClose(_window, GL_TRUE);
	}

	void Device::begin(RenderTarget * target, int clear)
	{
		if (target)
		{
			target->load();
		}

		GLbitfield flag = 0;
		if ((clear | ClearTarget::COLOR) > 0)
			flag |= GL_COLOR_BUFFER_BIT;
		if ((clear | ClearTarget::DEPTH) > 0)
			flag |= GL_DEPTH_BUFFER_BIT;
		if ((clear | ClearTarget::ACCUM) > 0)
			flag |= GL_ACCUM_BUFFER_BIT;
		if ((clear | ClearTarget::STENCIL) > 0)
			flag |= GL_STENCIL_BUFFER_BIT;

		if (flag > 0)
			glClear(flag);
	}

	void Device::flush()
	{
		glFlush();
	}
	
	void Device::end(RenderTarget * target)
	{
		if (target)
		{
			target->unload();
		}
		else
			glFinish();
	}

	void Device::use_program(Program * program)
	{
		if (_current_program != program)
		{
			_current_program = program;
			clear_cache();
			_current_program->use();
		}
	}

	void Device::set_blend(bool enable, BlendFunc func)
	{
		if (_enable_blend != enable)
		{
			_enable_blend = enable;

			if (_enable_blend)
			{
				glEnable(GL_BLEND);

				_blend_func = func;
				glBlendFunc(lly_util::to_blend_factor(func.src), lly_util::to_blend_factor(func.dst));
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}
		else if (_enable_blend)
		{
			if (_blend_func != func)
			{
				_blend_func = func;
				glBlendFunc(lly_util::to_blend_factor(func.src), lly_util::to_blend_factor(func.dst));
			}
		}
	}

	void Device::set_alpha_test(bool enable, AlphaTestFunc func, float factor)
	{
		if (_enable_alpha_test != enable)
		{
			_enable_alpha_test = enable;
			if (_enable_alpha_test)
			{
				glEnable(GL_ALPHA_TEST);

				_alpha_test_func = func;
				_alpha_test_factor = factor;
				glAlphaFunc(lly_util::to_alpha_test_func(func), factor);
			}
			else
				glDisable(GL_ALPHA_TEST);
		}
		else if (_enable_alpha_test)
		{
			if (_alpha_test_func != func || lly_util::eq(_alpha_test_factor, factor))
			{
				_alpha_test_func = func;
				_alpha_test_factor = factor;
				glAlphaFunc(lly_util::to_alpha_test_func(func), factor);
			}
		}
	}

	void Device::set_depth_test(bool enable, bool write, DepthFunc func)
	{
		if (_enable_depth_test != enable)
		{
			_enable_depth_test = enable;
			if (_enable_depth_test)
			{
				glEnable(GL_DEPTH_TEST);

				_enable_depth_write = write;
				if (_enable_depth_write)
					glDepthMask(GL_TRUE);
				else
					glDepthMask(GL_FALSE);

				_depth_func = func;
				glDepthFunc(lly_util::to_depth_func(func));
			}
			else
				glDisable(GL_DEPTH_TEST);
		}
		else if (_enable_depth_test)
		{
			if (_enable_depth_write != write)
			{
				_enable_depth_write = write;
				if (_enable_depth_write)
					glDepthMask(GL_TRUE);
				else
					glDepthMask(GL_FALSE);
			}

			if (_depth_func != func)
			{
				_depth_func = func;
				glDepthFunc(lly_util::to_depth_func(func));
			}
		}
	}

	void Device::set_cull_face(bool enable, CullFaceSide side, FrontFace front)
	{
		if (_enable_cull_face != enable)
		{
			_enable_cull_face = enable;
			if (_enable_cull_face)
			{
				glEnable(GL_CULL_FACE);

				_cull_face_side = side;
				glCullFace(lly_util::to_cull_fase_side(side));

				_front_face = front;
				glFrontFace(lly_util::to_front_face(front));
			}
			else
				glDisable(GL_CULL_FACE);
		}
		else if (_enable_cull_face)
		{
			if (_cull_face_side != side)
			{
				_cull_face_side = side;
				glCullFace(lly_util::to_cull_fase_side(side));
			}

			if (_front_face != front)
			{
				_front_face = front;
				glFrontFace(lly_util::to_front_face(front));
			}
		}
	}

	void Device::set_stencil_test(bool enable, unsigned int write_mask, StencilFunc func, StencilOp op)
	{
		if (_enable_stencil_test != enable)
		{
			_enable_stencil_test = enable;
			if (_enable_stencil_test)
			{
				glEnable(GL_STENCIL_TEST);
			}
			else
				glDisable(GL_STENCIL_TEST);
		}
		else if (_enable_stencil_test)
		{
			if (_stencil_write_mask != write_mask)
			{
				_stencil_write_mask = write_mask;
				glStencilMask((GLuint)write_mask);
			}

			if (_stencil_func != func)
			{
				_stencil_func = func;
				glStencilFunc(lly_util::to_stencil_func(func.func), (GLint)func.ref, (GLuint)func.mask);
			}

			if (_stencil_op != op)
			{
				_stencil_op = op;
				glStencilOp(lly_util::to_stencil_op(op.s_fail), lly_util::to_stencil_op(op.d_fail), lly_util::to_stencil_op(op.d_pass));
			}
		}
	}

	void Device::clear_cache()
	{
		for (auto& itr : _caches)
		{
			itr.second.clear();
		}
		_caches.clear();
	}


	void Device::setUniformWith1i(const std::string& name, GLint i1)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::I1);
		if (location == -1)
			return;

		if (_caches[name].update(&i1, sizeof(GLint)))
		{
			glUniform1i(location, i1);
		}
	}

	void Device::setUniformWith2i(const std::string& name, GLint i1, GLint i2)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::I2);
		if (location == -1)
			return;

		int tmp[] = { i1, i2 };
		if (_caches[name].update(&tmp, sizeof(tmp)))
		{
			glUniform2i(location, i1, i2);
		}
	}

	void Device::setUniformWith3i(const std::string& name, GLint i1, GLint i2, GLint i3)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::I3);
		if (location == -1)
			return;

		int tmp[] = { i1, i2, i3 };
		if (_caches[name].update(&tmp, sizeof(tmp)))
		{
			glUniform3i(location, i1, i2, i3);
		}
	}

	void Device::setUniformWith4i(const std::string& name, GLint i1, GLint i2, GLint i3, GLint i4)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::I4);
		if (location == -1)
			return;

		int tmp[] = { i1, i2, i3, i4 };
		if (_caches[name].update(&tmp, sizeof(tmp)))
		{
			glUniform4i(location, i1, i2, i3, i4);
		}
	}

	void Device::setUniformWith1iv(const std::string& name, GLint* ints, unsigned int numberOfArrays)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::IV1);
		if (location == -1)
			return;

		if (_caches[name].update(ints, sizeof(int)* numberOfArrays))
		{
			glUniform1iv(location, (GLsizei)numberOfArrays, ints);
		}
	}

	void Device::setUniformWith2iv(const std::string& name, GLint* ints, unsigned int numberOfArrays)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::IV2);
		if (location == -1)
			return;

		if (_caches[name].update(ints, sizeof(int)* 2 * numberOfArrays))
		{
			glUniform2iv(location, (GLsizei)numberOfArrays, ints);
		}
	}

	void Device::setUniformWith3iv(const std::string& name, GLint* ints, unsigned int numberOfArrays)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::IV3);
		if (location == -1)
			return;

		if (_caches[name].update(ints, sizeof(int)* 3 * numberOfArrays))
		{
			glUniform3iv(location, (GLsizei)numberOfArrays, ints);
		}
	}

	void Device::setUniformWith4iv(const std::string& name, GLint* ints, unsigned int numberOfArrays)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::IV4);
		if (location == -1)
			return;

		if (_caches[name].update(ints, sizeof(int)* 4 * numberOfArrays))
		{
			glUniform4iv(location, (GLsizei)numberOfArrays, ints);
		}
	}

	void Device::setUniformWith1f(const std::string& name, GLfloat f1)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::F1);
		if (location == -1)
			return;

		if (_caches[name].update(&f1, sizeof(f1)))
		{
			glUniform1f(location, f1);
		}
	}

	void Device::setUniformWith2f(const std::string& name, GLfloat f1, GLfloat f2)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::F2);
		if (location == -1)
			return;

		float tmp[] = { f1, f2 };
		if (_caches[name].update(&tmp, sizeof(tmp)))
		{
			glUniform2f(location, f1, f2);
		}
	}

	void Device::setUniformWith3f(const std::string& name, GLfloat f1, GLfloat f2, GLfloat f3)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::F3);
		if (location == -1)
			return;

		float tmp[] = { f1, f2, f3 };
		if (_caches[name].update(&tmp, sizeof(tmp)))
		{
			glUniform3f(location, f1, f2, f3);
		}
	}

	void Device::setUniformWith4f(const std::string& name, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::F4);
		if (location == -1)
			return;

		float tmp[] = { f1, f2, f3, f4 };
		if (_caches[name].update(&tmp, sizeof(tmp)))
		{
			glUniform4f(location, f1, f2, f3, f4);
		}
	}

	void Device::setUniformWith1fv(const std::string& name, GLfloat* floats, unsigned int numberOfArrays)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::FV1);
		if (location == -1)
			return;

		if (_caches[name].update(floats, sizeof(float)* numberOfArrays))
		{
			glUniform1fv(location, (GLsizei)numberOfArrays, floats);
		}
	}

	void Device::setUniformWith2fv(const std::string& name, GLfloat* floats, unsigned int numberOfArrays)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::FV2);
		if (location == -1)
			return;

		if (_caches[name].update(floats, sizeof(float)* 2 * numberOfArrays))
		{
			glUniform2fv(location, (GLsizei)numberOfArrays, floats);
		}
	}

	void Device::setUniformWith3fv(const std::string& name, GLfloat* floats, unsigned int numberOfArrays)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::FV3);
		if (location == -1)
			return;

		if (_caches[name].update(floats, sizeof(float)* 3 * numberOfArrays))
		{
			glUniform3fv(location, (GLsizei)numberOfArrays, floats);
		}
	}

	void Device::setUniformWith4fv(const std::string& name, GLfloat* floats, unsigned int numberOfArrays)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::FV4);
		if (location == -1)
			return;

		if (_caches[name].update(floats, sizeof(float)* 4 * numberOfArrays))
		{
			glUniform4fv(location, (GLsizei)numberOfArrays, floats);
		}
	}

	void Device::setUniformWithMatrix2fv(const std::string& name, GLfloat* matrixArray, unsigned int numberOfMatrices)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::MATRIX2);
		if (location == -1)
			return;

		if (_caches[name].update(matrixArray, sizeof(float)* 4 * numberOfMatrices))
		{
			glUniformMatrix2fv(location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
		}
	}

	void Device::setUniformWithMatrix3fv(const std::string& name, GLfloat* matrixArray, unsigned int numberOfMatrices)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::MATRIX3);
		if (location == -1)
			return;

		if (_caches[name].update(matrixArray, sizeof(float)* 9 * numberOfMatrices))
		{
			glUniformMatrix3fv(location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
		}
	}

	void Device::setUniformWithMatrix4fv(const std::string& name, GLfloat* matrixArray, unsigned int numberOfMatrices)
	{
		if (_current_program == nullptr)
			return;

		GLint location = _current_program->get_location(name, Program::UniformType::MATRIX4);
		if (location == -1)
			return;

		if (_caches[name].update(matrixArray, sizeof(float)* 16 * numberOfMatrices))
		{
			glUniformMatrix4fv(location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
		}
	}

	void Device::setUniformWithTex(const std::string& name, Texture2D* texture, bool mipmap, TextureWrap s, TextureWrap t, TextureMinFilter min, TextureMagFilter mag)
	{
		if (_current_program == nullptr)
			return;

		auto tup = _current_program->get_texture_loaction(name, Program::UniformType::TEXTURE2D);
		GLint location = std::get<0>(tup);
		if (location == -1)
			return;

		GLint bind = std::get<1>(tup);

		Texture2DCache tmp = { bind, texture->get_tex(), mipmap, s, t, min, mag };
		if (_caches[name].update(&tmp, sizeof(tmp)))
		{
			glUniform1i(location, bind);
			glActiveTexture(GL_TEXTURE0 + bind);
			glBindTexture(GL_TEXTURE_2D, texture->get_tex());

			texture->set_warp(s, t);
			texture->set_filter(min, mag);
		}
	}

	void Device::setUniformWithTex(const std::string& name, Texture3D* texture, bool mipmap, TextureWrap s, TextureWrap t, TextureWrap r, TextureMinFilter min, TextureMagFilter mag)
	{
		if (_current_program == nullptr)
			return;

		auto tup = _current_program->get_texture_loaction(name, Program::UniformType::TEXTURE3D);
		GLint location = std::get<0>(tup);
		if (location == -1)
			return;

		GLint bind = std::get<1>(tup);

		Texture3DCache tmp = { bind, texture->get_tex(), mipmap, s, t, r, min, mag };
		if (_caches[name].update(&tmp, sizeof(tmp)))
		{
			glUniform1i(location, bind);
			glActiveTexture(GL_TEXTURE0 + bind);
			glBindTexture(GL_TEXTURE_3D, texture->get_tex());

			//texture->set_warp(s, t, r);
			//texture->set_filter(min, mag);
		}
	}

	void Device::setUniformWithTex(const std::string& name, TextureCube* texture)
	{
		if (_current_program == nullptr)
			return;

		auto tup = _current_program->get_texture_loaction(name, Program::UniformType::TEXTURECUBE);
		GLint location = std::get<0>(tup);
		if (location == -1)
			return;

		GLint bind = std::get<1>(tup);
	}
}
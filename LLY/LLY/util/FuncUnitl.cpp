#include <glm/gtc/matrix_transform.hpp>
#include "FuncUnitl.h"
#include <fbxsdk.h>

namespace lly_util {

	GLenum to_usage(lly::VertexUsage usage)
	{
		switch (usage)
		{
		case lly::VertexUsage::STATIC_DRAW: return GL_STATIC_DRAW;
		case lly::VertexUsage::STATIC_COPY: return GL_STATIC_COPY;
		case lly::VertexUsage::STATIC_READ: return GL_STATIC_READ;
		case lly::VertexUsage::STREAM_DRAW: return GL_STREAM_DRAW;
		case lly::VertexUsage::STREAM_COPY: return GL_STREAM_COPY;
		case lly::VertexUsage::STREAM_READ: return GL_STREAM_READ;
		case lly::VertexUsage::DYNAMIC_DRAW: return GL_DYNAMIC_DRAW;
		case lly::VertexUsage::DYNAMIC_COPY: return GL_DYNAMIC_COPY;
		case lly::VertexUsage::DYNAMIC_READ: return GL_DYNAMIC_READ;
		}
		return 0;
	}

	GLenum to_draw_mode(lly::DrawType mode)
	{
		switch (mode)
		{
		case lly::DrawType::POINTS: return GL_POINTS;
		case lly::DrawType::LINE_STRIP: return GL_LINE_STRIP;
		case lly::DrawType::LINE_LOOP: return GL_LINE_LOOP;
		case lly::DrawType::LINES: return GL_LINES;
		case lly::DrawType::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		case lly::DrawType::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
		case lly::DrawType::TRIANGLES: return GL_TRIANGLES;
		case lly::DrawType::QUAD_STRIP: return GL_QUAD_STRIP;
		case lly::DrawType::QUADS: return GL_QUADS;
		case lly::DrawType::POLYGON: return GL_POLYGON;
		}
		return 0;
	}


	GLenum to_color_format(lly::ColorFormat format)
	{
		switch (format)
		{
		case lly::ColorFormat::BGRA8888: return GL_BGRA;
		case lly::ColorFormat::RGBA8888: return GL_RGBA;
		case lly::ColorFormat::RGB888: return GL_RGB;
		case lly::ColorFormat::RGB565: return GL_RGB565;
		case lly::ColorFormat::A8: return GL_ALPHA;
		case lly::ColorFormat::I8: return GL_LUMINANCE;
		case lly::ColorFormat::AI88: return GL_LUMINANCE_ALPHA;
		case lly::ColorFormat::RGBA4444: return GL_RGBA;
		case lly::ColorFormat::RGB5A1: return GL_RGBA;
		case lly::ColorFormat::RG16: return GL_RG16;
		case lly::ColorFormat::RG16F: return GL_RG16F;
		case lly::ColorFormat::R32F: return GL_R32F;
		case lly::ColorFormat::DEPTH_COMPONENT: return GL_DEPTH_COMPONENT;
		}
		return GL_RGBA;
	}

	int to_color_bpp(lly::ColorFormat format)
	{
		switch (format)
		{
		case lly::ColorFormat::BGRA8888: return 32;
		case lly::ColorFormat::RGBA8888: return 32;
		case lly::ColorFormat::RG16: return 32;
		case lly::ColorFormat::RG16F: return 32;
		case lly::ColorFormat::R32F: return 32;
		case lly::ColorFormat::RGB888: return 24;
		case lly::ColorFormat::RGB565: return 16;
		case lly::ColorFormat::A8: return 8;
		case lly::ColorFormat::I8: return 8;
		case lly::ColorFormat::AI88: return 16;
		case lly::ColorFormat::RGBA4444: return 16;
		case lly::ColorFormat::RGB5A1: return 16;
		case lly::ColorFormat::DEPTH_COMPONENT: return 32;
		}
		return 32;
	}

	GLenum to_color_type(lly::ColorFormat format)
	{
		switch (format)
		{
		case lly::ColorFormat::BGRA8888: return GL_UNSIGNED_BYTE;
		case lly::ColorFormat::RGBA8888: return GL_UNSIGNED_BYTE;
		case lly::ColorFormat::RG16: return GL_UNSIGNED_SHORT;
		case lly::ColorFormat::RG16F: return GL_HALF_FLOAT;
		case lly::ColorFormat::R32F: return GL_FLOAT;
		case lly::ColorFormat::RGB888: return GL_UNSIGNED_BYTE;
		case lly::ColorFormat::RGB565: return GL_UNSIGNED_SHORT_5_6_5;
		case lly::ColorFormat::A8: return GL_UNSIGNED_BYTE;
		case lly::ColorFormat::I8: return GL_UNSIGNED_BYTE;
		case lly::ColorFormat::AI88: return GL_UNSIGNED_BYTE;
		case lly::ColorFormat::RGBA4444: return GL_UNSIGNED_SHORT_4_4_4_4;
		case lly::ColorFormat::RGB5A1: return GL_UNSIGNED_SHORT_5_5_5_1;
		case lly::ColorFormat::DEPTH_COMPONENT: return GL_FLOAT;
		}
		return GL_UNSIGNED_BYTE;
	}

	GLenum to_min_filter(lly::TextureMinFilter filter)
	{
		return GL_LINEAR;
	}

	GLenum to_mag_filter(lly::TextureMagFilter filter)
	{
		return GL_LINEAR;
	}

	GLuint to_position(lly::VertexAttribPosition position)
	{
		switch (position)
		{
		case lly::VertexAttribPosition::POSITION: return 0;
		case lly::VertexAttribPosition::NORMAL: return 1;
		case lly::VertexAttribPosition::COLOR: return 2;
		case lly::VertexAttribPosition::TANGENT: return 3;
		case lly::VertexAttribPosition::TEXCOOD0: return 4;
		case lly::VertexAttribPosition::TEXCOOD1: return 5;
		case lly::VertexAttribPosition::TEXCOOD2: return 6;
		case lly::VertexAttribPosition::TEXCOOD3: return 7;
		case lly::VertexAttribPosition::TEXCOOD4: return 8;
		case lly::VertexAttribPosition::TEXCOOD5: return 9;
		case lly::VertexAttribPosition::TEXCOOD6: return 10;
		case lly::VertexAttribPosition::TEXCOOD7: return 11;
		case lly::VertexAttribPosition::WEIGHT0: return 12;
		case lly::VertexAttribPosition::WEIGHT1: return 13;
		case lly::VertexAttribPosition::WEIGHT2: return 14;
		case lly::VertexAttribPosition::WEIGHT3: return 15;
		}

		return 0;
	}

	GLenum to_data_type(lly::DataType type)
	{
		switch (type)
		{
		case lly::DataType::BYTE: return GL_BYTE;
		case lly::DataType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
		case lly::DataType::SHORT: return GL_SHORT;
		case lly::DataType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
		case lly::DataType::FIXED: return GL_FIXED;
		case lly::DataType::FLOAT: return GL_FLOAT;
		}

		return GL_FLOAT;
	}

	GLenum to_blend_factor(lly::BlendFactor factor)
	{
		switch (factor)
		{
		case lly::BlendFactor::DST_ALPHA: return GL_DST_ALPHA;
		case lly::BlendFactor::DST_COLOR: return GL_DST_COLOR;
		case lly::BlendFactor::ONE: return GL_ONE;
		case lly::BlendFactor::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		case lly::BlendFactor::ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
		case lly::BlendFactor::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case lly::BlendFactor::SRC_ALPHA: return GL_SRC_ALPHA;
		case lly::BlendFactor::SRC_ALPHA_SATURATE: return GL_SRC_ALPHA_SATURATE;
		case lly::BlendFactor::ZERO: return GL_ZERO;
		}
		return GL_ZERO;
	}

	GLenum to_alpha_test_func(lly::AlphaTestFunc func)
	{
		switch (func)
		{
		case lly::AlphaTestFunc::ALWAYS: return GL_ALWAYS;
		case lly::AlphaTestFunc::NEVER: return GL_NEVER;
		case lly::AlphaTestFunc::LESS: return GL_LESS;
		case lly::AlphaTestFunc::LEQUAL: return GL_LEQUAL;
		case lly::AlphaTestFunc::EQUAL: return GL_EQUAL;
		case lly::AlphaTestFunc::GREATER: return GL_GREATER;
		case lly::AlphaTestFunc::GEQUAL: return GL_GEQUAL;
		case lly::AlphaTestFunc::NOTEQUAL: return GL_NOTEQUAL;
		}
		return GL_ALWAYS;
	}

	GLenum to_depth_func(lly::DepthFunc func)
	{
		switch (func)
		{
		case lly::DepthFunc::ALWAYS: return GL_ALWAYS;
		case lly::DepthFunc::NEVER: return GL_NEVER;
		case lly::DepthFunc::LESS: return GL_LESS;
		case lly::DepthFunc::LEQUAL: return GL_LEQUAL;
		case lly::DepthFunc::EQUAL: return GL_EQUAL;
		case lly::DepthFunc::GREATER: return GL_GREATER;
		case lly::DepthFunc::GEQUAL: return GL_GEQUAL;
		case lly::DepthFunc::NOTEQUAL: return GL_NOTEQUAL;
		}
		return GL_ALWAYS;
	}

	GLenum to_cull_fase_side(lly::CullFaceSide side)
	{
		switch (side)
		{
		case lly::CullFaceSide::FRONT: return GL_FRONT;
		case lly::CullFaceSide::BACK: return GL_BACK;
		}
		return GL_BACK;
	}

	GLenum to_front_face(lly::FrontFace front)
	{
		switch (front)
		{
		case lly::FrontFace::CCW: return GL_CCW;
		case lly::FrontFace::CW: return GL_CW;
		}
		return GL_CCW;
	}

	GLenum to_stencil_func(lly::StencilFuncType func)
	{
		switch (func)
		{
		case lly::StencilFuncType::ALWAYS: return GL_ALWAYS;
		case lly::StencilFuncType::NEVER: return GL_NEVER;
		case lly::StencilFuncType::LESS: return GL_LESS;
		case lly::StencilFuncType::LEQUAL: return GL_LEQUAL;
		case lly::StencilFuncType::EQUAL: return GL_EQUAL;
		case lly::StencilFuncType::GREATER: return GL_GREATER;
		case lly::StencilFuncType::GEQUAL: return GL_GEQUAL;
		case lly::StencilFuncType::NOTEQUAL: return GL_NOTEQUAL;
		}
		return GL_ALWAYS;
	}

	GLenum to_stencil_op(lly::StencilOpType func)
	{
		switch (func)
		{
		case lly::StencilOpType::KEEP: return GL_KEEP;
		case lly::StencilOpType::ZERO: return GL_ZERO;
		case lly::StencilOpType::REPLACE: return GL_REPLACE;
		case lly::StencilOpType::INCR: return GL_INCR;
		case lly::StencilOpType::DECR: return GL_DECR;
		case lly::StencilOpType::INVERT: return GL_INVERT;
		}
		return GL_KEEP;
	}

	GLenum to_warp(lly::TextureWrap wrap)
	{
		switch (wrap)
		{
		case lly::TextureWrap::REPEAT: return GL_REPEAT;
		case lly::TextureWrap::CLAMP: return GL_CLAMP;
		case lly::TextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		}
		return GL_CLAMP_TO_EDGE;
	}

	bool eq(float a, float b)
	{
		return fabs(a - b) < 0.0000000001;
	}

	glm::mat4 to_sqt(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
	{
		glm::mat4 ret = glm::mat4_cast(rotation);
		ret = glm::scale(ret, scale);
		//ret[0][0] *= scale.x;
		//ret[1][1] *= scale.y;
		//ret[2][2] *= scale.z;

		ret[3][0] = position.x;
		ret[3][1] = position.y;
		ret[3][2] = position.z;
		
		return ret;
	}
}

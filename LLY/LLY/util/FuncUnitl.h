#ifndef FUNCUTIL_H_
#define FUNCUTIL_H_
#include <string>
#include "LLYIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace lly_util {

	GLenum to_usage(lly::VertexUsage usage);
	GLenum to_draw_mode(lly::DrawType mode);
	GLenum to_color_format(lly::ColorFormat format);
	int to_color_bpp(lly::ColorFormat format);
	GLenum to_color_type(lly::ColorFormat format);
	GLenum to_min_filter(lly::TextureMinFilter filter);
	GLenum to_mag_filter(lly::TextureMagFilter filter);
	GLuint to_position(lly::VertexAttribPosition position);
	GLenum to_data_type(lly::DataType type);
	GLenum to_blend_factor(lly::BlendFactor factor);
	GLenum to_alpha_test_func(lly::AlphaTestFunc func);
	GLenum to_depth_func(lly::DepthFunc func);
	GLenum to_cull_fase_side(lly::CullFaceSide side);
	GLenum to_front_face(lly::FrontFace front);
	GLenum to_stencil_func(lly::StencilFuncType func);
	GLenum to_stencil_op(lly::StencilOpType func);
	GLenum to_warp(lly::TextureWrap wrap);

	glm::mat4 to_sqt(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

	bool eq(float a, float b);
}
#endif //FUNCUTIL_H_

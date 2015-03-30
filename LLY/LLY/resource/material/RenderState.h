#ifndef RENDERSTATE_H_
#define RENDERSTATE_H_
#include "../../util/LLYIncludes.h"

namespace lly_util
{
	class Data;
}

namespace lly{
	class Device;

	class RenderState
	{
	public:
		enum Mask
		{
			ENABLE_BLEND = 0x00000001,
			BLEND_FUNC_SRC = 0x00000002,
			BLEND_FUNC_DST = 0x00000004,

			ENABLE_ALPHA = 0x00000008,
			ALPHA_FUNC = 0x00000010,
			ALPHA_FACTOR = 0x00000020,

			ENABLE_DEPTH = 0x00000040,
			DEPTH_WRITE = 0x00000080,
			DEPTH_FUNC = 0x00000100,

			ENABLE_CULL_FACE = 0x00000200,
			CULL_FACE_SIDE = 0x00000400,
			FRONT_FACE = 0x00000800,

			ENABLE_STENCIL_TEST = 0x00001000,
			STENCIL_WRITE_MASK = 0x00002000,
			STENCIL_FUNC = 0x00004000,
			STENCIL_FUNC_REF = 0x00008000,
			STENCIL_FUNC_MASK = 0x00010000,
			STENCIL_OP_S_FAIL = 0x00020000,
			STENCIL_OP_D_FAIL = 0x00040000,
			STENCIL_OP_D_PASS = 0x00080000,
		};

		RenderState();
		~RenderState();

		void use(Device& device);

		void set_blend(bool enable);
		void set_blend_func(BlendFactor src, BlendFactor dst);
		void set_alpha_test(bool enable);
		void set_alpha_test_func(AlphaTestFunc func, float factor);
		void set_depth_test(bool enable);
		void set_depth_test_write(bool write);
		void set_depth_test_func(DepthFunc func);
		void set_cull_face(bool enable);
		void set_cull_face_side(CullFaceSide side);
		void set_front_face(FrontFace front);
		void set_stencil_test(bool enable);
		void set_stencil_mask(unsigned int write_mask);
		void set_stencil_func(StencilFuncType func, int ref, unsigned int mask);
		void set_stencil_op(StencilOpType sf, StencilOpType df, StencilOpType dp);
		void set_mask(int mask) { _mask = mask; }

		void merge(const RenderState& rhs);

	private:
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

		unsigned int _mask;
	};

}

#endif //RENDERSTATE_H_


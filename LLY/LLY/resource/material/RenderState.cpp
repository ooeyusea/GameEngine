#include "RenderState.h"
#include "../../render/Device.h"
#include "../../util/Macros.h"

namespace lly{

	RenderState::RenderState()
		: _enable_blend(false)
		, _blend_func({BlendFactor::ONE, BlendFactor::ZERO})
		, _enable_alpha_test(false)
		, _alpha_test_func(AlphaTestFunc::ALWAYS)
		, _alpha_test_factor(0.0f)
		, _enable_depth_test(false)
		, _enable_depth_write(true)
		, _depth_func(DepthFunc::LESS)
		, _enable_cull_face(false)
		, _cull_face_side(CullFaceSide::BACK)
		, _front_face(FrontFace::CCW)
		, _enable_stencil_test(false)
		, _stencil_write_mask(0xff)
		, _stencil_func({StencilFuncType::ALWAYS, 0, 0xff})
		, _stencil_op({StencilOpType::KEEP, StencilOpType::KEEP, StencilOpType::KEEP})
	{
	}


	RenderState::~RenderState()
	{
	}

	void RenderState::use(Device& device)
	{
		device.set_blend(_enable_blend, _blend_func);
		device.set_alpha_test(_enable_alpha_test, _alpha_test_func, _alpha_test_factor);
		device.set_cull_face(_enable_cull_face, _cull_face_side, _front_face);
		device.set_depth_test(_enable_depth_test, _enable_depth_write, _depth_func);
		device.set_stencil_test(_enable_stencil_test, _stencil_write_mask, _stencil_func, _stencil_op);
	}

	void RenderState::set_blend(bool enable)
	{
		_enable_blend = enable;
	}

	void RenderState::set_blend_func(BlendFactor src, BlendFactor dst)
	{
		_blend_func = { src, dst };
	}

	void RenderState::set_alpha_test(bool enable)
	{
		_enable_alpha_test = enable;
	}

	void RenderState::set_alpha_test_func(AlphaTestFunc func, float factor)
	{
		_alpha_test_func = func;
		_alpha_test_factor = factor;
	}

	void RenderState::set_depth_test(bool enable)
	{
		_enable_depth_test = enable;
	}

	void RenderState::set_depth_test_write(bool write)
	{
		_enable_depth_write = write;
	}

	void RenderState::set_depth_test_func(DepthFunc func)
	{
		_depth_func = func;
	}
	
	void RenderState::set_cull_face(bool enable)
	{
		_enable_cull_face = enable;
	}

	void RenderState::set_cull_face_side(CullFaceSide side)
	{
		_cull_face_side = side;
	}

	void RenderState::set_front_face(FrontFace front)
	{
		_front_face = front;
	}

	void RenderState::set_stencil_test(bool enable)
	{
		_enable_stencil_test = enable;
	}

	void RenderState::set_stencil_mask(unsigned int write_mask)
	{
		_stencil_write_mask = write_mask;
	}

	void RenderState::set_stencil_func(StencilFuncType func, int ref, unsigned int mask)
	{
		_stencil_func = { func, ref, mask };
	}
	void RenderState::set_stencil_op(StencilOpType sf, StencilOpType df, StencilOpType dp)
	{
		_stencil_op = { sf, df, dp };
	}

	void RenderState::merge(const RenderState& rhs)
	{
		MERGE_RENDER_STATE(ENABLE_BLEND, _mask, _enable_blend);
		MERGE_RENDER_STATE(BLEND_FUNC_SRC, _mask, _blend_func.src);
		MERGE_RENDER_STATE(BLEND_FUNC_DST, _mask, _blend_func.dst);
		MERGE_RENDER_STATE(BLEND_FUNC_DST, _mask, _blend_func.dst);
		MERGE_RENDER_STATE(ENABLE_ALPHA, _mask, _enable_alpha_test);
		MERGE_RENDER_STATE(ALPHA_FUNC, _mask, _alpha_test_func);
		MERGE_RENDER_STATE(ALPHA_FACTOR, _mask, _alpha_test_factor);
		MERGE_RENDER_STATE(ENABLE_DEPTH, _mask, _enable_depth_test);
		MERGE_RENDER_STATE(DEPTH_WRITE, _mask, _enable_depth_write);
		MERGE_RENDER_STATE(DEPTH_FUNC, _mask, _depth_func);
		MERGE_RENDER_STATE(ENABLE_CULL_FACE, _mask, _enable_cull_face);
		MERGE_RENDER_STATE(CULL_FACE_SIDE, _mask, _cull_face_side);
		MERGE_RENDER_STATE(FRONT_FACE, _mask, _front_face);
		MERGE_RENDER_STATE(ENABLE_STENCIL_TEST, _mask, _enable_stencil_test);
		MERGE_RENDER_STATE(STENCIL_WRITE_MASK, _mask, _stencil_write_mask);
		MERGE_RENDER_STATE(STENCIL_FUNC, _mask, _stencil_func.func);
		MERGE_RENDER_STATE(STENCIL_FUNC_REF, _mask, _stencil_func.ref);
		MERGE_RENDER_STATE(STENCIL_FUNC_MASK, _mask, _stencil_func.mask);
		MERGE_RENDER_STATE(STENCIL_OP_S_FAIL, _mask, _stencil_op.s_fail);
		MERGE_RENDER_STATE(STENCIL_OP_D_FAIL, _mask, _stencil_op.d_fail);
		MERGE_RENDER_STATE(STENCIL_OP_D_PASS, _mask, _stencil_op.d_pass);
	}
}

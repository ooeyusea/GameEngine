#ifndef DEFINES_H_
#define DEFINES_H_

namespace lly {
	enum class VertexUsage
	{
		STATIC_DRAW,
		STATIC_COPY,
		STATIC_READ,
		STREAM_DRAW,
		STREAM_COPY,
		STREAM_READ,
		DYNAMIC_DRAW,
		DYNAMIC_COPY,
		DYNAMIC_READ,
	};

	enum class DrawType
	{
		POINTS,
		LINE_STRIP,
		LINE_LOOP,
		LINES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		TRIANGLES,
		QUAD_STRIP,
		QUADS,
		POLYGON,
	};

	enum class VertexAttribPosition
	{
		POSITION,
		NORMAL,
		COLOR,
		TANGENT,
		TEXCOOD0,
		TEXCOOD1,
		TEXCOOD2,
		TEXCOOD3,
		TEXCOOD4,
		TEXCOOD5,
		TEXCOOD6,
		TEXCOOD7,
		WEIGHT0,
		WEIGHT1,
		WEIGHT2,
		WEIGHT3,
	};

	enum class DataType
	{
		BYTE,
		UNSIGNED_BYTE,
		SHORT,
		UNSIGNED_SHORT,
		FIXED,
		FLOAT,
	};

	enum class ColorFormat
	{
		DEFAULT,
		BGRA8888,
		RGBA8888,
		RGB888,
		RGB565,
		A8,
		I8,
		AI88,
		RGBA4444,
		RGB5A1,
	};

	enum class TextureMinFilter
	{
		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST,
		LINEAR_MIPMAP_LINEAR,
	};

	enum class TextureMagFilter
	{
		NEAREST,
		LINEAR,
	};

	enum class TextureWrap
	{
		REPEAT,
		CLAMP,
		CLAMP_TO_EDGE,
	};

	enum class TextureFileType
	{
		PNG,
		JPG,
		TIFF,
		WEBP,
		TGA,
		UNKOWN
	};

	enum class BlendFactor
	{
		DST_ALPHA,
		DST_COLOR,
		ONE,
		ONE_MINUS_DST_ALPHA,
		ONE_MINUS_DST_COLOR,
		ONE_MINUS_SRC_ALPHA,
		SRC_ALPHA,
		SRC_ALPHA_SATURATE,
		ZERO
	};

	struct BlendFunc
	{
		BlendFactor src;
		BlendFactor dst;

		bool operator!=(const BlendFunc& rhs) const
		{
			return src != rhs.src || dst != rhs.dst;
		}
	};

	enum class AlphaTestFunc
	{
		ALWAYS,
		NEVER,
		LESS,
		LEQUAL,
		EQUAL,
		GREATER,
		GEQUAL,
		NOTEQUAL
	};

	enum ClearTarget
	{
		None = 0x0,
		COLOR = 0x1,
		DEPTH = 0x2,
		ACCUM = 0x4,
		STENCIL = 0x8,
	};

	enum class StencilFuncType
	{
		ALWAYS,
		NEVER,
		LESS,
		LEQUAL,
		EQUAL,
		GREATER,
		GEQUAL,
		NOTEQUAL
	};

	struct StencilFunc
	{
		StencilFuncType func;
		int ref;
		unsigned int mask;

		bool operator!=(const StencilFunc& rhs) const
		{
			return func != rhs.func || ref != rhs.ref || mask != rhs.mask;
		}
	};

	enum class StencilOpType
	{
		KEEP,
		ZERO,
		REPLACE,
		INCR,
		DECR,
		INVERT,
	};

	struct StencilOp
	{
		StencilOpType s_fail;
		StencilOpType d_fail;
		StencilOpType d_pass;

		bool operator!=(const StencilOp& rhs) const
		{
			return s_fail != rhs.s_fail || d_fail != rhs.d_fail || d_pass != rhs.d_pass;
		}
	};

	enum class DepthFunc
	{
		ALWAYS,
		NEVER,
		LESS,
		LEQUAL,
		EQUAL,
		GREATER,
		GEQUAL,
		NOTEQUAL,
	};

	enum class CullFaceSide
	{
		FRONT,
		BACK,
	};

	enum class FrontFace
	{
		CCW,
		CW,
	};
}

#endif //DEFINES_H_


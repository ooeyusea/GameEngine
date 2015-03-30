#include "MaterialLoader.h"
#include "Material.h"
#include "../../util/FileUtil.h"
#include <list>
#include "../../util/Macros.h"
#include "RenderState.h"
#include "Pass.h"
#include "Technique.h"
#include "Uniform.h"
#include <cctype>
#include <regex>
#include "../../System.h"

namespace lly
{
	namespace material_loader
	{
		template <typename T>
		class AutoDeletor
		{
		public:
			explicit AutoDeletor(T* t) : _t(t) {}
			~AutoDeletor()
			{
				LLY_SAFE_DELETE(_t);
			}

			void no() { _t = nullptr; }

		private:
			T* _t;
		};

		std::string get_word(std::list<std::string>& words)
		{
			if (words.empty())
				throw std::logic_error("words eof");
			std::string ret = *words.begin();
			words.pop_front();
			return std::move(ret);
		}

		bool to_switch(const std::string& word)
		{
			if (word == "on")
				return true;
			else if (word == "off")
				return false;
			throw std::logic_error("parse switch error");
		}

		bool to_bool(const std::string& word)
		{
			if (word == "true")
				return true;
			else if (word == "false")
				return false;
			throw std::logic_error("parse switch error");
		}

		int to_int(const std::string& word)
		{
			std::regex pattern("^[+-]?[0-9]+$");
			if (!std::regex_match(word, pattern))
			{
				throw std::logic_error("parse int error");
			}
			
			return atoi(word.c_str());
		}

		unsigned int to_unsigned_int(const std::string& word)
		{
			std::regex pattern("^[0-9]+$");
			if (!std::regex_match(word, pattern))
			{
				std::regex pattern_16("^0x[0-9a-fA-F]+$");
				if (!std::regex_match(word, pattern_16))
					throw std::logic_error("parse int error");

				return strtoul(word.substr(2).c_str(), nullptr, 16);
			}
			return strtoul(word.c_str(), nullptr, 10);
		}

		float to_float(const std::string& word)
		{
			std::regex pattern("^[+-]?([0-9]+\\.[0-9]+)([eE][+-]?[0-9]+)?$");
			if (!std::regex_match(word, pattern))
			{
				throw std::logic_error("parse float error");
			}
			
			return (float)atof(word.c_str());
		}

		bool is_inner(const std::string& word)
		{
			if (word == "MODEL_MATRIX")
				return true;
			else if (word == "VIEW_MATRIX")
				return true;
			else if (word == "PROJECTION_MATRIX")
				return true;
			else if (word == "VIEW_PROJECTION_MATRIX")
				return true;
			else if (word == "WORLD_VIEW_PROJECTION_MATRIX")
				return true;
			else if (word == "INVERSE_TRANSPOSE_WORLD_MATRIX")
				return true;
			else if (word == "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX")
				return true;
			else if (word == "CAMERA_WORLD_POSITION")
				return true;
			else if (word == "CAMERA_VIEW_POSITION")
				return true;
			else if (word == "MATRIX_PALETTE")
				return true;
			return false;
		}

		Uniform::Usage to_inner(const std::string& word)
		{
			if (word == "MODEL_MATRIX")
				return Uniform::Usage::MODEL_MATRIX;
			else if (word == "VIEW_MATRIX")
				return Uniform::Usage::VIEW_MATRIX;
			else if (word == "PROJECTION_MATRIX")
				return Uniform::Usage::PROJECTION_MATRIX;
			else if (word == "VIEW_PROJECTION_MATRIX")
				return Uniform::Usage::VIEW_PROJECTION_MATRIX;
			else if (word == "WORLD_VIEW_PROJECTION_MATRIX")
				return Uniform::Usage::WORLD_VIEW_PROJECTION_MATRIX;
			else if (word == "INVERSE_TRANSPOSE_WORLD_MATRIX")
				return Uniform::Usage::INVERSE_TRANSPOSE_WORLD_MATRIX;
			else if (word == "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX")
				return Uniform::Usage::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX;
			else if (word == "CAMERA_WORLD_POSITION")
				return Uniform::Usage::CAMERA_WORLD_POSITION;
			else if (word == "CAMERA_VIEW_POSITION")
				return Uniform::Usage::CAMERA_VIEW_POSITION;
			else if (word == "MATRIX_PALETTE")
				return Uniform::Usage::MATRIX_PALETTE;
			
			throw std::logic_error("parse usage error");
		}

		std::string to_path(const std::string& word)
		{
			/*std::regex pattern("^([\\w-]+)*$");
			if (!std::regex_match(word, pattern))
			{
				throw std::logic_error("parse path error");
			}*/
			
			return word;
		}

		std::string to_var(const std::string& word)
		{
			std::regex pattern("^[a-zA-Z][a-zA-Z0-9_]*$");
			if (!std::regex_match(word, pattern))
			{
				throw std::logic_error("parse var error");
			}

			return word;
		}

		std::vector<float> to_float_array(std::list<std::string>& words)
		{
			std::vector<float> ret;
			ret.push_back(to_float(get_word(words)));

			while (true)
			{
				if (words.empty())
					break;

				std::string tmp = get_word(words);
				if (tmp == ",")
				{
					ret.push_back(to_float(get_word(words)));
				}
				else
				{
					words.push_front(tmp);
					break;
				}
			}

			return std::move(ret);
		}
		
		BlendFactor to_blend_factor(const std::string& word)
		{
			if (word == "dst_alpha")
				return BlendFactor::DST_ALPHA;
			else if (word == "dst_color")
				return BlendFactor::DST_COLOR;
			else if (word == "one")
				return BlendFactor::ONE;
			else if (word == "one_minus_dst_alpha")
				return BlendFactor::ONE_MINUS_DST_ALPHA;
			else if (word == "one_minus_dst_color")
				return BlendFactor::ONE_MINUS_DST_COLOR;
			else if (word == "one_minus_src_alpha")
				return BlendFactor::ONE_MINUS_SRC_ALPHA;
			else if (word == "src_alpha")
				return BlendFactor::SRC_ALPHA;
			else if (word == "src_alpha_saturate")
				return BlendFactor::SRC_ALPHA_SATURATE;
			else if (word == "zero")
				return BlendFactor::ZERO;

			throw std::logic_error("parse blend factor error");
		}

		AlphaTestFunc to_alpha_test_func(const std::string& word)
		{
			if (word == "always")
				return AlphaTestFunc::ALWAYS;
			else if (word == "never")
				return AlphaTestFunc::NEVER;
			else if (word == "less")
				return AlphaTestFunc::LESS;
			else if (word == "lequal")
				return AlphaTestFunc::LEQUAL;
			else if (word == "equal")
				return AlphaTestFunc::EQUAL;
			else if (word == "greater")
				return AlphaTestFunc::GREATER;
			else if (word == "gequal")
				return AlphaTestFunc::GEQUAL;
			else if (word == "notequal")
				return AlphaTestFunc::NOTEQUAL;

			throw std::logic_error("parse alpha test func error");
		}

		DepthFunc to_depth_func(const std::string& word)
		{
			if (word == "always")
				return DepthFunc::ALWAYS;
			else if (word == "never")
				return DepthFunc::NEVER;
			else if (word == "less")
				return DepthFunc::LESS;
			else if (word == "lequal")
				return DepthFunc::LEQUAL;
			else if (word == "equal")
				return DepthFunc::EQUAL;
			else if (word == "greater")
				return DepthFunc::GREATER;
			else if (word == "gequal")
				return DepthFunc::GEQUAL;
			else if (word == "notequal")
				return DepthFunc::NOTEQUAL;

			throw std::logic_error("parse depth test func error");
		}

		CullFaceSide to_cull_face_side(const std::string& word)
		{
			if (word == "front")
				return CullFaceSide::FRONT;
			else if (word == "back")
				return CullFaceSide::BACK;
			throw std::logic_error("parse cull face side error");
		}

		FrontFace to_front_face(const std::string& word)
		{
			if (word == "ccw")
				return FrontFace::CCW;
			else if (word == "cw")
				return FrontFace::CW;
			throw std::logic_error("parse front face error");
		}

		StencilFuncType to_stencil_func(const std::string& word)
		{
			if (word == "always")
				return StencilFuncType::ALWAYS;
			else if (word == "never")
				return StencilFuncType::NEVER;
			else if (word == "less")
				return StencilFuncType::LESS;
			else if (word == "lequal")
				return StencilFuncType::LEQUAL;
			else if (word == "equal")
				return StencilFuncType::EQUAL;
			else if (word == "greater")
				return StencilFuncType::GREATER;
			else if (word == "gequal")
				return StencilFuncType::GEQUAL;
			else if (word == "notequal")
				return StencilFuncType::NOTEQUAL;
			throw std::logic_error("parse stencil func error");
		}

		StencilOpType to_stencil_op(const std::string& word)
		{
			if (word == "keep")
				return StencilOpType::KEEP;
			else if (word == "zero")
				return StencilOpType::ZERO;
			else if (word == "replace")
				return StencilOpType::REPLACE;
			else if (word == "incr")
				return StencilOpType::INCR;
			else if (word == "decr")
				return StencilOpType::DECR;
			else if (word == "invert")
				return StencilOpType::INVERT;
			throw std::logic_error("parse stencil op error");
		}

		TextureWrap to_texture_warp(const std::string& word)
		{
			if (word == "repeat")
				return TextureWrap::REPEAT;
			else if (word == "clamp")
				return TextureWrap::CLAMP;
			else if (word == "edge")
				return TextureWrap::CLAMP_TO_EDGE;
			throw std::logic_error("parse texture warp error");
		}

		TextureMinFilter to_texture_min_filter(const std::string& word)
		{
			if (word == "nearest")
				return TextureMinFilter::NEAREST;
			else if (word == "linear")
				return TextureMinFilter::LINEAR;
			else if (word == "nearest_nearest")
				return TextureMinFilter::NEAREST_MIPMAP_NEAREST;
			else if (word == "nearest_linear")
				return TextureMinFilter::NEAREST_MIPMAP_LINEAR;
			else if (word == "linear_nearest")
				return TextureMinFilter::LINEAR_MIPMAP_NEAREST;
			else if (word == "linear_linear")
				return TextureMinFilter::LINEAR_MIPMAP_LINEAR;

			throw std::logic_error("parse texture min filter error");
		}

		TextureMagFilter to_texture_mag_filter(const std::string& word)
		{
			if (word == "nearest")
				return TextureMagFilter::NEAREST;
			else if (word == "linear")
				return TextureMagFilter::LINEAR;
			throw std::logic_error("parse texture mag filter error");
		}

		Uniform load_sampler2d(std::list<std::string>& words)
		{
			Uniform ret;
			ret.set_name(to_var(get_word(words)));

			std::string tmp = get_word(words);
			if (tmp != "{")
				throw std::logic_error("load render state but format error");

			unsigned int mask = 0;
			unsigned int id = 0;
			bool mipmap = false;
			TextureWrap s = TextureWrap::CLAMP;
			TextureWrap t = TextureWrap::CLAMP;
			TextureMinFilter min_filter = TextureMinFilter::NEAREST;
			TextureMagFilter mag_filter = TextureMagFilter::NEAREST;

			while (!words.empty())
			{
				tmp = get_word(words);
				if (tmp == "}")
				{
					break;
				}
				else if (tmp == "path")
				{
					id = System::instance().get_resource_manager().load_texture_2d(to_path(get_word(words)));
				}
				else if (tmp == "mipmap")
				{
					mipmap = to_bool(get_word(words));
					mask |= Uniform::Mask::MIPMAP;
				}
				else if (tmp == "warp")
				{
					s = to_texture_warp(get_word(words));
					t = to_texture_warp(get_word(words));
					mask |= (Uniform::Mask::WARP_S | Uniform::Mask::WARP_T);
				}
				else if (tmp == "filter")
				{
					min_filter = to_texture_min_filter(get_word(words));
					mag_filter = to_texture_mag_filter(get_word(words));
					mask |= (Uniform::Mask::MIN_FILTER | Uniform::Mask::MAG_FILTER);
				}
			}

			ret.set_value(id, mipmap, s, t, min_filter, mag_filter, mask);
			return std::move(ret);
		}

		RenderState load_render_state(std::list<std::string>& words)
		{
			std::string tmp = get_word(words);
			if (tmp != "{")
				throw std::logic_error("load render state but format error");

			RenderState ret;
			unsigned int mask = 0;
			while (!words.empty())
			{
				tmp = get_word(words);
				if (tmp == "}")
				{
					break;
				}
				else if (tmp == "blend")
				{
					ret.set_blend(to_switch(get_word(words)));
					mask |= RenderState::Mask::ENABLE_BLEND;
				}
				else if (tmp == "blend_func")
				{
					BlendFactor src = to_blend_factor(get_word(words));
					BlendFactor dst = to_blend_factor(get_word(words));
					ret.set_blend_func(src, dst);
					mask |= (RenderState::Mask::BLEND_FUNC_SRC | RenderState::Mask::BLEND_FUNC_DST);
				}
				else if (tmp == "alpha")
				{
					ret.set_alpha_test(to_switch(get_word(words)));
					mask |= RenderState::Mask::ENABLE_ALPHA;
				}
				else if (tmp == "alpha_func")
				{
					AlphaTestFunc func = to_alpha_test_func(get_word(words));
					float factor = to_float(get_word(words));
					ret.set_alpha_test_func(func, factor);
					mask |= (RenderState::Mask::ALPHA_FUNC | RenderState::Mask::ALPHA_FACTOR);
				}
				else if (tmp == "depth")
				{
					ret.set_depth_test(to_switch(get_word(words)));
					mask |= RenderState::Mask::ENABLE_DEPTH;
				}
				else if (tmp == "depth_write")
				{
					ret.set_depth_test_write(to_switch(get_word(words)));
					mask |= RenderState::Mask::DEPTH_WRITE;
				}
				else if (tmp == "depth_func")
				{
					ret.set_depth_test_func(to_depth_func(get_word(words)));
					mask |= RenderState::Mask::DEPTH_FUNC;
				}
				else if (tmp == "cull_face")
				{
					ret.set_cull_face(to_switch(get_word(words)));
					mask |= RenderState::Mask::ENABLE_CULL_FACE;
				}
				else if (tmp == "cull_face_side")
				{
					ret.set_cull_face_side(to_cull_face_side(get_word(words)));
					mask |= RenderState::Mask::CULL_FACE_SIDE;
				}
				else if (tmp == "front_face")
				{
					ret.set_front_face(to_front_face(get_word(words)));
					mask |= RenderState::Mask::FRONT_FACE;
				}
				else if (tmp == "stencil")
				{
					ret.set_stencil_test(to_switch(get_word(words)));
					mask |= RenderState::Mask::ENABLE_STENCIL_TEST;
				}
				else if (tmp == "stencil_mask")
				{
					ret.set_stencil_mask(to_unsigned_int(get_word(words)));
					mask |= RenderState::Mask::STENCIL_WRITE_MASK;
				}
				else if (tmp == "stencil_func")
				{
					StencilFuncType func = to_stencil_func(get_word(words));
					int ref = to_int(get_word(words));
					unsigned int mask = to_unsigned_int(get_word(words));
					ret.set_stencil_func(func, ref, mask);
					mask |= (RenderState::Mask::STENCIL_FUNC | RenderState::Mask::STENCIL_FUNC_REF | RenderState::Mask::STENCIL_FUNC_MASK);
				}
				else if (tmp == "stencil_op")
				{
					StencilOpType sf = to_stencil_op(get_word(words));
					StencilOpType df = to_stencil_op(get_word(words));
					StencilOpType dp = to_stencil_op(get_word(words));
					ret.set_stencil_op(sf, df, dp);
					mask |= (RenderState::Mask::STENCIL_FUNC | RenderState::Mask::STENCIL_FUNC_REF | RenderState::Mask::STENCIL_FUNC_MASK);
				}
			}

			ret.set_mask(mask);
			return std::move(ret);
		}

		Uniform load_uniform(std::list<std::string>& words)
		{
			std::string tmp = get_word(words);
			if (tmp != "=")
				throw std::logic_error("parse uniform miss =");

			Uniform ret;
			std::string value = get_word(words);
			if (is_inner(value))
			{
				ret.set_value(to_inner(value));
				return std::move(ret);
			}
			
			words.push_front(value);
			std::vector<float> values = to_float_array(words);
			if (values.size() == 1)
			{
				ret.set_value(values[0]);
			}
			else if (values.size() == 2)
			{
				ret.set_value(values[0], values[1]);
			}
			else if (values.size() == 3)
			{
				ret.set_value(values[0], values[1], values[2]);
			}
			else if (values.size() == 4)
			{
				ret.set_value(values[0], values[1], values[2], values[3]);
			}
			else if (values.size() == 16)
			{
				ret.set_matrix(values.data());
			}
			else
				throw std::logic_error("load uniform float array size invalid");

			return std::move(ret);
		}

		Pass* load_pass(std::list<std::string>& words)
		{
			std::string tmp = get_word(words);
			if (tmp != "{")
				throw std::logic_error("load pass but format error");

			Pass * ret = new Pass;
			AutoDeletor<Pass> deletor(ret);

			std::string vertex;
			std::string fragment;
			std::string tess_control;
			std::string tess_evaluation;
			std::string geometry;

			while (!words.empty())
			{
				tmp = get_word(words);
				if (tmp == "}")
				{
					
					if (vertex != "" && fragment != "")
					{
						ret->set_program(System::instance().get_resource_manager().load_program(vertex, fragment, tess_control, tess_evaluation, geometry));
					}
					else if (vertex != "" || fragment != "")
					{
						throw std::runtime_error("parse pass miss vertex or fragment shader");
					}

					deletor.no();
					return ret;
				}
				else if (tmp == "vertex")
				{
					vertex = to_path(get_word(words));
				}
				else if (tmp == "fragment")
				{
					fragment = to_path(get_word(words));
				}
				else if (tmp == "tess_control")
				{
					tess_control = to_path(get_word(words));
				}
				else if (tmp == "tess_evaluation")
				{
					tess_evaluation = to_path(get_word(words));
				}
				else if (tmp == "geometry")
				{
					geometry = to_path(get_word(words));
				}
				else if (tmp == "sampler_2d")
				{
					Uniform uniform = load_sampler2d(words);
					ret->add_uniform(uniform);
				}
				else if (tmp == "renderstate")
				{
					RenderState state = load_render_state(words);
					ret->set_render_state(state);
				}
				else
				{
					Uniform uniform = load_uniform(words);
					uniform.set_name(to_var(tmp));
					ret->add_uniform(uniform);
				}
			}

			throw std::logic_error("load pass but miss }");
		}

		Technique* load_technique(std::list<std::string>& words)
		{
			Technique * ret = new Technique;
			AutoDeletor<Technique> deletor(ret);

			std::string tmp = get_word(words);
			if (tmp != "{")
			{
				ret->set_name(to_var(tmp));

				tmp = get_word(words);
				if (tmp == "[")
				{
					ret->set_id(to_int(get_word(words)));

					tmp = get_word(words);
					if (tmp != "]")
						throw std::logic_error("load technique but id invalid");
					else
					{
						tmp = get_word(words);
						if (tmp != "{")
							throw std::logic_error("load technique but miss {");
					}
				}
				else if (tmp != "{")
					throw std::logic_error("load technique but miss {");
				else
				{

				}
			}
			else
			{

			}

			while (!words.empty())
			{
				tmp = get_word(words);
				if (tmp == "pass")
				{
					ret->add_pass(load_pass(words));
				}
				else if (tmp == "sampler_2d")
				{
					Uniform uniform = load_sampler2d(words);
					ret->add_uniform(uniform);
				}
				else if (tmp == "renderstate")
				{
					RenderState state = load_render_state(words);
					ret->set_render_state(state);
				}
				else if (tmp == "}")
				{
					deletor.no();
					return ret;
				}
				else
				{
					Uniform uniform = load_uniform(words);
					ret->add_uniform(uniform);
				}
			}

			throw std::logic_error("load technique but miss }");
		}

		Material* load(std::list<std::string>& words)
		{
			if (words.empty())
				throw std::logic_error("load one material but no words");

			std::string key_word = get_word(words);
			if (key_word != "material")
				throw std::logic_error("load one material but key word is not material");

			Material * ret = new Material;
			AutoDeletor<Material> deletor(ret);

			ret->set_name(to_var(get_word(words)));

			std::string tmp = get_word(words);
			if (tmp != "{")
				throw std::logic_error("load one material but miss {");

			while (!words.empty())
			{
				tmp = get_word(words);
				if (tmp == "technique")
				{
					ret->add_technique(load_technique(words));
				}
				else if (tmp == "sampler_2d")
				{
					Uniform uniform = load_sampler2d(words);
					ret->add_uniform(uniform);
				}
				else if (tmp == "renderstate")
				{
					RenderState state = load_render_state(words);
					ret->set_render_state(state);
				}
				else if (tmp == "}")
				{
					deletor.no();
					ret->merge_render_option();
					return ret;
				}
				else
				{
					Uniform uniform = load_uniform(words);
					ret->add_uniform(uniform);
				}
			}

			throw std::logic_error("load one material but miss }");
		}

		Material* load_material(const std::string& filename)
		{
			lly_util::Data data = lly_util::load_from_file(filename);
			std::list<std::string> words;
			int offset = 0;
			while (true)
			{
				if (!data.skip_white_space(offset))
					break;

				std::string word;

				if (data.cmp("/*", offset))
				{
					if (!data.next("*/", offset))
						throw std::logic_error("material format error");
				}
				else if (data.cmp("//", offset))
				{
					if (!data.next("\n", offset))
						throw std::logic_error("material format error");
				}
				else if (data.get_word(offset, word))
				{
					words.push_back(word);
				}
				else
				{
					throw std::logic_error("material format error");
				}
			}

			return material_loader::load(words);
		}
	}
}
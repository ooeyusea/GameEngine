#include "Uniform.h"
#include "../../render/Device.h"
#include "../../util/Macros.h"
#include "../../System.h"
#include <glm/glm.hpp>

namespace lly {

	Uniform::Uniform()
		: _type(ValueType::NONE)
	{

	}

	Uniform::Uniform(const Uniform& rhs)
	{
		_name = rhs._name;
		_type = rhs._type;

		switch (_type)
		{
		case ValueType::INNER: _value.usage = rhs._value.usage; break;
		case ValueType::FLOAT: _value.float1 = rhs._value.float1; break;
		case ValueType::FLOAT2: _value.float2 = rhs._value.float2; break;
		case ValueType::FLOAT3: _value.float3 = rhs._value.float3; break;
		case ValueType::FLOAT4: _value.float4 = rhs._value.float4; break;
		case ValueType::FLOAT_ARRAY: 
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float)* rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* rhs._value.float_array.count);
			}
			break;
		case ValueType::FLOAT2_ARRAY:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float) * 2 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float) * 2 * rhs._value.float_array.count);
			}
			break;
		case ValueType::FLOAT3_ARRAY:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float) * 3 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float) * 3 * rhs._value.float_array.count);
			}
			break;
		case ValueType::FLOAT4_ARRAY:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float) * 4 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float) * 4 * rhs._value.float_array.count);
			}
			break;
		case ValueType::INT: _value.int1 = rhs._value.int1; break;
		case ValueType::INT2: _value.int2 = rhs._value.int2; break;
		case ValueType::INT3: _value.int3 = rhs._value.int3; break;
		case ValueType::INT4: _value.int4 = rhs._value.int4; break;
		case ValueType::INT_ARRAY: 
			{
				_value.int_array.ptr = (int*)malloc(sizeof(int) * rhs._value.int_array.count);
				_value.int_array.count = rhs._value.int_array.count;
				memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* rhs._value.int_array.count);
			}
			break;
		case ValueType::INT2_ARRAY:
			{
				_value.int_array.ptr = (int*)malloc(sizeof(int) * 2 * rhs._value.int_array.count);
				_value.int_array.count = rhs._value.int_array.count;
				memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 2 * rhs._value.int_array.count);
			}
			break;
		case ValueType::INT3_ARRAY:
			{
				_value.int_array.ptr = (int*)malloc(sizeof(int) * 3 * rhs._value.int_array.count);
				_value.int_array.count = rhs._value.int_array.count;
				memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 3 * rhs._value.int_array.count);
			}
			break;
		case ValueType::INT4_ARRAY:
			{
				_value.int_array.ptr = (int*)malloc(sizeof(int) * 4 * rhs._value.int_array.count);
				_value.int_array.count = rhs._value.int_array.count;
				memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 4 * rhs._value.int_array.count);
			}
			break;
		case ValueType::MATRIX2: 
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float) * 4 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float) * 4 * rhs._value.float_array.count);
			}
			break;
		case ValueType::MATRIX3: 
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float) * 9 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float) * 9 * rhs._value.float_array.count);
			}
			break;
		case ValueType::MATRIX4:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float) * 16 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float) * 16 * rhs._value.float_array.count);
			}
			break;
		case ValueType::SAMPLER2D: _value.sampler_2d = rhs._value.sampler_2d; break;
		}
	}

	Uniform::Uniform(Uniform&& rhs)
	{
		std::swap(_name, rhs._name);
		std::swap(_type, rhs._type);
		std::swap(_value, rhs._value);
	}

	Uniform::~Uniform()
	{
		switch (_type)
		{
		case ValueType::INT_ARRAY: 
		case ValueType::INT2_ARRAY: 
		case ValueType::INT3_ARRAY: 
		case ValueType::INT4_ARRAY: 
			LLY_SAFE_DELETE(_value.int_array.ptr); 
			break;
		case ValueType::FLOAT_ARRAY: 
		case ValueType::FLOAT2_ARRAY: 
		case ValueType::FLOAT3_ARRAY:
		case ValueType::FLOAT4_ARRAY:
		case ValueType::MATRIX2:
		case ValueType::MATRIX3:
		case ValueType::MATRIX4: 
			LLY_SAFE_DELETE(_value.float_array.ptr); 
			break;
		}
	}

	Uniform& Uniform::operator=(const Uniform& rhs)
	{
		_name = rhs._name;
		_type = rhs._type;

		switch (_type)
		{
		case ValueType::INNER: _value.usage = rhs._value.usage; break;
		case ValueType::FLOAT: _value.float1 = rhs._value.float1; break;
		case ValueType::FLOAT2: _value.float2 = rhs._value.float2; break;
		case ValueType::FLOAT3: _value.float3 = rhs._value.float3; break;
		case ValueType::FLOAT4: _value.float4 = rhs._value.float4; break;
		case ValueType::FLOAT_ARRAY:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float)* rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* rhs._value.float_array.count);
			}
			break;
		case ValueType::FLOAT2_ARRAY:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float)* 2 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 2 * rhs._value.float_array.count);
			}
			break;
		case ValueType::FLOAT3_ARRAY:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float)* 3 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 3 * rhs._value.float_array.count);
			}
			break;
		case ValueType::FLOAT4_ARRAY:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float)* 4 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 4 * rhs._value.float_array.count);
			}
			break;
		case ValueType::INT: _value.int1 = rhs._value.int1; break;
		case ValueType::INT2: _value.int2 = rhs._value.int2; break;
		case ValueType::INT3: _value.int3 = rhs._value.int3; break;
		case ValueType::INT4: _value.int4 = rhs._value.int4; break;
		case ValueType::INT_ARRAY:
			{
				_value.int_array.ptr = (int*)malloc(sizeof(int)* rhs._value.int_array.count);
				_value.int_array.count = rhs._value.int_array.count;
				memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* rhs._value.int_array.count);
			}
			break;
		case ValueType::INT2_ARRAY:
			{
				_value.int_array.ptr = (int*)malloc(sizeof(int)* 2 * rhs._value.int_array.count);
				_value.int_array.count = rhs._value.int_array.count;
				memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 2 * rhs._value.int_array.count);
			}
			break;
		case ValueType::INT3_ARRAY:
			{
				_value.int_array.ptr = (int*)malloc(sizeof(int)* 3 * rhs._value.int_array.count);
				_value.int_array.count = rhs._value.int_array.count;
				memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 3 * rhs._value.int_array.count);
			}
			break;
		case ValueType::INT4_ARRAY:
			{
				_value.int_array.ptr = (int*)malloc(sizeof(int)* 4 * rhs._value.int_array.count);
				_value.int_array.count = rhs._value.int_array.count;
				memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 4 * rhs._value.int_array.count);
			}
			break;
		case ValueType::MATRIX2:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float)* 4 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 4 * rhs._value.float_array.count);
			}
			break;
		case ValueType::MATRIX3:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float)* 9 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 9 * rhs._value.float_array.count);
			}
			break;
		case ValueType::MATRIX4:
			{
				_value.float_array.ptr = (float*)malloc(sizeof(float)* 16 * rhs._value.float_array.count);
				_value.float_array.count = rhs._value.float_array.count;
				memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 16 * rhs._value.float_array.count);
			}
			break;
		case ValueType::SAMPLER2D: _value.sampler_2d = rhs._value.sampler_2d; break;
		}
		return *this;
	}

	Uniform& Uniform::operator=(Uniform&& rhs)
	{
		std::swap(_name, rhs._name);
		std::swap(_type, rhs._type);
		std::swap(_value, rhs._value);
		return *this;
	}

	void Uniform::use(Device& device)
	{
		switch (_type)
		{
		case ValueType::FLOAT: device.setUniformWith1f(_name, _value.float1); break;
		case ValueType::FLOAT2: device.setUniformWith2f(_name, _value.float2.a, _value.float2.b);  break;
		case ValueType::FLOAT3: device.setUniformWith3f(_name, _value.float3.a, _value.float3.b, _value.float3.c);  break;
		case ValueType::FLOAT4: device.setUniformWith4f(_name, _value.float4.a, _value.float4.b, _value.float4.c, _value.float4.d);  break;
		case ValueType::FLOAT_ARRAY: device.setUniformWith1fv(_name, _value.float_array.ptr, _value.float_array.count); break;
		case ValueType::FLOAT2_ARRAY: device.setUniformWith2fv(_name, _value.float_array.ptr, _value.float_array.count); break;
		case ValueType::FLOAT3_ARRAY: device.setUniformWith3fv(_name, _value.float_array.ptr, _value.float_array.count); break;
		case ValueType::FLOAT4_ARRAY: device.setUniformWith4fv(_name, _value.float_array.ptr, _value.float_array.count); break;
		case ValueType::INT: device.setUniformWith1i(_name, _value.int1); break;
		case ValueType::INT2: device.setUniformWith2i(_name, _value.int2.a, _value.int2.b);  break;
		case ValueType::INT3: device.setUniformWith3i(_name, _value.int3.a, _value.int3.b, _value.int3.c);  break;
		case ValueType::INT4: device.setUniformWith4i(_name, _value.int4.a, _value.int4.b, _value.int4.c, _value.int4.d);  break;
		case ValueType::INT_ARRAY: device.setUniformWith1iv(_name, _value.int_array.ptr, _value.int_array.count); break;
		case ValueType::INT2_ARRAY: device.setUniformWith2iv(_name, _value.int_array.ptr, _value.int_array.count); break;
		case ValueType::INT3_ARRAY: device.setUniformWith3iv(_name, _value.int_array.ptr, _value.int_array.count); break;
		case ValueType::INT4_ARRAY: device.setUniformWith4iv(_name, _value.int_array.ptr, _value.int_array.count); break;
		case ValueType::MATRIX2: device.setUniformWithMatrix2fv(_name, _value.float_array.ptr, _value.float_array.count);; break;
		case ValueType::MATRIX3: device.setUniformWithMatrix3fv(_name, _value.float_array.ptr, _value.float_array.count);; break;
		case ValueType::MATRIX4: device.setUniformWithMatrix4fv(_name, _value.float_array.ptr, _value.float_array.count);; break;
		case ValueType::SAMPLER2D: device.setUniformWithTex(_name, _value.sampler_2d.texture.ptr, _value.sampler_2d.mipmap, _value.sampler_2d.s, _value.sampler_2d.t, _value.sampler_2d.min, _value.sampler_2d.mag);  break;
		}
	}

	void Uniform::set_value(Usage usage)
	{
		check_and_set_type(ValueType::INNER);
		_value.usage = usage;
	}

	void Uniform::set_value(float a)
	{
		check_and_set_type(ValueType::FLOAT);
		_value.float1 = a;
	}

	void Uniform::set_value(float a, float b)
	{
		check_and_set_type(ValueType::FLOAT2);
		_value.float2.a = a;
		_value.float2.b = b;
	}

	void Uniform::set_value(float a, float b, float c)
	{
		check_and_set_type(ValueType::FLOAT3);
		_value.float3.a = a;
		_value.float3.b = b;
		_value.float3.c = c;
	}

	void Uniform::set_value(float a, float b, float c, float d)
	{
		check_and_set_type(ValueType::FLOAT4);
		_value.float4.a = a;
		_value.float4.b = b;
		_value.float4.c = c;
		_value.float4.d = d;
	}

	void Uniform::set_value(float* ptr, int count)
	{
		if (!check_and_set_type(ValueType::FLOAT_ARRAY))
		{
			delete _value.float_array.ptr;
			_value.float_array.ptr = nullptr;
			_value.float_array.count = 0;
		}

		if (count > 0)
		{
			_value.float_array.ptr = (float*)malloc(sizeof(float) * count);
			_value.float_array.count = count;
			memcpy(_value.float_array.ptr, ptr, sizeof(float)* count);
		}
	}

	void Uniform::set_matrix(float* ptr)
	{
		if (!check_and_set_type(ValueType::MATRIX4))
		{
			delete _value.float_array.ptr;
			_value.float_array.ptr = nullptr;
			_value.float_array.count = 0;
		}

		_value.float_array.ptr = (float*)malloc(sizeof(float) * 16);
		_value.float_array.count = 1;
		memcpy(_value.float_array.ptr, ptr, sizeof(float) * 16);
	}

	void Uniform::set_value(int a)
	{
		check_and_set_type(ValueType::INT);
		_value.int1 = a;
	}

	void Uniform::set_value(int a, int b)
	{
		check_and_set_type(ValueType::INT2);
		_value.int2.a = a;
		_value.int2.b = b;
	}

	void Uniform::set_value(int a, int b, int c)
	{
		check_and_set_type(ValueType::INT3);
		_value.int3.a = a;
		_value.int3.b = b;
		_value.int3.c = c;
	}

	void Uniform::set_value(int a, int b, int c, int d)
	{
		check_and_set_type(ValueType::INT4);
		_value.int4.a = a;
		_value.int4.b = b;
		_value.int4.c = c;
		_value.int4.d = d;
	}

	void Uniform::set_value(int* ptr, int count)
	{
		if (!check_and_set_type(ValueType::INT_ARRAY))
		{
			delete _value.int_array.ptr;
			_value.int_array.ptr = nullptr;
			_value.int_array.count = 0;
		}

		if (count > 0)
		{
			_value.int_array.ptr = (int*)malloc(sizeof(int)* count);
			_value.int_array.count = count;
			memcpy(_value.int_array.ptr, ptr, sizeof(int)* count);
		}
	}

	void Uniform::set_value(unsigned int id, bool mipmap, TextureWrap s, TextureWrap t, TextureMinFilter min, TextureMagFilter mag, unsigned int mask)
	{
		check_and_set_type(ValueType::SAMPLER2D);

		_value.sampler_2d.texture.id = id;
		_value.sampler_2d.mipmap = mipmap;
		_value.sampler_2d.s = s;
		_value.sampler_2d.t = t;
		_value.sampler_2d.min = min;
		_value.sampler_2d.mag = mag;
		_value.sampler_2d.mask = mask;
	}

	void Uniform::merge(const Uniform& rhs)
	{
		if (_type == ValueType::NONE)
		{
			switch (rhs._type)
			{
			case ValueType::INNER: _value.usage = rhs._value.usage; break;
			case ValueType::FLOAT: _value.float1 = rhs._value.float1; break;
			case ValueType::FLOAT2: _value.float2 = rhs._value.float2; break;
			case ValueType::FLOAT3: _value.float3 = rhs._value.float3; break;
			case ValueType::FLOAT4: _value.float4 = rhs._value.float4; break;
			case ValueType::FLOAT_ARRAY: 
				{
					_value.float_array.ptr = (float*)malloc(sizeof(float)* rhs._value.float_array.count);
					_value.float_array.count = rhs._value.float_array.count;
					memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* rhs._value.float_array.count);
				} break;
			case ValueType::FLOAT2_ARRAY:
				{
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 2 * rhs._value.float_array.count);
					_value.float_array.count = rhs._value.float_array.count;
					memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 2 * rhs._value.float_array.count);
				} break;
			case ValueType::FLOAT3_ARRAY:
				{
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 3 * rhs._value.float_array.count);
					_value.float_array.count = rhs._value.float_array.count;
					memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 3 * rhs._value.float_array.count);
				} break;
			case ValueType::FLOAT4_ARRAY:
				{
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 4 * rhs._value.float_array.count);
					_value.float_array.count = rhs._value.float_array.count;
					memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 4 * rhs._value.float_array.count);
				} break;
			case ValueType::INT: _value.int1 = rhs._value.int1; break;
			case ValueType::INT2: _value.int2 = rhs._value.int2; break;
			case ValueType::INT3: _value.int3 = rhs._value.int3; break;
			case ValueType::INT4: _value.int4 = rhs._value.int4; break;
			case ValueType::INT_ARRAY: 
				{
					_value.int_array.ptr = (int*)malloc(sizeof(int)* rhs._value.int_array.count);
					_value.int_array.count = rhs._value.int_array.count;
					memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* rhs._value.int_array.count);
				} break;
			case ValueType::INT2_ARRAY: 
				{
					_value.int_array.ptr = (int*)malloc(sizeof(int)* 2 * rhs._value.int_array.count);
					_value.int_array.count = rhs._value.int_array.count;
					memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 2 * rhs._value.int_array.count);
				} break;
			case ValueType::INT3_ARRAY: 
				{
					_value.int_array.ptr = (int*)malloc(sizeof(int)* 3 * rhs._value.int_array.count);
					_value.int_array.count = rhs._value.int_array.count;
					memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 3 * rhs._value.int_array.count);
				} break;
			case ValueType::INT4_ARRAY: 
				{
					_value.int_array.ptr = (int*)malloc(sizeof(int)* 4 * rhs._value.int_array.count);
					_value.int_array.count = rhs._value.int_array.count;
					memcpy(_value.int_array.ptr, rhs._value.int_array.ptr, sizeof(int)* 4 * rhs._value.int_array.count);
				} break;
			case ValueType::MATRIX2:
				{
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 4 * rhs._value.float_array.count);
					_value.float_array.count = rhs._value.float_array.count;
					memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 4 * rhs._value.float_array.count);
				} break;
			case ValueType::MATRIX3:
				{
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 9 * rhs._value.float_array.count);
					_value.float_array.count = rhs._value.float_array.count;
					memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 9 * rhs._value.float_array.count);
				} break;
			case ValueType::MATRIX4:
				{
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 16 * rhs._value.float_array.count);
					_value.float_array.count = rhs._value.float_array.count;
					memcpy(_value.float_array.ptr, rhs._value.float_array.ptr, sizeof(float)* 16 * rhs._value.float_array.count);
				} break;
			case ValueType::SAMPLER2D: _value.sampler_2d = rhs._value.sampler_2d; break;
			default: break;
			}

			_type = rhs._type;
		}
		else if (_type == rhs._type && _type == ValueType::SAMPLER2D)
		{
			if (_value.sampler_2d.texture.id == 0)
				_value.sampler_2d.texture.id = rhs._value.sampler_2d.texture.id;

			if (_value.sampler_2d.texture.id == rhs._value.sampler_2d.texture.id)
			{
				MERGE_RENDER_STATE(MIPMAP, _value.sampler_2d.mask, _value.sampler_2d.mipmap);
				MERGE_RENDER_STATE(WARP_S, _value.sampler_2d.mask, _value.sampler_2d.s);
				MERGE_RENDER_STATE(WARP_T, _value.sampler_2d.mask, _value.sampler_2d.t);
				MERGE_RENDER_STATE(MIN_FILTER, _value.sampler_2d.mask, _value.sampler_2d.min);
				MERGE_RENDER_STATE(MAG_FILTER, _value.sampler_2d.mask, _value.sampler_2d.mag);
			}
		}
	}

	void Uniform::load_inner()
	{
		if (_type == ValueType::INNER)
		{
			switch (_value.usage)
			{
			case Usage::MODEL_MATRIX:
				{
					const glm::mat4& matrix = System::instance().get_model_matrix();

					_type = ValueType::MATRIX4;
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 16 * 1);
					_value.float_array.count = 1;
					memcpy(_value.float_array.ptr, &matrix[0][0], sizeof(float)* 16);
				}
				break;
			case Usage::VIEW_MATRIX:
				{
					const glm::mat4& matrix = System::instance().get_view_matrix();

					_type = ValueType::MATRIX4;
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 16 * 1);
					_value.float_array.count = 1;
					memcpy(_value.float_array.ptr, &matrix[0][0], sizeof(float)* 16);
				}
				break;
			case Usage::PROJECTION_MATRIX:
				{
					const glm::mat4& matrix = System::instance().get_projection_matrix();

					_type = ValueType::MATRIX4;
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 16 * 1);
					_value.float_array.count = 1;
					memcpy(_value.float_array.ptr, &matrix[0][0], sizeof(float)* 16);
				}
				break;
			case Usage::VIEW_PROJECTION_MATRIX: break;
			case Usage::WORLD_VIEW_PROJECTION_MATRIX:
				{
					const glm::mat4& matrix = System::instance().get_mvp_matrix();

					_type = ValueType::MATRIX4;
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 16 * 1);
					_value.float_array.count = 1;
					memcpy(_value.float_array.ptr, &matrix[0][0], sizeof(float)* 16);

				}
				break;
			case Usage::INVERSE_TRANSPOSE_WORLD_MATRIX: break;
			case Usage::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX: break;
			case Usage::CAMERA_WORLD_POSITION: break;
			case Usage::CAMERA_VIEW_POSITION: break;
			case Usage::MATRIX_PALETTE:
				{
					const auto& matrixs = System::instance().get_matrix_palette();

					_type = ValueType::MATRIX4;
					_value.float_array.count = matrixs.size();
					_value.float_array.ptr = (float*)malloc(sizeof(float)* 16 * _value.float_array.count);
					int offset = 0;
					for (const auto& matrix : matrixs)
					{
						memcpy((_value.float_array.ptr + offset), &matrix[0][0], sizeof(float)* 16);
						offset += 16;
					}
					
				}
				break;
			}
		}
		else if (_type == ValueType::SAMPLER2D)
		{
			_value.sampler_2d.texture.ptr = System::instance().get_resource_manager().get_texture_2d(_value.sampler_2d.texture.id);
		}
	}

	bool Uniform::check_and_set_type(ValueType type)
	{
		if (_type != ValueType::NONE)
		{
			if (_type != type)
			{
				throw std::logic_error("can not set used uniform for another type");
			}
			return false;
		}
		else
		{
			_type = type;
			return true;
		}
	}

}

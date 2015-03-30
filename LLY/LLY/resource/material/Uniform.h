#ifndef UNIFORM_H_
#define UNIFORM_H_
#include <string>
#include "../../util/LLYIncludes.h"

namespace lly {
	class Device;
	class Texture2D;

	class Uniform
	{
		enum class ValueType
		{
			NONE,
			INNER,
			FLOAT,
			FLOAT2,
			FLOAT3,
			FLOAT4,
			FLOAT_ARRAY,
			FLOAT2_ARRAY,
			FLOAT3_ARRAY,
			FLOAT4_ARRAY,
			INT,
			INT2,
			INT3,
			INT4,
			INT_ARRAY,
			INT2_ARRAY,
			INT3_ARRAY,
			INT4_ARRAY,
			MATRIX2,
			MATRIX3,
			MATRIX4,
			SAMPLER2D,
			SAMPLER3D,
			SAMPLERCUBE,
		};

	public:
		enum Mask
		{
			MIPMAP = 0x01,
			WARP_S = 0x02,
			WARP_T = 0x04,
			WARP_R = 0x08,
			MIN_FILTER = 0x10,
			MAG_FILTER = 0x20,
		};

		enum class Usage
		{
			MODEL_MATRIX,
			VIEW_MATRIX,
			PROJECTION_MATRIX,
			VIEW_PROJECTION_MATRIX,
			WORLD_VIEW_PROJECTION_MATRIX,
			INVERSE_TRANSPOSE_WORLD_MATRIX,
			INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,
			CAMERA_WORLD_POSITION,
			CAMERA_VIEW_POSITION,
			MATRIX_PALETTE
		};

		Uniform();
		Uniform(const Uniform& rhs);
		Uniform(Uniform&& rhs);
		~Uniform();

		Uniform& operator=(const Uniform& rhs);
		Uniform& operator=(Uniform&& rhs);

		void use(Device& device);

		void set_name(const std::string& name) { _name = name; }
		const std::string& get_name() const { return _name; }

		void set_value(Usage usage);
		void set_value(float a);
		void set_value(float a, float b);
		void set_value(float a, float b, float c);
		void set_value(float a, float b, float c, float d);
		void set_value(float* ptr, int count);

		void set_matrix(float* ptr);

		void set_value(int a);
		void set_value(int a, int b);
		void set_value(int a, int b, int c);
		void set_value(int a, int b, int c, int d);
		void set_value(int* ptr, int count);

		void set_value(unsigned int id, bool mipmap, TextureWrap s, TextureWrap t, TextureMinFilter min, TextureMagFilter mag, unsigned int mask);

		void merge(const Uniform& rhs);

		void load_inner();

	private:
		bool check_and_set_type(ValueType type);

	private:
		std::string _name;
		ValueType _type;
		union {
			Usage usage;

			float float1;

			struct {
				float a;
				float b;
			} float2;

			struct {
				float a;
				float b;
				float c;
			} float3;

			struct {
				float a;
				float b;
				float c;
				float d;
			} float4;

			struct {
				float * ptr;
				int count;
			} float_array;
			
			int int1;

			struct {
				int a;
				int b;
			} int2;

			struct {
				int a;
				int b;
				int c;
			} int3;

			struct {
				int a;
				int b;
				int c;
				int d;
			} int4;

			struct {
				int * ptr;
				int count;
			} int_array;

			struct {
				union {
					unsigned int id;
					Texture2D* ptr;
				} texture;

				bool mipmap;
				TextureWrap s;
				TextureWrap t;
				TextureMinFilter min;
				TextureMagFilter mag;
				unsigned int mask;
			}sampler_2d;
		} _value;
	};

}

#endif //UNIFORM_H_


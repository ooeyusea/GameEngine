#include "VertexDescription.h"
#include "../util/LLYIncludes.h"
#include "../util/FuncUnitl.h"

namespace lly {

	void VertexDescription::bind() const
	{
		for (auto& desc : _descriptions)
		{
			GLuint index = lly_util::to_position(desc.position);
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, desc.size, lly_util::to_data_type(desc.type), desc.normalized, desc.stride, (void*)desc.offset);
		}
	}

	void VertexDescription::add(VertexAttribPosition position, unsigned int size, DataType type, bool normalized, int stride, int offset)
	{
		_descriptions.push_back({ position, size, type, normalized, stride, offset });
	}
}

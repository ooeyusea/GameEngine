#include "VertexBuffer.h"
#include "VertexDescription.h"
#include "../util/FuncUnitl.h"

namespace lly {

	VertexBuffer::VertexBuffer()
		: _vbo(0)
	{
	}


	VertexBuffer::~VertexBuffer()
	{
		if (_vbo)
		{
			glDeleteBuffers(1, &_vbo);
			_vbo = 0;
		}
	}

	bool VertexBuffer::load_data(const char * data, int len, VertexUsage usage)
	{
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, len, data, lly_util::to_usage(usage));

		return true;
	}

}


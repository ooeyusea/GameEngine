#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexDescription.h"
#include "../util/FuncUnitl.h"

namespace lly {
	IndexBuffer::IndexBuffer()
		: _vao(0)
		, _vio(0)
		, _drawType(DrawType::POINTS)
		, _count(0)
	{
	}


	IndexBuffer::~IndexBuffer()
	{
		if (_vio)
		{
			glDeleteBuffers(1, &_vio);
			_vio = 0;
		}

		if (_vao)
		{
			glDeleteVertexArrays(1, &_vao);
			_vao = 0;
		}
	}

	bool IndexBuffer::create(int size)
	{
		glGenBuffers(1, &_vio);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vio);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

		return true;
	}

	bool IndexBuffer::load_indices(const char * data, int len)
	{
		glGenBuffers(1, &_vio);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vio);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, data, GL_STATIC_DRAW);

		return true;
	}

	void IndexBuffer::update(const char * data, int len, int count)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vio);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, len, data);
		_count = count;
	}

	bool IndexBuffer::gen(VertexBuffer * vertex_buffer, const VertexDescription& description)
	{
		_drawType = description.get_draw_type();
		_count = description.get_size();

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->_vbo);
		description.bind();
		if (_vio)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vio);
		}
		glBindVertexArray(0);

		return true;
	}

	void IndexBuffer::draw(Device& device)
	{
		glBindVertexArray(_vao);
		glDrawElements(lly_util::to_draw_mode(_drawType), _count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

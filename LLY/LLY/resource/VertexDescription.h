#ifndef VERTEXDESCRIPTION_H_
#define VERTEXDESCRIPTION_H_
#include "../util/LLYIncludes.h"
#include <vector>

namespace lly {

	class VertexDescription
	{
		struct Description
		{
			VertexAttribPosition position;
			unsigned int size;
			DataType type;
			bool normalized;
			int stride;
			int offset;
		};

	public:
		VertexDescription(DrawType mode, int size) : _mode(mode), _size(size) {}
		virtual ~VertexDescription() {}

		virtual void bind() const;

		DrawType get_draw_type() const { return _mode; }
		int get_size() const { return _size; }

		void add(VertexAttribPosition position, unsigned int size, DataType type, bool normalized, int stride, int offset);

	private:
		DrawType _mode;
		int _size;

		std::vector<Description> _descriptions;
	};
}

#endif //VERTEXDESCRIPTION_H_


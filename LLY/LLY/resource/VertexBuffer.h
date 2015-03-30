#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_
#include "../util/LLYIncludes.h"

namespace lly {

	class VertexDescription;
	class Device;
	class IndexBuffer;

	class VertexBuffer
	{
		friend class IndexBuffer;
	public:
		VertexBuffer();
		~VertexBuffer();

		bool load_data(const char * data, int len, VertexUsage usage);

	private:
		GLuint _vbo;
	};
}

#endif //VERTEXBUFFER_H_


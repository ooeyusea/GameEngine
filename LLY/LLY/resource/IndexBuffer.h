#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_
#include "../util/LLYIncludes.h"

namespace lly {

	class VertexDescription;
	class Device;
	class VertexBuffer;

	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		bool create(int size);
		bool load_indices(const char * data, int len);
		void update(const char * data, int len, int count);
		bool gen(VertexBuffer * vertex_buffer, const VertexDescription& description);
		
		void draw(Device& device);

	private:
		GLuint _vao;
		GLuint _vio;

		DrawType _drawType;
		int _count;
	};
}

#endif //INDEXBUFFER_H_


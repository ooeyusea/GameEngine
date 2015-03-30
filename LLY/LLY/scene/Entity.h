#ifndef ENTITY_H_
#define ENTITY_H_
#include "node.h"

namespace lly {
	class Mesh;
	class Skeleton;

	class Entity : public Node
	{
	public:
		Entity();
		~Entity();

		void add_mesh(Mesh * mesh);
		void draw();
		void update(float elapse);

		void play_animation(const std::string& animation, bool loop);

	private:
		Mesh * _mesh;
		Skeleton * _skeleton;
	};
}

#endif //ENTITY_H_

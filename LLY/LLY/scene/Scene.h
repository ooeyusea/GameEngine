#ifndef SCENE_H_
#define SCENE_H_
#include <set>
#include <vector>

namespace lly {
	class Node;
	class Camera;
	class Light;
	class RenderTarget;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		bool init();
		void release();

		void update(float elapse);

		void render();

		void add_node(Node * node) { _roots.insert(node); }
		void add_node(Light * light) { _lights.insert(light); }
		void add_node(Camera * camera);

	private:
		void __render(Camera * camera);
		void __render(Light * light);

		std::set<Node *> _roots;
		std::set<Light*> _lights;
		std::vector<Camera*> _cameras;
	};
}

#endif //SCENE_H_
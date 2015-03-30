#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

namespace lly {

	class GameObject;
	class Camera;

	class Scene
	{
	public:
		static Scene* create();

		Scene();
		virtual ~Scene();

		virtual void onEnter();
		virtual void onExit();

		virtual void run();
		virtual void updateBefore();
		virtual void update();
		virtual void updateAfter();

		void addChild(GameObject* child);
		void removeChild(GameObject* child);

		void addCamera(Camera* camera);
		void setCurCamera(unsigned int curCameraIndex);
		Camera* getCurCamera();

	protected:

	private:
		GameObject* _root;

		unsigned int _curCameraIndex;
		std::vector<Camera*> _cameras;
	};
}

#endif //SCENE_H_
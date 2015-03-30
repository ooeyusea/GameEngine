#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

namespace lly {
	
	class Scene;

	class SceneManager
	{
	public:
		static SceneManager* getInstance();

		SceneManager();
		~SceneManager();

		void replaceScene(Scene* scene);
		
		void run();

	private:
		Scene* _scene;
	};
}

#endif
#ifndef _SCENELOADER_H_
#define _SCENELOADER_H_
#include <string>

namespace lly {
	class Scene;
}

namespace loader {
	class SceneLoader
	{
	public:
		SceneLoader();
		~SceneLoader();

		lly::Scene * load_from(const std::string& file);
	};
}

#endif //_SCENELOADER_H_
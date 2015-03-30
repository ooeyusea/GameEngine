#ifndef MATERIALLOADER_H_
#define MATERIALLOADER_H_
#include <vector>

namespace lly
{
	class Material;

	namespace material_loader
	{
		Material* load_material(const std::string& filename);
	}
}

#endif //MATERIALLOADER_H_

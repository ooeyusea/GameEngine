#include "MeshPart.h"
#include "../scene/skeleton.h"
#include "../scene/bone.h"
#include "IndexBuffer.h"

namespace lly {

	MeshPart::MeshPart()
		: _ib(nullptr)
	{
	}


	MeshPart::~MeshPart()
	{
		if (_ib != nullptr)
		{
			delete _ib;
			_ib = nullptr;
		}
	}

	std::vector<glm::mat4> MeshPart::get_matrix_palette(Skeleton * skeleton)
	{
		std::vector<glm::mat4> ret;
		for (auto& bone_info : _bones)
		{
			auto bone = skeleton->get_bone(bone_info.name);
			if (bone != nullptr)
			{
				ret.push_back(bone->get_transform() * bone_info.inverse_bindpose);
			}
			else
				ret.push_back(glm::mat4(1.0f));
		}

		return std::move(ret);
	}
}

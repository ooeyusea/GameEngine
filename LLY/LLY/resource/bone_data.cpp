#include "bone_data.h"

namespace lly {
	BoneData::BoneData()
        : _creator(nullptr)
		, _parent(nullptr)
		, _scale(1.0f)
    {

    }

	BoneData::~BoneData()
    {

    }

	void BoneData::add_child(BoneData * node)
	{
		_children.push_back(node);
	}

	BoneData * BoneData::get_child_by_name(const std::string& name)
	{
		for (auto child : _children)
		{
			if (child->get_name() == name)
			{
				return child;
			}
		}
		return nullptr;
	}

	void BoneData::visit_child(std::function<void(BoneData*)> f)
	{
		for (auto child : _children)
		{
			f(child);
		}
	}
}

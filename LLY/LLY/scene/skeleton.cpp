#include "skeleton.h"
#include "bone.h"
#include "../resource/skeleton_data.h"

namespace lly {

	Skeleton::Skeleton(SkeletonData * data)
        : _data(data)
        , _root(nullptr)
    {
    }

    Skeleton::~Skeleton()
    {

    }

	Bone * Skeleton::create_bone(BoneData * data)
    {
		Bone * bone = new Bone(data);
        bone->set_creator(this);

        _bones.push_back(bone);
		_bones_by_name[bone->get_name()] = bone;

        return bone;
    }

    Bone * Skeleton::get_bone(const std::string& name)
    {
        auto itr = _bones_by_name.find(name);
        if (itr != _bones_by_name.end())
        {
            return itr->second;
        }
        return nullptr;
    }

	void Skeleton::play_animation(const std::string& animation, bool loop)
	{
		auto& animation_state = _state_set[animation];
		animation_state.set_loop(loop);
		animation_state.set_animation(animation);
	}

	void Skeleton::update(float elapse)
	{
		for (auto itr = _state_set.begin(); itr != _state_set.end(); ++itr)
		{
			itr->second.update(elapse);
			itr->second.apply_skeleton(this);
		}
	}
}

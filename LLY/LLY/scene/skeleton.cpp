#include "skeleton.h"
#include "bone.h"
#include "../resource/skeleton_data.h"
#include <algorithm>
#include "AnimationState.h"

namespace lly {

	Skeleton::Skeleton(SkeletonData * data)
        : _data(data)
        , _root(nullptr)
    {
		_data->init_animation_state(_state_set);
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

	void Skeleton::set_animation(const std::string& animation, bool loop, float weight)
	{
		auto itr = _state_set.find(animation);
		if (itr == _state_set.end())
			return;

		auto enable_itr = std::find(_enable_states.begin(), _enable_states.end(), &itr->second);
		if (enable_itr == _enable_states.end())
		{
			itr->second.set_loop(loop);
			itr->second.set_loop(weight);
			_enable_states.push_back(&itr->second);
		}
	}

	void Skeleton::stop_animation(const std::string& animation)
	{
		auto itr = _state_set.find(animation);
		if (itr == _state_set.end())
			return;

		auto enable_itr = std::find(_enable_states.begin(), _enable_states.end(), &itr->second);
		if (enable_itr != _enable_states.end())
		{
			_enable_states.erase(enable_itr);
		}
	}

	void Skeleton::update(float elapse)
	{
		auto itr = _enable_states.begin();
		while (itr != _enable_states.end())
		{
			(*itr)->update(elapse);
			(*itr)->apply_skeleton(this);

			if (!(*itr)->is_loop() && (*itr)->is_end())
				itr = _enable_states.erase(itr);
			else
				++itr;
		}
	}
}

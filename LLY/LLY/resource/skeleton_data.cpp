#include "skeleton_data.h"
#include "bone_data.h"
#include "animation.h"
#include "../scene/skeleton.h"
#include "../scene/bone.h"

namespace lly {

    SkeletonData::SkeletonData()
        : _root(nullptr)
    {

    }

    SkeletonData::~SkeletonData()
    {
        if (_root != nullptr)
        {
            delete _root;
            _root = nullptr;
        }
    }

    BoneData * SkeletonData::create_bone(const std::string& name)
    {
		BoneData * bone = new BoneData;
        bone->set_creator(this);
        bone->set_name(name);

        _bones.push_back(bone);
        _bones_by_name[name] = bone;

        return bone;
    }

	BoneData * SkeletonData::get_bone(const std::string& name)
    {
        auto itr = _bones_by_name.find(name);
        if (itr != _bones_by_name.end())
        {
            return itr->second;
        }
        return nullptr;
    }

    Animation * SkeletonData::create_animation(const std::string& name)
    {
        Animation * animation = new Animation();
		animation->set_name(name);
        _animations[name] = animation;
        return animation;
    }

    Animation * SkeletonData::get_animation(const std::string& name)
    {
        auto itr = _animations.find(name);
        if (itr != _animations.end())
        {
            return itr->second;
        }
        return nullptr;
    }

    Skeleton * SkeletonData::create_skeleton()
    {
		Skeleton * ret = new Skeleton(this);

		for (auto bone : _bones)
			ret->create_bone(bone);

		for (auto bone : _bones)
		{
			Bone * parent = ret->get_bone(bone->get_name());
			parent->set_position(bone->get_position());
			parent->set_rotation(bone->get_rotation());
			parent->set_scale(bone->get_scale());
			bone->visit_child([&ret, &parent](BoneData * data) {
				Bone * child = ret->get_bone(data->get_name());
				parent->add_child(child);
				child->set_parent(parent);
			});
		}

		ret->set_root_bone(ret->get_bone(_root->get_name()));

		return ret;
    }
}

#ifndef SKELETON_DATA_H_
#define SKELETON_DATA_H_
#include <vector>
#include <string>
#include <unordered_map>

namespace lly {
	class Skeleton;
    class BoneData;
    class Animation;
	class AnimationState;
    class SkeletonData
    {
    public:
        SkeletonData();
        ~SkeletonData();

		BoneData * create_bone(const std::string& name);
		BoneData * get_bone(const std::string& name);

        Animation * create_animation(const std::string& name);
        Animation * get_animation(const std::string& name);

		void set_root_bone(BoneData * root) { _root = root; }
		BoneData* get_root_bone() const { return _root; }

        Skeleton * create_skeleton();

		void init_animation_state(std::unordered_map<std::string, lly::AnimationState>& state_set);

    private:
        std::string _name;
		std::vector<BoneData*> _bones;
		std::unordered_map<std::string, BoneData*> _bones_by_name;
		BoneData * _root;

        std::unordered_map<std::string, Animation*> _animations;
    };
}

#endif //SKELETON_DATA_H_

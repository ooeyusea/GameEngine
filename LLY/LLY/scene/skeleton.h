#ifndef SKELETON_H_
#define SKELETON_H_
#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/fwd.hpp>
#include "AnimationState.h"
#include <list>

namespace lly {

	class SkeletonData;
	class BoneData;
    class Bone;
	class AnimationState;

    class Skeleton
    {
    public:
		Skeleton(SkeletonData * data);
        ~Skeleton();

		Bone * create_bone(lly::BoneData * data);
        Bone * get_bone(const std::string& name);

		void set_root_bone(Bone * root) { _root = root; }
		Bone* get_root_bone() const { return _root; }

		SkeletonData * get_data() const { return _data; }

		void set_animation(const std::string& animation, bool loop, float weight);
		void stop_animation(const std::string& animation);

		void update(float elapse);

    private:
		SkeletonData * _data;

        std::string _name;
        std::vector<Bone*> _bones;
        std::unordered_map<std::string, Bone*> _bones_by_name;
        Bone * _root;
		std::unordered_map<std::string, AnimationState> _state_set;
		std::list<AnimationState*> _enable_states;
    };
}

#endif // SKELETON_H_

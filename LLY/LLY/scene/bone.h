#ifndef BONE_H_
#define BONE_H_
#include "node.h"
#include <vector>

namespace lly {
    class Skeleton;
	class BoneData;

    class Bone : public Node
    {
    public:
		Bone(BoneData * data);
        virtual ~Bone();

        void set_creator(Skeleton * creator) { _creator = creator; }
        Skeleton * get_creator() const { return _creator; }

        void reset();

    private:
        Skeleton * _creator;
		BoneData * _data;
    };
}
#endif //BONE_H_


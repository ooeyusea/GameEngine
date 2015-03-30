#include "bone.h"
#include "../resource/bone_data.h"

namespace lly {
	Bone::Bone(BoneData * data)
        : _creator(nullptr)
		, _data(data)
    {
		_name = data->get_name();
    }

    Bone::~Bone()
    {

    }

    void Bone::reset()
    {
		_position = _data->get_position();
		_rotation = _data->get_rotation();
        _scale = _data->get_scale();
    }
}

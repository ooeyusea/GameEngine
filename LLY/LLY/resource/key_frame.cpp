#include "key_frame.h"
#include "../scene/skeleton.h"
#include "../scene/bone.h"
#include "track.h"

namespace lly {

    void TransformFrame::apply(Skeleton* skeleton, Track * track, float weight)
    {
        NodeTrack * node_track = static_cast<NodeTrack *>(track);
        Bone * bone = skeleton->get_bone(node_track->get_target());
        if (bone != nullptr)
        {
            bone->set_position(_position);
            bone->set_rotation(_rotation);
            bone->set_scale(_scale);
        }
    }

    void TransformFrame::interpolate_from(KeyFrame * a, KeyFrame * b)
    {
        TransformFrame * transform_a = static_cast<TransformFrame *>(a);
        TransformFrame * transform_b = static_cast<TransformFrame *>(b);

        if (a == b)
        {
            _position = transform_a->_position;
            _rotation = transform_a->_rotation;
            _scale = transform_a->_scale;
        }
        else
        {
            float interpolate_rate = (_time_pos - transform_a->_time_pos) / (transform_b->_time_pos - transform_a->_time_pos);

            _position = transform_a->_position + (transform_b->_position - transform_a->_position) * interpolate_rate;
            _rotation = glm::slerp(transform_a->_rotation, transform_b->_rotation, interpolate_rate);
            _scale = transform_a->_scale + (transform_b->_scale - transform_a->_scale) * interpolate_rate;
        }
    }
}

#include "track.h"
#include "key_frame.h"

namespace lly {

    void Track::get_interpolate_frame(KeyFrame& frame)
    {
        if (_frames.empty())
            throw std::logic_error("empty frame");

        size_t i = 0;
        while (i < _frames.size())
        {
            if (frame.get_time_pos() < _frames[i]->get_time_pos())
            {
                break;
            }
            ++i;
        }

        if (i == _frames.size())
        {
            frame.interpolate_from(_frames[i - 1], _frames[i - 1]);
        }
        else if (i == 0)
        {
            frame.interpolate_from(_frames[i], _frames[i]);
        }
        else
            frame.interpolate_from(_frames[i - 1], _frames[i]);
    }

    void NodeTrack::apply(Skeleton* skeleton, float time_pos, float weight)
    {
        TransformFrame frame(time_pos);
		get_interpolate_frame(frame);

        frame.apply(skeleton, this, weight);
    }
}

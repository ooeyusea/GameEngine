#include "animation.h"
#include "track.h"

namespace lly {

    void Animation::apply(Skeleton* skeleton, float time_pos, float weight)
    {
        for (auto track : _tracks)
        {
            track->apply(skeleton, time_pos, weight);
        }
    }
}

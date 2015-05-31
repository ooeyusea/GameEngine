#include "AnimationState.h"
#include "../resource/skeleton_data.h"
#include "skeleton.h"
#include "../resource/animation.h"

namespace lly {

	AnimationState::AnimationState()
		: _current_time(0.0f)
		, _time_scale(1.0f)
		, _length(0.0f)
		, _weight(1.0f)
		, _loop(false)
	{
	}

	AnimationState::~AnimationState()
	{
	}

	void AnimationState::update(float elapse)
	{
		_current_time += elapse * _time_scale;
		if (_loop && _length > 0.0f)
		{
			_current_time = _current_time - (int)(_current_time / _length) * _length;
		}
	}

	void AnimationState::apply_skeleton(Skeleton * skeleton)
	{
		auto animation = skeleton->get_data()->get_animation(_animation);
		animation->apply(skeleton, _current_time, _weight);
	}
}

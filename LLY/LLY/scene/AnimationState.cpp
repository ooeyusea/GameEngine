#include "AnimationState.h"
#include "../resource/skeleton_data.h"
#include "skeleton.h"
#include "../resource/animation.h"

namespace lly {

	AnimationState::AnimationState()
		: _current_time(0.0f)
		, _time_scale(1.0f)
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
	}

	void AnimationState::apply_skeleton(Skeleton * skeleton)
	{
		float time_pos = _current_time;

		auto animation = skeleton->get_data()->get_animation(_animation);
		if (_loop)
		{
			time_pos = _current_time - (int)(_current_time / animation->get_length()) * animation->get_length();
		}

		animation->apply(skeleton, time_pos, _weight);
	}
}

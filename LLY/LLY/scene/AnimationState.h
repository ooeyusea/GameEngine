#ifndef ANIMATION_STATE_H_
#define ANIMATION_STATE_H_
#include <string>

namespace lly {
	class Skeleton;

	class AnimationState
	{
	public:
		AnimationState();
		~AnimationState();

		void set_animation(const std::string& animation) { _animation = animation; }
		const std::string& get_animation() const { return _animation; }

		void set_time_scale(float scale) { _time_scale = scale; }
		float get_time_scale() const { return _time_scale; }

		void set_length(float length) { _length = length; }
		float get_length() const { return _length; }

		void set_weight(float weight) { _weight = weight; }
		float get_weight() const { return _weight; }

		void set_loop(bool loop) { _loop = loop; }
		bool is_loop() const { return _loop; }

		bool is_end() const { return _current_time >= _length; }

		void update(float elapse);

		void apply_skeleton(Skeleton * skeleton);

	private:
		std::string _animation;
		float _current_time;
		float _time_scale;
		float _length;
		float _weight;
		bool _loop;
	};

}
#endif //ANIMATIONSTATE_H_

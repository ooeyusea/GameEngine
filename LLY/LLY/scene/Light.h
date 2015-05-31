#ifndef LIGHT_H_
#define LIGHT_H_
#include "node.h"

namespace lly {

	class Light : public Node
	{
	public:
		enum class Type
		{
			AMBIENT,
			DIRECTIONAL,
			POINT,
			SPOT,
		};

		Light();
		~Light();

		void set_type(Type val) { _type = val; }

		void set_color(const glm::vec4& color) { _color = color; }
		glm::vec4 get_color() const { return _color; }

		void set_direction(const glm::vec3& direction) { _direction = direction; }
		glm::vec3 get_direction() const { return _direction; }

		void set_constant(float val) { _constant = val; }
		float get_constant() const { return _constant; }

		void set_linear(float val) { _linear = val; }
		float get_linear() const { return _linear; }

		void set_quadratic(float val) { _quadratic = val; }
		float get_quadratic() const { return _quadratic; }

		void set_exp(float val) { _exp = val; }
		float get_exp() const { return _exp; }

		void set_cutoff(float val) { _cutoff = val; }
		float get_cutoff() const { return _cutoff; }

		virtual void draw_light();

		virtual void add_to_scene(lly::Scene * scene) override;

	private:
		Type _type;
		glm::vec4 _color;
		glm::vec3 _direction;
		float _constant;
		float _linear;
		float _quadratic;
		float _exp;
		float _cutoff;
	};
}
#endif //LIGHT_H_


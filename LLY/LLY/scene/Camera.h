#ifndef CAMERA_H_
#define CAMERA_H_
#include "node.h"

namespace lly {
	class RenderTarget;

	class Camera : public Node
	{
	public:
		enum class Type
		{
			PERSPECTIVE = 1,
			ORTHOGRAPHIC = 2
		};

		Camera();
		virtual ~Camera();

		Camera::Type get_type() const { return _type; }
		void set_type(Camera::Type type) { _type = type; }

		void set_prespective(float fovy, float aspect, float near, float far);
		void set_ortho(float left, float right, float top, float bottom, float near, float far);

		float get_fovy() const { return _data.prespective.fovy; }
		void set_fovy(float val) { _data.prespective.fovy = val; update_projection(); }

		float get_aspect() const { return _data.prespective.aspect; }
		void set_aspect(float val) { _data.prespective.aspect = val; update_projection(); }

		float get_left() const { return _data.ortho.left; }
		void set_left(float val) { _data.ortho.left = val; update_projection(); }

		float get_right() const { return _data.ortho.right; }
		void set_right(float val) { _data.ortho.right = val; update_projection(); }

		float get_top() const { return _data.ortho.top; }
		void set_top(float val) { _data.ortho.top = val; update_projection(); }

		float get_bottom() const { return _data.ortho.bottom; }
		void set_bottom(float val) { _data.ortho.bottom = val; update_projection(); }

		float get_near() const { return _near; }
		void set_near(float val) { _near = val; update_projection(); }

		float get_far() const { return _far; }
		void set_far(float val) { _far = val; update_projection(); }

		void set_target(RenderTarget * target) { _target = target; }

		void set_order(int val) { _order = val; }
		int get_order() const { return _order; }

		static bool order(Camera * a, Camera * b);

		void update_projection();

		virtual void add_to_scene(lly::Scene * scene) override;

		void begin_render();

	private:
		Camera::Type _type;
		union
		{
			struct
			{
				float left;
				float right;
				float top;
				float bottom;
			} ortho;

			struct
			{
				float fovy;
				float aspect;
			} prespective;
		} _data;

		float _near;
		float _far;
		int _order;

		glm::mat4 _projection;

		RenderTarget * _target;
		int _clear_flags;
	};
}

#endif
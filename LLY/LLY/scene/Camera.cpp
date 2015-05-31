#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../util/LLYIncludes.h"
#include "../System.h"
#include "Scene.h"

namespace lly {

	Camera::Camera()
		: _near(0.0f)
		, _far(0.0f)
		, _order(0)
		, _target(nullptr)
		, _clear_flags(ClearTarget::COLOR | ClearTarget::DEPTH | ClearTarget::STENCIL)
	{
		memset(&_data, 0, sizeof(_data));
	}

	Camera::~Camera()
	{
	}

	void Camera::set_prespective(float fovy, float aspect, float n, float f)
	{
		_type = Type::PERSPECTIVE;
		_data.prespective.aspect = aspect;
		_data.prespective.fovy = fovy;
		_near = n;
		_far = f;

		update_projection();
	}

	void Camera::set_ortho(float left, float right, float top, float bottom, float n, float f)
	{
		_type = Type::ORTHOGRAPHIC;
		_data.ortho.left = left;
		_data.ortho.right = right;
		_data.ortho.top = top;
		_data.ortho.bottom = right;
		_near = n;
		_far = f;

		update_projection();
	}

	bool Camera::order(Camera * a, Camera * b)
	{
		return a->order < b->order;
	}

	void Camera::update_projection()
	{
		if (_type == Type::PERSPECTIVE)
		{
			_projection = glm::perspective(_data.prespective.fovy, _data.prespective.aspect, _near, _far);
		}
		else if (_type == Type::PERSPECTIVE)
		{
			_projection = glm::ortho(_data.ortho.left, _data.ortho.right, _data.ortho.bottom, _data.ortho.top, _near, _far);
		}
	}

	void Camera::add_to_scene(lly::Scene * scene)
	{
		scene->add_node(this);
	}

	void Camera::begin_render()
	{
		System::instance().add_render_command_group(_target, _clear_flags);
		System::instance().set_projection_matrix(_projection);

		glm::vec3 center = _position + _rotation * glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = _rotation * glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 view = glm::lookAt(_position, center, up);
		System::instance().set_view_matrix(view);

		System::instance().set_eye(_position);
	}
}
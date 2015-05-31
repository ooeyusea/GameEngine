#include "Light.h"
#include "../System.h"
#include "../render/AmbientCommand.h"
#include "../render/DirectLightCommand.h"
#include "../render/PointLightingCommand.h"
#include "../render/SpotLightingCommand.h"
#include "Scene.h"

namespace lly {
	Light::Light()
		: _type(Type::AMBIENT)
		, _constant(0.0f)
		, _linear(0.0f)
		, _quadratic(0.0f)
		, _exp(0.0f)
		, _cutoff(0.0f)
	{
	}


	Light::~Light()
	{
	}

	void Light::draw_light()
	{
		glm::vec3 direction = _rotation * _direction;
		switch (_type)
		{
		case Type::AMBIENT: System::instance().add_lighting_command(new AmbientCommand(_color)); break;
		case Type::DIRECTIONAL: System::instance().add_lighting_command(new DirectLightCommand(_color, direction));  break;
		case Type::POINT: System::instance().add_lighting_command(new PointLightingCommand(_color, _position, _constant, _linear, _quadratic)); break;
		case Type::SPOT: System::instance().add_lighting_command(new SpotLightingCommand(_color, _position, direction, _constant, _linear, _quadratic, _exp, _cutoff)); break;
		}
	}

	void Light::add_to_scene(lly::Scene * scene)
	{
		scene->add_node(this);
	}
}

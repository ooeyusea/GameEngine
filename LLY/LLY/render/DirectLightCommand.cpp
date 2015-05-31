#include "DirectLightCommand.h"
#include "Device.h"

namespace lly {
	void DirectLightCommand::draw(Device& device)
	{
		device.setUniformWith1i("u_light_type", 2);
		device.setUniformWith4f("u_light_color", _color.r, _color.g, _color.b, _color.a);
		device.setUniformWith3f("u_light_direction", _direction.x, _direction.y, _direction.z);
	}
}

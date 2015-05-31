#include "AmbientCommand.h"
#include "Device.h"

namespace lly {
	void AmbientCommand::draw(Device& device)
	{
		device.setUniformWith1i("u_light_type", 1);
		device.setUniformWith4f("u_light_color", _ambient.r, _ambient.g, _ambient.b, _ambient.a);
	}
}

#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
#include "../System.h"
#include "../resource/Mesh.h"
#include "../resource/skeleton_data.h"
#include "skeleton.h"
#include "bone.h"

namespace lly {
	Entity::Entity()
		: _mesh(nullptr)
		, _skeleton(nullptr)
	{
	}


	Entity::~Entity()
	{
	}

	void Entity::add_mesh(Mesh * mesh)
	{
		_mesh = mesh;
		if (mesh->has_skeleton())
		{
			auto skeleton_data = System::instance().get_resource_manager().get_skeleton(mesh->get_skeleton());
			_skeleton = skeleton_data->create_skeleton();
			add_child(_skeleton->get_root_bone());
			_skeleton->get_root_bone()->set_parent(this);
		}
	}

	void Entity::draw()
	{
		if (_mesh != nullptr)
		{
			if (_skeleton != nullptr)
				_mesh->draw(_skeleton);
			else
			{
				_mesh->draw();
			}
		}
	}
	
	void Entity::update(float elapse)
	{
		if (_skeleton != nullptr)
			_skeleton->update(elapse);
		apply_transform();
	}

	void Entity::play_animation(const std::string& animation, bool loop)
	{
		if (_skeleton != nullptr)
			_skeleton->set_animation(animation, loop, 1.0f);
	}
}

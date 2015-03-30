#include "GameObject.h"

namespace lly {

	void GameObject::destory(GameObject* gameObject)
	{
		gameObject->removeFromParent();

		gameObject->onDestory();

		delete gameObject;
	}

	GameObject::GameObject()
		: _parent(nullptr)
	{
	}

	GameObject::~GameObject()
	{
		for (auto& child : _children)
		{
			delete child;
		}
		_children.clear();
	}

	void GameObject::setParent(GameObject* parent)
	{
		_parent = parent;
	}

	GameObject* GameObject::getParent()
	{
		return _parent;
	}

	void GameObject::addChild(GameObject* child)
	{
		_children.push_back(child);

		child->setParent(this);

		child->onEnter();
	}

	void GameObject::removeChild(GameObject* child)
	{
		for (std::vector<GameObject*>::const_iterator it = _children.begin();
			 it != _children.end();
			 ++it)
		{
			if (*it == child)
			{
				_children.erase(it);

				child->setParent(nullptr);

				child->onExit();

				return;
			}
		}
	}

	void GameObject::removeFromParent()
	{
		_parent->removeChild(this);
	}

	void GameObject::onEnter()
	{
	}

	void GameObject::onExit()
	{
	}

	void GameObject::onDestory()
	{
	}

	void GameObject::updateBefore()
	{
	}

	void GameObject::update()
	{
	}

	void GameObject::updateAfter()
	{
	}
}
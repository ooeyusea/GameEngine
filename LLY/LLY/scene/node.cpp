#include <glm/gtc/matrix_transform.hpp>
#include "node.h"
#include "../util/FuncUnitl.h"

namespace lly {
    Node::Node()
        : _parent(nullptr)
		, _scale(1.0f)
    {

    }

    Node::~Node()
    {
        for (auto child : _children)
        {
            delete child;
        }
        _children.clear();
    }

	void Node::remove_from_parent()
	{
		if (_parent)
			_parent->remove_child(this);
	}

    void Node::add_child(Node * node)
    {
        _children.push_back(node);
    }

	void Node::remove_child(Node * node)
	{
		auto itr = std::find(_children.begin(), _children.end(), node);
		if (itr != _children.end())
			_children.erase(itr);
	}

    Node * Node::get_child_by_name(const std::string& name)
    {
        for (auto child : _children)
        {
            if (child->get_name() == name)
            {
                return child;
            }
        }
        return nullptr;
    }

	void Node::visit(std::function<void(Node*)> f)
	{
		f(this);
		for (auto child : _children)
		{
			child->visit(f);
		}
	}

    void Node::apply_transform()
    {
		glm::mat4 transform = lly_util::to_sqt(_position, _rotation, _scale);
		on_changed();

        if (_parent)
        {
            _transform = _parent->get_transform() * transform;
        }
        else
            _transform = transform;

        for (auto child : _children)
        {
            child->apply_transform();
        }
    }
}

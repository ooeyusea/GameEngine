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

    void Node::add_child(Node * node)
    {
        _children.push_back(node);
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

    void Node::apply_transform()
    {
		glm::mat4 transform = lly_util::to_sqt(_position, _rotation, _scale);

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

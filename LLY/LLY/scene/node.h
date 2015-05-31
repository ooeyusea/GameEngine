#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/fwd.hpp>
#include <functional>

namespace lly {
	class Scene;
    class Node
    {
    public:
        Node();
        virtual ~Node();

        void set_name(const std::string& name) { _name = name; }
        const std::string& get_name() const { return _name; }

        void set_parent(Node * node) { _parent = node; }
        Node * get_parent() const { return _parent; }
		void remove_from_parent();

        void add_child(Node * node);
        Node * get_child_by_name(const std::string& name);
		void remove_child(Node * node);

        void set_position(float x, float y, float z) { _position = glm::vec3(x, y, z); }
        void set_position(const glm::vec3& v) { _position = v; }
        void set_position_x(float x) { _position.x = x; }
        void set_position_y(float y) { _position.y = y; }
        void set_position_z(float z) { _position.y = z; }
        const glm::vec3& get_position() { return _position; }

        void set_rotation(float x, float y, float z) { _rotation = glm::quat(glm::vec3(x, y, z)); }
        void set_rotation(const glm::vec3& v) { _rotation = glm::quat(v); }
        void set_rotation(const glm::quat& q) { _rotation = q; }
        const glm::quat& get_rotation() { return _rotation; }

        void set_scale(float x, float y, float z) { _scale = glm::vec3(x, y, z); }
        void set_scale(const glm::vec3& v) { _scale = v; }
        void set_scale_x(float x) { _scale.x = x; }
        void set_scale_y(float y) { _scale.y = y; }
        void set_scale_z(float z) { _scale.y = z; }
        const glm::vec3& get_scale() { return _scale; }

        const glm::mat4& get_transform() const { return _transform; }

		virtual void draw() {}
		virtual void update(float elapse) {}
		virtual void last_update() {}

		virtual void add_to_scene(lly::Scene * scene) {}

		virtual void visit(std::function<void (Node*)> f);

        void apply_transform();

    protected:
        std::string _name;
        Node * _parent;
        std::vector<Node*> _children;

        glm::vec3 _position;
        glm::quat _rotation;
        glm::vec3 _scale;

        glm::mat4 _transform;
    };
}

#endif //NODE_H_

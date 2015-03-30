#ifndef BONE_DATA_H_
#define BONE_DATA_H_
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/fwd.hpp>
#include <string>
#include <vector>
#include <functional>

namespace lly {
	class SkeletonData;

    class BoneData
    {
    public:
		BoneData();
		virtual ~BoneData();

        void set_creator(SkeletonData * creator) { _creator = creator; }
		SkeletonData * get_creator() const { return _creator; }

		void set_name(const std::string& name) { _name = name; }
		const std::string& get_name() const { return _name; }

		void set_parent(BoneData * node) { _parent = node; }
		BoneData * get_parent() const { return _parent; }

		void add_child(BoneData * node);
		BoneData * get_child_by_name(const std::string& name);

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

		void visit_child(std::function<void (BoneData*)> f);

    private:
		SkeletonData * _creator;

		std::string _name;
		BoneData * _parent;
		std::vector<BoneData*> _children;

		glm::vec3 _position;
		glm::quat _rotation;
		glm::vec3 _scale;
    };
}
#endif //BONE_DATA_H_

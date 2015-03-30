#ifndef KEYFRAME_H_
#define KEYFRAME_H_
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/fwd.hpp>

namespace lly {
	class Skeleton;
    class Track;
    class KeyFrame
    {
    public:
        KeyFrame(float time_pos) : _time_pos(time_pos) {}
        virtual ~KeyFrame() {}

        float get_time_pos() const { return _time_pos; }

        virtual void apply(Skeleton* skeleton, Track * track, float weight) = 0;
        virtual void interpolate_from(KeyFrame * a, KeyFrame * b) = 0;

    protected:
        float _time_pos;
    };

    class TransformFrame : public KeyFrame
    {
    public:
        TransformFrame(float time_pos) : KeyFrame(time_pos) {}
        virtual ~TransformFrame() {}

        void set_position(const glm::vec3& v) { _position = v; }
        const glm::vec3& get_position() { return _position; }

        void set_rotation(const glm::quat& q) { _rotation = q; }
        const glm::quat& get_rotation() { return _rotation; }

        void set_scale(const glm::vec3& v) { _scale = v; }
        const glm::vec3& get_scale() { return _scale; }

		void apply(Skeleton* skeleton, Track * track, float weight);
        void interpolate_from(KeyFrame * a, KeyFrame * b);

    private:
        glm::vec3 _position;
        glm::quat _rotation;
        glm::vec3 _scale;
    };
}

#endif //KEYFRAME_H_

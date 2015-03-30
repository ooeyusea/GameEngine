#ifndef TRACK_H_
#define TRACK_H_
#include <string>
#include <vector>

namespace lly {
    class KeyFrame;
	class Skeleton;
    class Track
    {
    public:
        Track() : _total_time(0.0f) {}
        virtual ~Track() {}

        void add_key_frame(KeyFrame * frame) { _frames.push_back(frame); }

        float get_total_time() const { return _total_time; }
        void set_total_time(float v) { _total_time = v; }

        virtual void apply(Skeleton* skeleton, float time_pos, float weight) = 0;

    protected:
        void get_interpolate_frame(KeyFrame& frame);

    private:
        std::vector<KeyFrame*> _frames;
        float _total_time;
    };

    class NodeTrack : public Track
    {
    public:
        NodeTrack() {}
        virtual ~NodeTrack() {}

        void set_target(const std::string& target) { _target = target; }
        const std::string& get_target() { return _target; }

        virtual void apply(Skeleton* skeleton, float time_pos, float weight);

    private:
        std::string _target;
    };
}
#endif //TRACK_H_


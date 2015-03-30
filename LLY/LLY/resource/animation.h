#ifndef ANIMATION_H_
#define ANIMATION_H_
#include <vector>
#include <string>

namespace lly {
    class Track;
	class Skeleton;
    class Animation
    {
    public:
        Animation() {}
        ~Animation() {}

        void set_name(const std::string& name) { _name = name; }
        const std::string& get_name() const { return _name; }

        void set_length(float length) { _length = length; }
        float get_length() const { return _length; }

        void add_track(Track * track) { _tracks.push_back(track); }

        void apply(Skeleton* skeleton, float time_pos, float weight);

    private:
        std::string _name;
        std::vector<Track*> _tracks;
        float _length;
    };
}
#endif //ANIMATION_H_

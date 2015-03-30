#ifndef RESOURCE_H_
#define RESOURCE_H_

namespace lly {

	class Resource
	{
	public:
		Resource();
		virtual ~Resource();

		int get_id() const { return _id; }
		void set_id(int id) { _id = id; }

		void use() { ++_use_count; }
		void unuse() { --_use_count; }

	private:
		unsigned int _id;
		unsigned int _use_count;
	};

}

#endif //RESOURCE_H_

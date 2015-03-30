#ifndef STRINGTABLE_H_
#define STRINGTABLE_H_
#include <unordered_map>
#include <string>

namespace lly_util
{
	class StringTable
	{
	public:
		StringTable() {}
		~StringTable() {}

		unsigned int get_hash_id(const std::string& str);
		const std::string& get_string(unsigned int id) const;

	private:
		unsigned int get_hash(const std::string&) const;

		std::unordered_map<unsigned int, std::string> _caches;
	};
}

#endif //STRINGTABLE_H_

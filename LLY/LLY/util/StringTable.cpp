#include "StringTable.h"

namespace lly_util
{
	unsigned int StringTable::get_hash_id(const std::string& str)
	{
		auto id = get_hash(str);
		auto itr = _caches.find(id);
		if (itr != _caches.end())
		{
			if (str != itr->second)
				throw std::logic_error("string hash collision");
		}
		else
			_caches[id] = str;

		return id;
	}

	const std::string& StringTable::get_string(unsigned int id) const
	{
		auto itr = _caches.find(id);
		if (itr == _caches.end())
		{
			throw std::logic_error("unset string");
		}

		return itr->second;
	}

	unsigned int StringTable::get_hash(const std::string& str) const
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0;

		for (auto& c : str)
		{
			hash = hash * seed + c;
		}

		return (hash & 0x7FFFFFFF);
	}
}

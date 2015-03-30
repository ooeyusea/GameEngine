#include "FileUtil.h"
#include <stdlib.h>
#include <algorithm>
#include <cctype>

namespace lly_util {
	Data::Data()
		: _data(nullptr)
		, _len(0)
	{

	}

	Data::Data(int len)
		: _data(nullptr)
		, _len(len)
	{
		_data = (char*)malloc(_len + 1);
		memset(_data, 0, _len + 1);
	}

	Data::Data(Data&& rhs)
		: _data(rhs._data)
		, _len(rhs._len)
	{
		rhs._data = nullptr;
		rhs._len = 0;
	}

	Data::~Data()
	{
		if (_data != nullptr)
		{
			delete _data;
			_data = nullptr;
		}
	}

	Data& Data::operator=(Data&& rhs)
	{
		std::swap(_data, rhs._data);
		std::swap(_len, rhs._len);

		return *this;
	}

	bool Data::skip_white_space(int& offset)
	{
		while (offset < _len)
		{
			if (!std::isspace(_data[offset]))
				return true;
			++offset;
		}
		return false;
	}

	bool Data::cmp(const char* str, int& offset)
	{
		int len = strlen(str);
		if (offset + len > _len)
			return false;

		if (strncmp(_data + offset, str, len) == 0)
		{
			++offset;
			return true;
		}
		return false;
	}

	bool Data::next(const char* str, int& offset)
	{
		int len = strlen(str);
		while (offset + len <= _len)
		{
			if (_data[offset] == str[0])
			{
				if (strncmp(_data + offset, str, len) == 0)
				{
					offset += len;
					return true;
				}
			}
			++offset;
		}
		return false;
	}

	bool Data::get_word(int& offset, std::string& word)
	{
		if (_data[offset] == '{' ||
			_data[offset] == '}' ||
			_data[offset] == '[' ||
			_data[offset] == ']' ||
			_data[offset] == '=' ||
			_data[offset] == ',')
		{
			word += _data[offset];
			++offset;
			return true;
		}

		word.reserve(20);
		while (offset <= _len)
		{
			if (std::isalnum(_data[offset]) || _data[offset] == '/' || _data[offset] == '.' || _data[offset] == '_' || _data[offset] == '-')
			{
				word += _data[offset];
			}
			else if (std::isspace(_data[offset]) ||
				     _data[offset] == '{' ||
					 _data[offset] == '}' ||
					 _data[offset] == '[' ||
					 _data[offset] == ']' ||
					 _data[offset] == '=' ||
					 _data[offset] == ',')
			{
				break;
			}
			else
			{
				return false;
			}

			++offset;
		}
		return true;
	}

	Data load_from_file(const std::string& filename)
	{
		FILE * fp = fopen(filename.c_str(), "rb");
		if (fp == nullptr)
		{
			throw std::runtime_error("can not find file:" + filename);
		}

		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		Data data(size);
		int read_size = fread(data.data(), sizeof(char), size, fp);
		fclose(fp);

		if (0 == read_size)
		{
			throw std::runtime_error("read no data from file:" + filename);
		}

		return std::move(data);
	}
}
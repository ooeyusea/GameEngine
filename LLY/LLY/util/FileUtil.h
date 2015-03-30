#ifndef FILEUTIL_H_
#define FILEUTIL_H_
#include <string>

namespace lly_util
{
	class Data
	{
	public:
		Data();
		Data(int len);
		Data(Data&& rhs);
		~Data();

		Data& operator=(Data&& rhs);

		char* data() { return _data; }
		int len() const { return _len; }

		bool skip_white_space(int& offset);
		bool cmp(const char* str, int& offset);
		bool next(const char* str, int& offset);
		bool get_word(int& offset, std::string& word);

	private:
		char* _data;
		int _len;
	};

	Data load_from_file(const std::string& filename);
}

#endif //FILEUTIL_H_


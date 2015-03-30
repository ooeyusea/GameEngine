#ifndef COMMONUTIL_H_
#define COMMONUTIL_H_

namespace lly {

	class CommonUtil
	{
	public:
		static void log(const char* format, ...);

	private:
		CommonUtil();
		~CommonUtil();
	};
}

#endif
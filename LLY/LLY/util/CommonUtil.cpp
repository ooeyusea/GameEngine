#include "CommonUtil.h"
#include <stdarg.h>
#include <string>

namespace lly {

	void log(const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char buffer[1024];
		vsnprintf(buffer, 1024 - 3, format, args);
		strcat(buffer, "\n");

		printf("%s", buffer);
		fflush(stdout);

		va_end(args);
	}

	CommonUtil::CommonUtil()
	{
	}

	CommonUtil::~CommonUtil()
	{
	}
}
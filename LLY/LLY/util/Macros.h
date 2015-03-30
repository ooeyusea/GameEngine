#ifndef MACROS_H_
#define MACROS_H_

#include "CommonUtil.h"
#include <assert.h>

#if defined(_DEBUG)
#define DLOG(format, ...) lly::CommonUtil::log(format, ##__VA_ARGS__)
#else
#define DLOG(...)
#endif

#define SAFE_DELETE(p) do {delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p) do {delete[] (p); (p) = nullptr; } while(0)

#define ASSERT(cond) assert(cond)

#define DEG_TO_RAD(x) ((x) * 0.0174532925f)
#define RAD_TO_DEG(x) ((x) * 57.29577951f)

#define MERGE_RENDER_STATE(mask, mask_var, variable) \
		if ((rhs.##mask_var & Mask::##mask) > 0)\
		{\
			if ((mask_var & Mask::##mask) == 0)\
			{\
				variable = rhs.##variable;\
				mask_var |= Mask::##mask; \
			}\
		}

#define LLY_SAFE_DELETE(ptr) \
		if (ptr != nullptr)\
		{\
			delete ptr;\
			ptr = nullptr;\
		}

#endif
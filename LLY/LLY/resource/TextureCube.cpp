#include "TextureCube.h"
#include "../util/FileUtil.h"
#include <png.h>
#include "Image.h"
#include "../util/FuncUnitl.h"

namespace lly {

	TextureCube::TextureCube()
		: _tex(0)
	{
	}


	TextureCube::~TextureCube()
	{
		if (_tex > 0)
		{
			glDeleteTextures(1, &_tex);
			_tex = 0;
		}
	}
}

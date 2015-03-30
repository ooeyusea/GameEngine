#include <Windows.h>
#include <tchar.h>

#include "render/Device.h"
#include "System.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	lly::Device device;
	try
	{
		lly::System::instance().set_device(&device).init("lly.config").run();
	}
	catch (std::exception& e)
	{
		::MessageBoxA(NULL, e.what(), "Exception", MB_OK);
	}

	return 0;
}
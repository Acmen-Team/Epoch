#include "eppch.h"

#include "Log.h"
#include "Application.h"

#ifdef EP_RELEASE
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif // EP_DEBUG

int main()
{
  int mainVer = 0;
  int minVer = 3;

  Epoch::Log::Init();

  EP_CORE_TRACE("------Initialized Log!------");
  EP_CORE_INFO("------Epoch Engine StartUp!------");
  EP_CORE_INFO("------Version(Alpha):{0}.{1}-------", mainVer, minVer);

  auto app = Epoch::CreateApplication();
  app->StartUp();
  delete app;

  return 0;
}
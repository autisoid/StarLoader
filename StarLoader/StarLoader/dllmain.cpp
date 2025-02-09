/**
 * Copyright - xWhitey, 2025.
 * DLLMAIN.C - The file with our DLL entrypoint.
 *
 * StarLoader (an .asi loader for GoldSrc) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "fake_exports.hpp"

typedef void(*_InitializeASI)(void);

char g_pszLogFileBuffer[0xFFFF];
bool g_bErasedTheLogFileAtLaunch;

HMODULE* g_rgpPlugins;
int g_nTotalPluginsLoaded;

void InsertPlugin(HMODULE _Handle) {
	void* pTemp;

	g_nTotalPluginsLoaded++;
	pTemp = realloc(g_rgpPlugins, g_nTotalPluginsLoaded * sizeof(_Handle));
	if (pTemp) {
		g_rgpPlugins = (HMODULE*)pTemp;
	} else {
		g_nTotalPluginsLoaded--;
		return;
	}

	g_rgpPlugins[g_nTotalPluginsLoaded - 1] = _Handle;
}

void CallDeinitialiseForEachPlugin() {
	if (!g_rgpPlugins) return;

	for (int nIndex = 0; nIndex < g_nTotalPluginsLoaded; nIndex++) {
		HMODULE hLibrary = g_rgpPlugins[nIndex];
		if (!hLibrary) continue;

		_InitializeASI pfnProceudre = (_InitializeASI)GetProcAddress(hLibrary, "DeinitializeASI");
		if (pfnProceudre)
			pfnProceudre();

		pfnProceudre = (_InitializeASI)GetProcAddress(hLibrary, "ASIShutdown");
		if (pfnProceudre)
			pfnProceudre();
	}

	free(g_rgpPlugins);
}

const char* GetLastErrorAsHumanReadable(DWORD _ErrorCode) {
	if (_ErrorCode == 0) {
		return NULL;
	}

	const char* pszResult = NULL;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
		_ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (char*)&pszResult, 0, NULL);

	return pszResult;
}

bool DoesFileExist(const char* _FileName) {
	FILE* pfTarget;
	errno_t nError = fopen_s(&pfTarget, _FileName, "r");

	bool bResult = !nError && pfTarget;

	if (pfTarget) {
		fclose(pfTarget);
	}

	return bResult;
}

bool TryCreatingPluginsDirectory() {
	char pszDirectory[MAX_PATH];
	char pszTargetDirectory[MAX_PATH];
	FILE* pfTrick;
	errno_t nError;
	DWORD dwErrorCode, dwResult;
	const char* pszLastErrorHr;
	char rgchMessage[1024];

	if (!GetCurrentDir(pszDirectory, sizeof(pszDirectory), false)) {
		return false;
	}

	sprintf_s(pszTargetDirectory, "%s\\" PLUGINS_DIRECTORY, pszDirectory);
	nError = fopen_s(&pfTrick, pszTargetDirectory, "r");

	if (nError == EISDIR) {
		return true;
	}

	if (!nError || pfTrick) {
		_unlink(pszTargetDirectory); //somebody is trying to trick the trickster, again
	}

	dwResult = CreateDirectory(pszTargetDirectory, NULL);

	if (dwResult == 0) {
		dwErrorCode = GetLastError();
		if (dwErrorCode == ERROR_ALREADY_EXISTS) {
			return true;
		}

		pszLastErrorHr = GetLastErrorAsHumanReadable(dwErrorCode);
		sprintf_s(rgchMessage, "Failure creating plugins directory!\nLast error: %d%s%s", dwErrorCode, pszLastErrorHr == NULL ? "" : "\nHuman readable: ", pszLastErrorHr == NULL ? "" : pszLastErrorHr);
		MessageBoxA(NULL, rgchMessage, MESSAGEBOX_TITLE, MB_ICONERROR);

		if (pszLastErrorHr) {
			LocalFree((HLOCAL)pszLastErrorHr);
		}

		return false;
	}

	return true;
}

unsigned long GetCurrentDir(char* _Buffer, size_t _BufferSize, bool _Log) {
	DWORD dwErrorCode, dwResult;
	const char* pszLastErrorHr;
	char rgchMessage[1024];

	dwResult = GetCurrentDirectoryA(_BufferSize, _Buffer);

	if (!dwResult) {
		dwErrorCode = GetLastError();
		pszLastErrorHr = GetLastErrorAsHumanReadable(dwErrorCode);
		sprintf_s(rgchMessage, "Failure getting current directory!\nLast error: %d%s%s", dwErrorCode, pszLastErrorHr == NULL ? "" : "\nHuman readable: ", pszLastErrorHr == NULL ? "" : pszLastErrorHr);
		MessageBoxA(NULL, rgchMessage, MESSAGEBOX_TITLE, MB_ICONERROR);

		if (_Log) {
			PrintToLogFile("[ERROR] %s\n", rgchMessage);
		}

		if (pszLastErrorHr) {
			LocalFree((HLOCAL)pszLastErrorHr);
		}
	}

	return dwResult;
}

int SetCurrentDir(char* _Buffer, bool _Log) {
	DWORD dwErrorCode;
	BOOLEAN bResult;
	const char* pszLastErrorHr;
	char rgchMessage[1024];

	bResult = SetCurrentDirectoryA(_Buffer);

	if (!bResult) {
		dwErrorCode = GetLastError();
		pszLastErrorHr = GetLastErrorAsHumanReadable(dwErrorCode);
		sprintf_s(rgchMessage, "Failure setting current directory!\nLast error: %d%s%s", dwErrorCode, pszLastErrorHr == NULL ? "" : "\nHuman readable: ", pszLastErrorHr == NULL ? "" : pszLastErrorHr);
		MessageBoxA(NULL, rgchMessage, MESSAGEBOX_TITLE, MB_ICONERROR);

		if (_Log) {
			PrintToLogFile("[ERROR] %s\n", rgchMessage);
		}

		if (pszLastErrorHr) {
			LocalFree((HLOCAL)pszLastErrorHr);
		}
	}

	return bResult;
}

void PrintToLogFile(const char* _Format, ...) {
	char pszDirectory[MAX_PATH]; //Buffer for getting current directory (and setting it back after we change it)
	char pszTargetDirectory[MAX_PATH]; //Buffer for setting current directory
	char rgchMessage[1024];
	FILE* pfLog;
	errno_t nError;
	int cbNumWritten;
	va_list va;

	memset(pszDirectory, 0, sizeof(pszDirectory));

	if (!GetCurrentDir(pszDirectory, sizeof(pszDirectory), false)) {
		return;
	}

	sprintf_s(pszTargetDirectory, "%s\\" PLUGINS_DIRECTORY, pszDirectory);

	if (!SetCurrentDir(pszTargetDirectory, false)) {
		SetCurrentDirectoryA(pszDirectory); //restore the original directory

		return;
	}

	if (!g_bErasedTheLogFileAtLaunch) {
		if (DoesFileExist(".\\" LOG_FILE)) {
			nError = fopen_s(&pfLog, ".\\" LOG_FILE, "w"); //erase the contents of the previous log file
			if (!nError) {
				fclose(pfLog);
			}
		}

		g_bErasedTheLogFileAtLaunch = true; //erase it once because if we get into this if we try to open
											//the log file again with "w" mode it will get erased again
	}

	nError = fopen_s(&pfLog, ".\\" LOG_FILE, "a+");
	if (nError) {
		sprintf_s(rgchMessage, "Failure opening the debug log file!\nTarget file: %s\\%s", pszTargetDirectory, LOG_FILE);
		MessageBoxA(NULL, rgchMessage, MESSAGEBOX_TITLE, MB_ICONERROR);

		SetCurrentDirectoryA(pszDirectory); //restore the original directory

		return;
	}

	memset(g_pszLogFileBuffer, 0, sizeof(g_pszLogFileBuffer));

	va_start(va, _Format);
	cbNumWritten = _vsnprintf_s(g_pszLogFileBuffer, sizeof(g_pszLogFileBuffer) - 1, _Format, va);
	va_end(va);

	if (cbNumWritten < 0 || cbNumWritten >= sizeof(g_pszLogFileBuffer)) {
		cbNumWritten = sprintf_s(g_pszLogFileBuffer, "[FATAL] Failure printing the debug text into the buffer.\n");
	}

	fwrite(g_pszLogFileBuffer, sizeof(g_pszLogFileBuffer[0]), cbNumWritten, pfLog);

	fclose(pfLog); //don't leak handles

	SetCurrentDirectoryA(pszDirectory); //clean up stuff after us doing the job
}

void LoadASIs() {
	char pszDirectory[MAX_PATH]; //Buffer for getting current directory (and setting it back after we change it)
	char pszTargetDirectory[MAX_PATH]; //Buffer for setting current directory
	char pszDllPath[MAX_PATH]; //Buffer for loading the plugins
	char rgchMessage[1024]; //Buffer for error messages
	DWORD dwErrorCode;
	WIN32_FIND_DATA fd;

	memset(pszDirectory, 0, sizeof(pszDirectory));

	PrintToLogFile("[DEBUG] Loading plugins NOW!\n");

	if (!GetCurrentDir(pszDirectory, sizeof(pszDirectory), true)) {
		return;
	}

	sprintf_s(pszTargetDirectory, "%s\\" PLUGINS_DIRECTORY, pszDirectory);

	if (!SetCurrentDir(pszTargetDirectory, true)) {
		SetCurrentDirectoryA(pszDirectory); //restore the original directory

		return;
	}

	HANDLE hFile = FindFirstFileA("*.asi", &fd);
	if (hFile != INVALID_HANDLE_VALUE) {
		do {
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) { //check if somebody is trying to trick the trickster
				size_t cbFileNameLength = strlen(fd.cFileName);

				if (fd.cFileName[cbFileNameLength - 4] == '.' &&
					(fd.cFileName[cbFileNameLength - 3] == 'a' || fd.cFileName[cbFileNameLength - 3] == 'A') &&
					(fd.cFileName[cbFileNameLength - 2] == 's' || fd.cFileName[cbFileNameLength - 2] == 'S') &&
					(fd.cFileName[cbFileNameLength - 1] == 'i' || fd.cFileName[cbFileNameLength - 1] == 'I')) {
					memset(pszDllPath, 0, sizeof(pszDllPath));
					sprintf_s(pszDllPath, "%s\\%s", pszTargetDirectory, fd.cFileName);

					if (GetModuleHandleA(pszDllPath) == NULL) { //check if the library isn't loaded already
						SetCurrentDirectoryA(pszDirectory); //restore directory so the plugin can access stuff from the game root directory
						HMODULE hLibrary = LoadLibraryA(pszDllPath);
						SetCurrentDirectoryA(pszDirectory); //restore directory in case the plugin switched it

						if (!hLibrary) {
							dwErrorCode = GetLastError();
							sprintf_s(rgchMessage, "Unable to load plugin \"%s\"!\nLast error: %d", fd.cFileName, dwErrorCode);
							MessageBoxA(NULL, rgchMessage, MESSAGEBOX_TITLE, MB_ICONERROR);
							PrintToLogFile("[ERROR] %s\n", rgchMessage);
						} else {
							PrintToLogFile("[DEBUG] Loaded plugin \"%s\".\n", fd.cFileName);

							_InitializeASI pfnProcedure = (_InitializeASI)GetProcAddress(hLibrary, "InitializeASI");

							if (pfnProcedure) {
								pfnProcedure();
								PrintToLogFile("[DEBUG] Called \"InitializeASI\" procedure in plugin \"%s\".\n", fd.cFileName);
							}

							pfnProcedure = (_InitializeASI)GetProcAddress(hLibrary, "ASIMain");

							if (pfnProcedure) {
								pfnProcedure();
								PrintToLogFile("[DEBUG] Called \"ASIMain\" procedure in plugin \"%s\".\n", fd.cFileName);
							}

							InsertPlugin(hLibrary); //this call goes last so we don't call shutdown for plugins BEFORE initializing them
						}
						SetCurrentDirectoryA(pszTargetDirectory); //restore our directory
					}
				}
			}
		} while (FindNextFileA(hFile, &fd));
		FindClose(hFile);
	}

	SetCurrentDirectoryA(pszDirectory); //clean up stuff after us doing the job
}

DWORD WINAPI DllMain( _In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved )
{
	if (_Reason == DLL_PROCESS_ATTACH)
	{
		if (!TryCreatingPluginsDirectory()) {
			return FALSE;
		}

		PrintToLogFile("[DEBUG] " MESSAGEBOX_TITLE " ver. " STARLOADER_VERSION " loaded!\n");

		LoadRealHidDll();

		LoadASIs();

		return TRUE;
	}
	if (_Reason == DLL_PROCESS_ATTACH)
	{
		CallDeinitialiseForEachPlugin();

		PrintToLogFile("[DEBUG] Shutting down gracefully...\n");

		return TRUE;
	}

	return FALSE;
}
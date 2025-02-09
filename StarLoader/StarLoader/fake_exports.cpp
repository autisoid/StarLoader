/**
 * Copyright - xWhitey, 2025.
 * HID_EXPORTS.C - description
 *
 * StarLoader (an .asi loader for GoldSrc) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

//hid_exports.c -- here we are faking the real hid.dll exports

#include "StdAfx.h"
#include "fake_exports.hpp"

HMODULE g_hRealHIDDll;

#define DECLARE_EXPORT(_ReturnType, _Name, ...)				\
	typedef _ReturnType (WINAPI* _ ## _Name)(__VA_ARGS__);	\
	_ ## _Name ORIG_ ## _Name;								\

#define CALL_ORIGINAL_EXPORT(_Name, ...) ORIG_ ## _Name(__VA_ARGS__)

#define IMPLEMENT_EXPORT(_ReturnType, _Name, ...) _ReturnType __declspec(dllexport) WINAPI _Name(__VA_ARGS__)

#define GET_EXPORT(_Name)																										\
							do {																								\
								ORIG_ ## _Name = (decltype(ORIG_ ## _Name)) GetProcAddress(g_hRealHIDDll, #_Name);				\
								if (ORIG_ ## _Name == NULL) {																	\
									dwErrorCode = GetLastError();																\
									pszLastErrorHr = GetLastErrorAsHumanReadable(dwErrorCode);									\
									memset(pszBuffer, 0, sizeof(pszBuffer));													\
									sprintf_s(pszBuffer, "Failure getting " #_Name " export!\nLast error: %d"					\
											"%s%s", dwErrorCode,																\
											pszLastErrorHr == NULL ? "" : "\nHuman readable: ",									\
											pszLastErrorHr == NULL ? "" : pszLastErrorHr);										\
									MessageBoxA(NULL, pszBuffer, MESSAGEBOX_TITLE, MB_ICONERROR);								\
									PrintToLogFile("[ERROR] %s\n", pszBuffer);													\
																																\
									if (pszLastErrorHr) {																		\
										LocalFree((HLOCAL)pszLastErrorHr);														\
									}																							\
								} else {																						\
									cbNumLoadedExports++;																		\
								}																								\
							} while (0)																							\

DECLARE_EXPORT(LONG, HidP_GetCaps, PVOID PreparsedData, PVOID Capabilities);
DECLARE_EXPORT(BOOLEAN, HidD_GetPreparsedData, HANDLE HidDeviceObject, PVOID* PreparsedData);
DECLARE_EXPORT(BOOLEAN, HidD_FreePreparsedData, HANDLE HidDeviceObject);
DECLARE_EXPORT(VOID, HidD_GetHidGuid, PVOID HidGuid);
DECLARE_EXPORT(BOOLEAN, HidD_FlushQueue, HANDLE HidDeviceObject);
DECLARE_EXPORT(BOOLEAN, HidD_GetAttributes, HANDLE HidDeviceObject, PVOID Attributes);
DECLARE_EXPORT(BOOLEAN, HidD_GetConfiguration, HANDLE HidDeviceObject, PVOID Configuration, ULONG ConfigurationLength);
DECLARE_EXPORT(BOOLEAN, HidD_GetFeature, HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_GetIndexedString, HANDLE HidDeviceObject, ULONG StringIndex, PVOID Buffer, ULONG BufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_GetInputReport, HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_GetManufacturerString, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_GetMsGenreDescriptor, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_GetNumInputBuffers, HANDLE HidDeviceObject, PULONG NumberBuffers);
DECLARE_EXPORT(BOOLEAN, HidD_GetPhysicalDescriptor, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_GetProductString, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_GetSerialNumberString, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_SetConfiguration, HANDLE HidDeviceObject, PVOID Configuration, ULONG ConfigurationLength);
DECLARE_EXPORT(BOOLEAN, HidD_SetFeature, HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength);
DECLARE_EXPORT(BOOLEAN, HidD_SetNumInputBuffers, HANDLE HidDeviceObject, ULONG NumberBuffers);
DECLARE_EXPORT(BOOLEAN, HidD_SetOutputReport, HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength);
DECLARE_EXPORT(LONG, HidP_GetButtonCaps, INT ReportType, PVOID ButtonCaps, PUSHORT ButtonCapsLength, PVOID PreparsedData);
DECLARE_EXPORT(LONG, HidP_GetData, INT ReportType, PVOID DataList, PULONG DataLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_GetExtendedAttributes, INT ReportType, USHORT DataIndex, PVOID PreparsedData, PVOID Attributes, PULONG LengthAttributes);
DECLARE_EXPORT(LONG, HidP_GetLinkCollectionNodes, PVOID LinkCollectionNodes, PULONG LinkCollectionNodesLength, PVOID PreparsedData);
DECLARE_EXPORT(LONG, HidP_GetScaledUsageValue, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PLONG UsageValue, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_GetSpecificButtonCaps, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PVOID ButtonCaps, PUSHORT ButtonCapsLength, PVOID PreparsedData);
DECLARE_EXPORT(LONG, HidP_GetSpecificValueCaps, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PVOID ValueCaps, PUSHORT ValueCapsLength, PVOID PreparsedData);
DECLARE_EXPORT(LONG, HidP_GetUsageValue, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PULONG UsageValue, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_GetUsageValueArray, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PCHAR UsageValue, USHORT UsageValueByteLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_GetUsages, INT ReportType, USHORT UsagePage, USHORT LinkCollection, PUSHORT UsageList, PULONG UsageLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_GetUsagesEx, INT ReportType, USHORT LinkCollection, PVOID ButtonList, ULONG* UsageLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_GetValueCaps, INT ReportType, PVOID ValueCaps, PUSHORT ValueCapsLength, PVOID PreparsedData);
DECLARE_EXPORT(LONG, HidP_InitializeReportForID, INT ReportType, UCHAR ReportID, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(ULONG, HidP_MaxDataListLength, INT ReportType, PVOID PreparsedData);
DECLARE_EXPORT(ULONG, HidP_MaxUsageListLength, INT ReportType, USHORT UsagePage, PVOID PreparsedData);
DECLARE_EXPORT(LONG, HidP_SetData, INT ReportType, PVOID DataList, PULONG DataLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_SetScaledUsageValue, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, LONG UsageValue, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_SetUsageValue, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, ULONG UsageValue, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_SetUsageValueArray, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PCHAR UsageValue, USHORT UsageValueByteLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_SetUsages, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT* UsageList, PULONG UsageLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_UnsetUsages, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT* UsageList, PULONG UsageLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength);
DECLARE_EXPORT(LONG, HidP_TranslateUsagesToI8042ScanCodes, USHORT* ChangedUsageList, ULONG UsageListLength, INT KeyAction, PVOID ModifierState, PVOID InsertCodesProcedure, PVOID InsertCodesContext);
DECLARE_EXPORT(LONG, HidP_UsageListDifference, USHORT* PreviousUsageList, USHORT* CurrentUsageList, USHORT* BreakUsageList, USHORT* MakeUsageList, ULONG UsageListLength);

void LoadRealHidDll() {
	char pszBuffer[MAX_PATH]; //Buffer for retrieving system directory
	char pszDllPath[MAX_PATH];
	DWORD dwErrorCode;
	const char* pszLastErrorHr;
	size_t cbNumLoadedExports = 0;

	PrintToLogFile("[DEBUG] Trying to load the real hid.dll NOW!\n");

	if (!GetSystemDirectoryA(pszBuffer, sizeof(pszBuffer))) {
		dwErrorCode = GetLastError();
		pszLastErrorHr = GetLastErrorAsHumanReadable(dwErrorCode);
		memset(pszBuffer, 0, sizeof(pszBuffer));
		sprintf_s(pszBuffer, "Failure getting system directory!\nLast error: %d%s%s", dwErrorCode, pszLastErrorHr == NULL ? "" : "\nHuman readable: ", pszLastErrorHr == NULL ? "" : pszLastErrorHr);
		MessageBoxA(NULL, pszBuffer, MESSAGEBOX_TITLE, MB_ICONERROR);
		PrintToLogFile("[ERROR] %s\n", pszBuffer);

		if (pszLastErrorHr) {
			LocalFree((HLOCAL)pszLastErrorHr);
		}

		return;
	}

	sprintf_s(pszDllPath, "%s\\hid.dll", pszBuffer);

	g_hRealHIDDll = LoadLibraryA(pszDllPath); //Windows will automatically swap System32 to SysWoW64, so we don't even have to bother about that.

	if (!g_hRealHIDDll) {
		dwErrorCode = GetLastError();
		pszLastErrorHr = GetLastErrorAsHumanReadable(dwErrorCode);
		memset(pszBuffer, 0, sizeof(pszBuffer));
		sprintf_s(pszBuffer, "Failure loading the real hid.dll!\nLast error: %d%s%s", dwErrorCode, pszLastErrorHr == NULL ? "" : "\nHuman readable: ", pszLastErrorHr == NULL ? "" : pszLastErrorHr);
		MessageBoxA(NULL, pszBuffer, MESSAGEBOX_TITLE, MB_ICONERROR);
		PrintToLogFile("[ERROR] %s\n", pszBuffer);

		if (pszLastErrorHr) {
			LocalFree((HLOCAL)pszLastErrorHr);
		}

		return;
	}

	PrintToLogFile("[DEBUG] Loaded it, now getting the exports.\n");

	GET_EXPORT(HidP_GetCaps);
	GET_EXPORT(HidD_GetPreparsedData);
	GET_EXPORT(HidD_FreePreparsedData);
	GET_EXPORT(HidD_GetHidGuid);
	GET_EXPORT(HidD_FlushQueue);
	GET_EXPORT(HidD_GetAttributes);
	GET_EXPORT(HidD_GetConfiguration);
	GET_EXPORT(HidD_GetFeature);
	GET_EXPORT(HidD_GetIndexedString);
	GET_EXPORT(HidD_GetInputReport);
	GET_EXPORT(HidD_GetManufacturerString);
	GET_EXPORT(HidD_GetMsGenreDescriptor);
	GET_EXPORT(HidD_GetNumInputBuffers);
	GET_EXPORT(HidD_GetPhysicalDescriptor);
	GET_EXPORT(HidD_GetProductString);
	GET_EXPORT(HidD_GetSerialNumberString);
	GET_EXPORT(HidD_SetConfiguration);
	GET_EXPORT(HidD_SetFeature);
	GET_EXPORT(HidD_SetNumInputBuffers);
	GET_EXPORT(HidD_SetOutputReport);
	GET_EXPORT(HidP_GetButtonCaps);
	GET_EXPORT(HidP_GetData);
	GET_EXPORT(HidP_GetExtendedAttributes);
	GET_EXPORT(HidP_GetLinkCollectionNodes);
	GET_EXPORT(HidP_GetScaledUsageValue);
	GET_EXPORT(HidP_GetSpecificButtonCaps);
	GET_EXPORT(HidP_GetSpecificValueCaps);
	GET_EXPORT(HidP_GetUsageValue);
	GET_EXPORT(HidP_GetUsageValueArray);
	GET_EXPORT(HidP_GetUsages);
	GET_EXPORT(HidP_GetUsagesEx);
	GET_EXPORT(HidP_GetValueCaps);
	GET_EXPORT(HidP_InitializeReportForID);
	GET_EXPORT(HidP_MaxDataListLength);
	GET_EXPORT(HidP_MaxUsageListLength);
	GET_EXPORT(HidP_SetData);
	GET_EXPORT(HidP_SetScaledUsageValue);
	GET_EXPORT(HidP_SetUsageValue);
	GET_EXPORT(HidP_SetUsageValueArray);
	GET_EXPORT(HidP_SetUsages);
	GET_EXPORT(HidP_UnsetUsages);
	GET_EXPORT(HidP_TranslateUsagesToI8042ScanCodes);
	GET_EXPORT(HidP_UsageListDifference);

	PrintToLogFile("[DEBUG] Loaded %d exports out of %d needed!\n", cbNumLoadedExports, NUM_REAL_EXPORTS);
}

IMPLEMENT_EXPORT(LONG, HidP_GetCaps, PVOID PreparsedData, PVOID Capabilities)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetCaps, PreparsedData, Capabilities);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetPreparsedData, HANDLE HidDeviceObject, PVOID* PreparsedData)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetPreparsedData, HidDeviceObject, PreparsedData);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_FreePreparsedData, HANDLE HidDeviceObject)
{
	return CALL_ORIGINAL_EXPORT(HidD_FreePreparsedData, HidDeviceObject);
}

IMPLEMENT_EXPORT(VOID, HidD_GetHidGuid, PVOID HidGuid)
{
	CALL_ORIGINAL_EXPORT(HidD_GetHidGuid, HidGuid);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_FlushQueue, HANDLE HidDeviceObject)
{
	return CALL_ORIGINAL_EXPORT(HidD_FlushQueue, HidDeviceObject);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetAttributes, HANDLE HidDeviceObject, PVOID Attributes)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetAttributes, HidDeviceObject, Attributes);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetConfiguration, HANDLE HidDeviceObject, PVOID Configuration, ULONG ConfigurationLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetConfiguration, HidDeviceObject, Configuration, ConfigurationLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetFeature, HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetFeature, HidDeviceObject, ReportBuffer, ReportBufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetIndexedString, HANDLE HidDeviceObject, ULONG StringIndex, PVOID Buffer, ULONG BufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetIndexedString, HidDeviceObject, StringIndex, Buffer, BufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetInputReport, HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetInputReport, HidDeviceObject, ReportBuffer, ReportBufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetManufacturerString, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetManufacturerString, HidDeviceObject, Buffer, BufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetMsGenreDescriptor, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetMsGenreDescriptor, HidDeviceObject, Buffer, BufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetNumInputBuffers, HANDLE HidDeviceObject, PULONG NumberBuffers)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetNumInputBuffers, HidDeviceObject, NumberBuffers);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetPhysicalDescriptor, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetPhysicalDescriptor, HidDeviceObject, Buffer, BufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetProductString, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetProductString, HidDeviceObject, Buffer, BufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_GetSerialNumberString, HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_GetSerialNumberString, HidDeviceObject, Buffer, BufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_SetConfiguration, HANDLE HidDeviceObject, PVOID Configuration, ULONG ConfigurationLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_SetConfiguration, HidDeviceObject, Configuration, ConfigurationLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_SetFeature, HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_SetFeature, HidDeviceObject, ReportBuffer, ReportBufferLength);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_SetNumInputBuffers, HANDLE HidDeviceObject, ULONG NumberBuffers)
{
	return CALL_ORIGINAL_EXPORT(HidD_SetNumInputBuffers, HidDeviceObject, NumberBuffers);
}

IMPLEMENT_EXPORT(BOOLEAN, HidD_SetOutputReport, HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength)
{
	return CALL_ORIGINAL_EXPORT(HidD_SetOutputReport, HidDeviceObject, ReportBuffer, ReportBufferLength);
}

IMPLEMENT_EXPORT(LONG, HidP_GetButtonCaps, INT ReportType, PVOID ButtonCaps, PUSHORT ButtonCapsLength, PVOID PreparsedData)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetButtonCaps, ReportType, ButtonCaps, ButtonCapsLength, PreparsedData);
}

IMPLEMENT_EXPORT(LONG, HidP_GetData, INT ReportType, PVOID DataList, PULONG DataLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetData, ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_GetExtendedAttributes, INT ReportType, USHORT DataIndex, PVOID PreparsedData, PVOID Attributes, PULONG LengthAttributes)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetExtendedAttributes, ReportType, DataIndex, PreparsedData, Attributes, LengthAttributes);
}

IMPLEMENT_EXPORT(LONG, HidP_GetLinkCollectionNodes, PVOID LinkCollectionNodes, PULONG LinkCollectionNodesLength, PVOID PreparsedData)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetLinkCollectionNodes, LinkCollectionNodes, LinkCollectionNodesLength, PreparsedData);
}

IMPLEMENT_EXPORT(LONG, HidP_GetScaledUsageValue, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PLONG UsageValue, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetScaledUsageValue, ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_GetSpecificButtonCaps, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PVOID ButtonCaps, PUSHORT ButtonCapsLength, PVOID PreparsedData)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetSpecificButtonCaps, ReportType, UsagePage, LinkCollection, Usage, ButtonCaps, ButtonCapsLength, PreparsedData);
}

IMPLEMENT_EXPORT(LONG, HidP_GetSpecificValueCaps, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PVOID ValueCaps, PUSHORT ValueCapsLength, PVOID PreparsedData)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetSpecificValueCaps, ReportType, UsagePage, LinkCollection, Usage, ValueCaps, ValueCapsLength, PreparsedData);
}

IMPLEMENT_EXPORT(LONG, HidP_GetUsageValue, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PULONG UsageValue, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetUsageValue, ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_GetUsageValueArray, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PCHAR UsageValue, USHORT UsageValueByteLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetUsageValueArray, ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_GetUsages, INT ReportType, USHORT UsagePage, USHORT LinkCollection, PUSHORT UsageList, PULONG UsageLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetUsages, ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_GetUsagesEx, INT ReportType, USHORT LinkCollection, PVOID ButtonList, ULONG* UsageLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetUsagesEx, ReportType, LinkCollection, ButtonList, UsageLength, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_GetValueCaps, INT ReportType, PVOID ValueCaps, PUSHORT ValueCapsLength, PVOID PreparsedData)
{
	return CALL_ORIGINAL_EXPORT(HidP_GetValueCaps, ReportType, ValueCaps, ValueCapsLength, PreparsedData);
}

IMPLEMENT_EXPORT(LONG, HidP_InitializeReportForID, INT ReportType, UCHAR ReportID, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_InitializeReportForID, ReportType, ReportID, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(ULONG, HidP_MaxDataListLength, INT ReportType, PVOID PreparsedData)
{
	return CALL_ORIGINAL_EXPORT(HidP_MaxDataListLength, ReportType, PreparsedData);
}

IMPLEMENT_EXPORT(ULONG, HidP_MaxUsageListLength, INT ReportType, USHORT UsagePage, PVOID PreparsedData)
{
	return CALL_ORIGINAL_EXPORT(HidP_MaxUsageListLength, ReportType, UsagePage, PreparsedData);
}

IMPLEMENT_EXPORT(LONG, HidP_SetData, INT ReportType, PVOID DataList, PULONG DataLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_SetData, ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_SetScaledUsageValue, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, LONG UsageValue, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_SetScaledUsageValue, ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_SetUsageValue, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, ULONG UsageValue, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_SetUsageValue, ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_SetUsageValueArray, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT Usage, PCHAR UsageValue, USHORT UsageValueByteLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_SetUsageValueArray, ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_SetUsages, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT* UsageList, PULONG UsageLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_SetUsages, ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_UnsetUsages, INT ReportType, USHORT UsagePage, USHORT LinkCollection, USHORT* UsageList, PULONG UsageLength, PVOID PreparsedData, PCHAR Report, ULONG ReportLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_UnsetUsages, ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
}

IMPLEMENT_EXPORT(LONG, HidP_TranslateUsagesToI8042ScanCodes, USHORT* ChangedUsageList, ULONG UsageListLength, INT KeyAction, PVOID ModifierState, PVOID InsertCodesProcedure, PVOID InsertCodesContext)
{
	return CALL_ORIGINAL_EXPORT(HidP_TranslateUsagesToI8042ScanCodes, ChangedUsageList, UsageListLength, KeyAction, ModifierState, InsertCodesProcedure, InsertCodesContext);
}

IMPLEMENT_EXPORT(LONG, HidP_UsageListDifference, USHORT* PreviousUsageList, USHORT* CurrentUsageList, USHORT* BreakUsageList, USHORT* MakeUsageList, ULONG UsageListLength)
{
	return CALL_ORIGINAL_EXPORT(HidP_UsageListDifference, PreviousUsageList, CurrentUsageList, BreakUsageList, MakeUsageList, UsageListLength);
}
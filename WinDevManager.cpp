#include "pch.h"
#include "WinDevManager.h"


CWinDevManager::CWinDevManager()
{

}

CWinDevManager::~CWinDevManager()
{

}
	

/**
 * Get friendly name of device from class GUID
 * 
 * @param _IN ClassGUID (ex. {xxx-xxx-xxx-xxx} )
 * 
 * @return FriendlyName
 *
 * @note return Empty string if giving wrong classGUID
 */

CString CWinDevManager::GetFriendlyNameFromClassGUID(CString strClassGUID)
{
	CString strFriendlyName = _T("");

	GUID guidClass;

	if (CLSIDFromString(strClassGUID, &guidClass) != S_OK)
	{
		return _T("");
	}

	HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&guidClass, NULL, NULL, DIGCF_PRESENT);
	if (deviceInfoSet == INVALID_HANDLE_VALUE) {

		return _T("");
	}

	SP_DEVINFO_DATA deviceInfoData = { sizeof(SP_DEVINFO_DATA) };
	DWORD deviceIndex = 0;

	while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
		TCHAR buffer[1024];
		DWORD bufferSize = sizeof(buffer);
		DWORD dataType = 0;

		if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME,
			&dataType, reinterpret_cast<PBYTE>(buffer), bufferSize, NULL)) {
			if (dataType == REG_SZ) {
				strFriendlyName = buffer;
				break;
			}
		}
		deviceIndex++;
	}

	SetupDiDestroyDeviceInfoList(deviceInfoSet);

	return strFriendlyName;
}



/**
 * Get class GUID of device from friendly name
 *
 * @param _IN Friendly name
 *
 * @return Class GUID
 *
 * @note return Empty string if giving wrong Friendly name
 */

CString CWinDevManager::GetClassGUIDFromFriendlyName(CString strFriendlyName)
{
	CString strClassGUID = _T("");
	LPOLESTR lpostrClassGUID;
	GUID guidClass;


	HDEVINFO deviceInfoSet = SetupDiGetClassDevs(nullptr, nullptr, nullptr, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (deviceInfoSet == INVALID_HANDLE_VALUE)
	{
		return _T("");
	}

	SP_DEVINFO_DATA deviceInfoData = { sizeof(SP_DEVINFO_DATA) };
	DWORD index = 0;

	while (SetupDiEnumDeviceInfo(deviceInfoSet, index++, &deviceInfoData))
	{
		WCHAR buffer[MAX_PATH];
		if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME,
			nullptr, reinterpret_cast<PBYTE>(buffer), sizeof(buffer), nullptr))
		{

			if (strFriendlyName.Compare(buffer) == 0)
			{
				deviceInfoData.ClassGuid;
				StringFromCLSID(deviceInfoData.ClassGuid, &lpostrClassGUID);

				strClassGUID = lpostrClassGUID;

				break;
			}

		}
	}

	SetupDiDestroyDeviceInfoList(deviceInfoSet);
	return strClassGUID;
}


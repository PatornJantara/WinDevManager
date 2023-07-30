#pragma once

#include <setupapi.h>  
#include <initguid.h>
#include <devpkey.h>


#pragma comment(lib, "Setupapi.lib")


class CWinDevManager
{
public:

	CWinDevManager();
	~CWinDevManager();

	CString GetFriendlyNameFromClassGUID(CString strClassGUID);

	CString GetClassGUIDFromFriendlyName(CString strFriendlyName);


};


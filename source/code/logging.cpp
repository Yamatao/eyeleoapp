/***
* Copyright 2008 by iWin, Inc.
* All rights reserved
* 
* This software is the confidential and proprietary information of iWin, Inc.
* You shall not disclose such Confidential Information and shall use it only 
* in accordance with the terms of the license agreement you entered into with iWin.
*/ 
#include <stdarg.h>
#include "logging.h"
#ifdef WIN32
	#include <stdio.h>
	#include <windows.h>
	#include <shellapi.h>
	#include "shlobj.h"
	#include <direct.h>
#endif

static FILE * dataLog;

wxString GetSavePath()
{
	static bool isInited = false;
	static wxString savePath;

#ifdef WIN32
	if (!isInited)
	{
		wchar_t appDataPath[MAX_PATH];
		
		// Getting a special path
		// CSIDL_COMMON_APPDATA -> 'C:\Documents and Settings\All Users\Application Data\'
		// CSIDL_APPDATA -> 'C:\Documents and Settings\username\Application Data\'
		// CSIDL_COMMON_DOCUMENTS -> 'C:\Documents and Settings\All Users\Documents\'
		if (SHGetSpecialFolderPathW(NULL, appDataPath, CSIDL_APPDATA, TRUE) == FALSE)
		{
			assert(!"SHGetSpecialFolderPath failed");
			return wxString(L"");
		}
		
		savePath.assign(appDataPath);
		savePath += L"\\EyeLeo\\";
		_wmkdir(savePath.c_str());
		
		isInited = true;
	}
#endif

	return savePath;
}

namespace logging
{
	void Init()
	{
#ifndef MASTER_RELEASE
#ifdef WIN32
		_wfopen_s(&dataLog, GetSavePath() + L"log.txt", L"w");
#else
		dataLog = fopen("log.txt", "w");
#endif
		if (dataLog)
			fclose(dataLog);
		dataLog = 0;
#endif
	}

	void logging::msg(wxString const & msg)
	{
#ifndef MASTER_RELEASE
		bool writeNewLine = true;

#ifdef WIN32
		_wfopen_s(&dataLog, (GetSavePath() + L"log.txt").c_str(), L"a");
#else
		dataLog = fopen("log.txt", "a");
#endif

		fwrite((const char*)msg.mb_str(wxConvUTF8), 1, msg.size(), dataLog);

		if (writeNewLine)
			fwrite("\n", 1, 1, dataLog);

		fclose(dataLog);
		dataLog = 0;
#endif
	}
}
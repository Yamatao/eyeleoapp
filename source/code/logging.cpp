#include "logging.h"
#include <stdarg.h>
#include <wx/filefn.h>
#ifdef WIN32
#include "shlobj.h"
#include <direct.h>
#include <shellapi.h>
#include <stdio.h>
#include <windows.h>
#endif

static FILE *logFile;

wxString GetSavePath() {
    static bool isInited = false;
    static wxString savePath;

#ifdef WIN32
    if (!isInited) {
        wchar_t appDataPath[MAX_PATH];

        // Getting a special path
        // CSIDL_COMMON_APPDATA -> 'C:\Documents and Settings\All Users\Application Data\'
        // CSIDL_APPDATA -> 'C:\Documents and Settings\username\Application Data\'
        // CSIDL_COMMON_DOCUMENTS -> 'C:\Documents and Settings\All Users\Documents\'
        if (SHGetSpecialFolderPathW(NULL, appDataPath, CSIDL_APPDATA, TRUE) == FALSE) {
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

namespace logging {
void Init() {
#ifdef WIN32
    _wfopen_s(&logFile, GetSavePath() + L"log.txt", L"r");
#else
    logFile = fopen("log.txt", "w");
#endif
    if (logFile) {
        fseek(logFile, 0, SEEK_END);
        long size = ftell(logFile);
        fseek(logFile, 0, SEEK_SET);

        fclose(logFile);
        logFile = 0;

        if (size > 1024 * 1024 * 3) // remove the log, if's larger than 3 megs
        {
            wxRemoveFile(GetSavePath() + L"log.txt");
        }
    }
}

void msg(wxString const &msg) {
#ifdef WIN32
    _wfopen_s(&logFile, (GetSavePath() + L"log.txt").c_str(), L"a");
#else
    logFile = fopen("log.txt", "a");
#endif

    if (logFile) {
        fwrite((const char *)msg.mb_str(wxConvUTF8), 1, msg.size(), logFile);
        fwrite("\n", 1, 1, logFile);

        fclose(logFile);
    }
    logFile = 0;
}
} // namespace logging
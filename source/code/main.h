#ifndef MAIN_H
#define MAIN_H

#include "task_mgr.h"
#include "wx/taskbar.h"
#include "wx/wx.h"
#include <vector>

class SettingsWindow;
class BigPauseWindow;
class MiniPauseWindow;
class WaitingFullscreenWindow;
class NotificationWindow;
class BeforePauseWindow;

enum EStates
{
    STATE_NONE = 0,
    STATE_FIRST_LAUNCH,
    STATE_SUSPENDED,
    STATE_IDLE,
    STATE_WAITING_SCREEN,
    STATE_START_BIG_PAUSE,
    STATE_AUTO_RELAX,
    STATE_RELAXING,
    STATE_DESTROY
};

class EyeTaskBarIcon;
class DebugWindow;

class EyeApp : public wxApp, ITask {
public:
    EyeApp();

    bool LoadSettings();
    void SaveSettings();
    void ResetSettings();
    void ApplySettings();
    void CheckSettings();

    virtual bool OnInit();
    bool IsOnlyInstance() const;

    virtual int OnExit();
    virtual void Exit();

    void OnUserActivity();
    void UpdateTaskbarText();

    void OpenSettings();
    void OnSettingsClosed();
    void OnMiniPauseWindowClosed(MiniPauseWindow *ptr);
    void OnBigPauseWindowClosed(BigPauseWindow *);
    void OnNotificationWindowClosed();
    void OnDebugWindowClosed();
    void OnCloseWaitingWnd(WaitingFullscreenWindow *ptr);
    void OnCloseBeforePauseWnd(BeforePauseWindow *ptr);
    void OnSkipBigPauseClicked();

    void OnQueryEndSession(wxCloseEvent &evt);
    void OnEndSession(wxCloseEvent &);

    void OnTaskEvent(wxCommandEvent &);

    void OnSessionUnlock();

    void StartBigPause(bool demo = false); // demo is true when 'long pause' is called from Settings window for demo purpose
    void StartMiniPause();
    void StopMiniPause();
    void PostponeBigPause();
    void RestartBigPauseInterval();
    void RefuseBigPause();

    virtual void ExecuteTask(float, long);

    ITask *getWindow(const wxString &address);

    bool GetBigPauseEnabled() const {
        return _enableBigPause;
    }
    int GetBigPauseInterval() const {
        return _bigPauseInterval;
    }
    int GetBigPauseDuration() const {
        return _bigPauseDuration;
    }
    bool GetMiniPauseEnabled() const {
        return _enableMiniPause;
    }
    int GetMiniPauseInterval() const {
        return _miniPauseInterval;
    }
    int GetMiniPauseDuration() const {
        return _miniPauseDuration;
    }
    bool GetWarningEnabled() const {
        return _enableWarning;
    }
    float GetWarningInterval() const {
        return _warningInterval;
    }
    bool GetSoundsEnabled() const {
        return _enableSounds;
    }
    bool GetStrictModeEnabled() const {
        return _enableStrictMode;
    }
    bool GetWindowNearbySetting() const {
        return _settingWindowNearby;
    }
    bool GetInactivityTrackingEnabled() const {
        return _settingInactivityTracking;
    }
    bool GetCanCloseNotificationsSetting() const {
        return _settingCanCloseNotifications;
    }
    bool HasSeenSettings() const {
        return _seenSettingsWindow;
    }
    bool GetShowNotificationsEnabled() const {
        return _showNotificationsEnabled;
    }

    void SetBigPauseEnabled(bool enabled) {
        _enableBigPause = enabled;
    }
    void SetBigPauseInterval(int interval) {
        _bigPauseInterval = interval;
    }
    void SetBigPauseDuration(int duration) {
        _bigPauseDuration = duration;
    }
    void SetMiniPauseEnabled(bool enabled) {
        _enableMiniPause = enabled;
    }
    void SetMiniPauseInterval(int interval) {
        _miniPauseInterval = interval;
    }
    void SetMiniPauseDuration(int duration) {
        _miniPauseDuration = duration;
    }
    void SetWarningEnabled(bool enabled) {
        _enableWarning = enabled;
    }
    void SetWarningInterval(float interval) {
        _warningInterval = interval;
    }
    void SetSoundsEnabled(bool enabled) {
        _enableSounds = enabled;
    }
    void SetStrictModeEnabled(bool enabled) {
        _enableStrictMode = enabled;
    }
    void SetWindowNearbySetting(bool enabled) {
        _settingWindowNearby = enabled;
    }
    void SetInactivityTrackingEnabled(bool enabled) {
        _settingInactivityTracking = enabled;
    }
    void SetCanCloseNotificationsSetting(bool enabled) {
        _settingCanCloseNotifications = enabled;
    }
    void SetShowNotificationsEnabled(bool enabled) {
        _showNotificationsEnabled = enabled;
    }

    bool IsFullscreenAppRunning(int *display = 0, HWND *fullscreenWndHandle = 0) const;

    void TogglePausedMode(int minites = 0);
    bool isPausedMode() const;

    void TakeLongBreakNow();

    void ChangeState(int nextState, int duration = 0);
    void RepeatState();
    void Stop();

    int GetStateDuration() const {
        return _lastDuration;
    }
    int GetNextState() const {
        return _nextState;
    }

    wxString const &getLang() const {
        return _lang;
    }
    wxString const &getVersionString() const {
        return _version;
    }
    wxString const &getWebsiteString() const {
        return _website;
    }

    bool isFinished() const {
        return _finished;
    }

private:
    SettingsWindow *_settingsWnd;
    EyeTaskBarIcon *_taskBarIcon;

    DebugWindow *_debugWindow;

    wxString _lang;
    wxString _version;
    wxString _website;

    int _lastDuration;
    int _currentState;
    int _nextState;

    bool _showedLongBreakCountdown;

    bool _fastMode;

    bool _finished;

    // settings
    bool _enableBigPause;
    int _bigPauseInterval; // in minutes
    int _bigPauseDuration;
    bool _enableWarning;
    float _warningInterval;
    bool _enableMiniPause;
    int _miniPauseInterval;
    int _miniPauseDuration;
    bool _enableSounds;
    bool _enableStrictMode;
    bool _settingWindowNearby;
    bool _settingInactivityTracking;
    bool _settingCanCloseNotifications;
    bool _seenSettingsWindow;
    bool _firstLaunch;
    bool _showNotificationsEnabled;

    // statistics
    unsigned int _userShortBreakCount;
    unsigned int _userLongBreakCount;
    unsigned int _userEarlySkipCount;
    unsigned int _userLateSkipCount;
    unsigned int _userRefuseCount;
    unsigned int _userPostponeCount;
    unsigned int _userAutoBreakCount;
    long _lastBigPauseTimeLeft; // time left when last shutdown happened
    long _lastMiniPauseTimeLeft;
    wxDateTime _lastShutdown;

    // current state
    long _timeLeftToBigPause;  // ms
    long _timeLeftToMiniPause; // ms

    long _relaxingTimeLeft;
    long _fullscreenBlockDuration;
    long _timeUntilWaitingWnd;
    long _inactivityTime;
    int _postponeCount;

    POINT _cursorPos;

    std::vector<BigPauseWindow *> _bigPauseWnds;
    std::vector<MiniPauseWindow *> _miniPauseWnds;
    std::vector<WaitingFullscreenWindow *> _waitWnds;
    std::vector<BeforePauseWindow *> _beforePauseWnds;
    NotificationWindow *_notificationWnd;

    void ReadConfig();

    void RestartMiniPauseInterval();
    void SetBigPauseTime(long ms);
    void SetMiniPauseTime(long ms);

    bool CheckInactivity();
    void CloseBigPauseWnds();
    void StopBigPause();
    void AutoRelax();
    void ShowWaitingWnd();
    void CloseWaitingWnd();
    void AskForBigPause();
    void CloseBeforePauseWnds();

    void UpdateDebugWindow();

    wxString GetSavePath() const;

    DECLARE_EVENT_TABLE()
};

EyeApp *getApp();

class EyeTaskBarIcon : public wxTaskBarIcon {
public:
    EyeTaskBarIcon();

    virtual wxMenu *CreatePopupMenu();

    void ShowBalloonToolip(wxString const &text, unsigned msec = 1000 * 10);
    void UpdateTooltip(wxString const &text);

    void OnQuit(wxCommandEvent &);
    void OnSettings(wxCommandEvent &);
    void OnPauseResumeMonitoring(wxCommandEvent &);
    void OnPauseResumeMonitoring2(wxCommandEvent &);
    void OnTakeLongBreakNow(wxCommandEvent &);
    void OnLeftButtonDown(/*wxTaskBarIconEvent*/ wxMouseEvent &);

private:
    void RecreatePopupMenu();

    wxMenu *_menu;

    wxIcon *_icon;
    wxIcon *_iconGray; // for paused mode
    wxIcon *_iconSettings;
    wxIcon *_iconPause;
    wxIcon *_iconResume;
};

bool IsWindowsVistaOrGreater();

#endif
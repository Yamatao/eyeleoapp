#ifndef SETTINGS_WND_H
#define SETTINGS_WND_H
#include "wx/wx.h"

enum
{
    ID_TASKBAR_MENU_QUIT = 1,
    ID_TASKBAR_MENU_SETTINGS,
    ID_TASKBAR_MENU_PAUSE_RESUME_MONITORING,
    ID_TASKBAR_MENU_PAUSE_RESUME_MONITORING_2,
    ID_TASKBAR_MENU_TAKE_LONG_BREAK_NOW,

    ID_SETTINGS_CHK_BIG_PAUSES,
    ID_SETTINGS_SEL_BIG_PAUSE_INTERVAL,
    ID_SETTINGS_SEL_BIG_PAUSE_DURATION,

    ID_SETTINGS_CHK_WARN_ABOUT_PAUSES,
    ID_SETTINGS_SEL_WARN_BEFORE_TIME,

    ID_SETTINGS_CHK_MINI_PAUSES,
    ID_SETTINGS_SEL_MINI_PAUSE_INTERVAL,
    ID_SETTINGS_SEL_MINI_PAUSE_DURATION,

    ID_SETTINGS_CHK_ENABLE_SOUNDS,
    ID_SETTINGS_CHK_CAN_CLOSE_NOTIFICATIONS,
    ID_SETTINGS_CHK_ENABLE_STRICT_MODE,

    ID_SETTINGS_CHK_WINDOW_NEARBY,
    ID_SETTINGS_CHK_ENABLE_INACTIVITY_TRACKING,

    ID_SETTINGS_BTN_SAVE_AND_QUIT,
    ID_SETTINGS_BTN_TRY_SHORT_BREAK,
    ID_SETTINGS_BTN_TRY_LONG_BREAK,

    ID_INFORMATION_BTN_GIVE_FEEDBACK,
    ID_INFORMATION_WRITE_EMAIL,
    ID_INFORMATION_MAKE_DONATION,
};

class wxNotebook;
class SettingsWindow : public wxFrame {
public:
    SettingsWindow(const wxString &title);

    void OnShow(wxShowEvent &event);
    void OnClose(wxCloseEvent &event);

    void OnCheckboxClicked(wxCommandEvent &event);

    void OnBigPauseEnabledClicked(wxCommandEvent &event);
    void OnMiniPauseEnabledClicked(wxCommandEvent &event);
    void OnWarningEnabledClicked(wxCommandEvent &event);
    void OnSoundsEnabledClicked(wxCommandEvent &event);
    void OnStrictModeEnabledClicked(wxCommandEvent &event);
    void OnCanCloseNotificationsClicked(wxCommandEvent &event);
    void OnSaveAndCloseClicked(wxCommandEvent &event);
    void OnTryShortBreakClicked(wxCommandEvent &);
    void OnTryLongBreakClicked(wxCommandEvent &);
    void OnGiveFeedbackClicked(wxCommandEvent &event);
    void OnDonateClicked(wxCommandEvent &event);

    void PullSettings();
    void PushSettings();

private:
    wxNotebook *_notebook;

    wxIcon _iconSettings;
    wxIcon _iconInformation;
    wxIcon _iconLongBreak;
    wxIcon _iconShortBreak;
    wxIcon _iconSound;
    wxIcon _iconStrictMode;
    wxIcon _iconCanCloseNotifs;
    wxIcon _iconWarning;
    wxIcon _iconWindow;
    wxImageList _notebookImgList;

    wxCheckBox *_chkBigPauses;
    wxComboBox *_selBigPauseInterval;
    wxComboBox *_selBigPauseDuration;

    wxCheckBox *_chkWarnPauses;
    wxComboBox *_selWarnPauseDelays;

    wxCheckBox *_chkMiniPauses;
    wxComboBox *_selMiniPauseInterval;
    wxComboBox *_selMiniPauseDuration;

    wxCheckBox *_chkEnableSounds;
    wxCheckBox *_chkEnableStrictMode;
    wxCheckBox *_chkCanCloseNotifications;

    wxCheckBox *_chkWindowNearby;
    wxCheckBox *_chkInactivityTracking;

    wxString GetInformation() const;

public:
    void SetBigPauseEnabled(bool value);
    void SetBigPauseInterval(int value);
    void SetBigPauseDuration(int value);
    void SetMiniPauseEnabled(bool value);
    void SetMiniPauseInterval(int value);
    void SetMiniPauseDuration(int value);
    void SetWarningEnabled(bool value);
    void SetWarningInterval(float value);
    void SetSoundsEnabled(bool value);
    void SetStrictModeEnabled(bool value);
    void SetWindowNearbySetting(bool value);
    void SetInactivityTrackingEnabled(bool value);
    void SetCanCloseNotifications(bool value);

private:
    bool GetBigPauseEnabled() const;
    int GetBigPauseInterval() const;
    int GetBigPauseDuration() const;
    bool GetMiniPauseEnabled() const;
    int GetMiniPauseInterval() const;
    int GetMiniPauseDuration() const;
    bool GetWarningEnabled() const;
    float GetWarningInterval() const;
    bool GetSoundsEnabled() const;
    bool GetStrictModeEnabled() const;
    bool GetCanCloseNotifications() const;
    bool GetWindowNearbySetting() const;
    bool GetInactivityTrackingEnabled() const;

    static bool inited;

    DECLARE_EVENT_TABLE()
};

#endif
#include "settings_wnd.h"
#include "language_set.h"
#include "main.h"
#include "pugixml.hpp"
#include "wx/notebook.h"

///////////////////////////////////////////////////////////////////////////////////////

// NOTE: IF YOU CHANGE THOSE VALUES, DON'T FORGET TO CHANGE EyeApp::CheckSettings
wxString bigPauseIntervalChoices[10] = {
    _(""),
};
int bigPauseIntervalValues[] = {20, 25, 30, 35, 40, 45, 50, 60, 90, 120};

wxString bigPauseDurationChoices[8] = {
    _(""),
};
int bigPauseDurationValues[] = {2, 3, 4, 5, 7, 10, 15, 20};

wxString bigWarnDelayChoices[4] = {
    _(""),
};
float bigWarnDelayValues[] = {0.3333333f, 0.5f, 1.0f, 1.5f};

wxString miniPauseIntervalChoices[6] = {
    _(""),
};
int miniPauseIntervalValues[] = {5, 8, 10, 15, 20, 30};

wxString miniPauseDurationChoices[3] = {
    _(""),
};
int miniPauseDurationValues[] = {8, 15, 20}; // in seconds

bool SettingsWindow::inited = false;

BEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////

SettingsWindow::SettingsWindow(const wxString &title)
    : wxFrame(NULL, -1, title, wxDefaultPosition, wxDefaultSize, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
    , _iconSettings(L"Resources/settings.ico", wxBITMAP_TYPE_ICO)
    , _iconInformation(L"Resources/information.ico", wxBITMAP_TYPE_ICO)
    , _iconLongBreak(L"Resources/long_break.ico", wxBITMAP_TYPE_ICO, 16, 16)
    , _iconShortBreak(L"Resources/short_break.ico", wxBITMAP_TYPE_ICO, 16, 16)
    , _iconSound(L"Resources/sound.ico", wxBITMAP_TYPE_ICO, 16, 16)
    , _iconStrictMode(L"Resources/strict_mode.ico", wxBITMAP_TYPE_ICO, 16, 16)
    , _iconWarning(L"Resources/notification.ico", wxBITMAP_TYPE_ICO, 16, 16)
    , _iconCanCloseNotifs(L"Resources/can-close-notifs.png", wxBITMAP_TYPE_PNG, 16, 16)
    , _iconWindow(L"Resources/window.png", wxBITMAP_TYPE_PNG, 16, 16)
    , _notebookImgList(16, 16, true, 2) {
    if (!SettingsWindow::inited) {
        for (int i = 0; i < (sizeof(bigPauseIntervalChoices) / sizeof(bigPauseIntervalChoices[0])); i++)
            bigPauseIntervalChoices[i] = langPack->Get(wxString::Format("settings_big_pause_value_1_%d", i + 1));

        for (int i = 0; i < (sizeof(bigPauseDurationChoices) / sizeof(bigPauseDurationChoices[0])); i++)
            bigPauseDurationChoices[i] = langPack->Get(wxString::Format("settings_big_pause_value_2_%d", i + 1));

        for (int i = 0; i < (sizeof(bigWarnDelayChoices) / sizeof(bigWarnDelayChoices[0])); i++)
            bigWarnDelayChoices[i] = langPack->Get(wxString::Format("settings_big_pause_warning_value_%d", i + 1));

        for (int i = 0; i < (sizeof(miniPauseIntervalChoices) / sizeof(miniPauseIntervalChoices[0])); i++)
            miniPauseIntervalChoices[i] = langPack->Get(wxString::Format("settings_mini_pause_value_%d", i + 1));

        for (int i = 0; i < (sizeof(miniPauseDurationChoices) / sizeof(miniPauseDurationChoices[0])); i++)
            miniPauseDurationChoices[i] = langPack->Get(wxString::Format("settings_mini_pause_duration_value_%d", i + 1));

        SettingsWindow::inited = true;
    }

    SetInitialSize(wxSize(580, 420));

    SetIcon(wxIcon(L"Resources/icon.ico", wxBITMAP_TYPE_ICO, 16, 16));

    _notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT);

    wxPanel *pageSettings = new wxPanel(_notebook);
    wxPanel *pageInformation = new wxPanel(_notebook);

    // PAGE SETTINGS
    wxBoxSizer *sizerSettings = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer *sizerPanel = new wxStaticBoxSizer(wxVERTICAL, pageSettings, langPack->Get("settings_general_panel_label"));

    wxBoxSizer *sizerBigPauses = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBitmap *imgIcon;
    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconLongBreak);
    _chkBigPauses = new wxCheckBox(pageSettings,
                                   ID_SETTINGS_CHK_BIG_PAUSES,
                                   langPack->Get("settings_big_pause_label_1"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   wxCHK_2STATE,
                                   wxDefaultValidator,
                                   _("chkBigPauses"));
    _selBigPauseInterval = new wxComboBox(pageSettings,
                                          ID_SETTINGS_SEL_BIG_PAUSE_INTERVAL,
                                          wxEmptyString,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          sizeof(bigPauseIntervalChoices) / sizeof(bigPauseIntervalChoices[0]),
                                          bigPauseIntervalChoices,
                                          wxCB_DROPDOWN | wxCB_READONLY,
                                          wxDefaultValidator,
                                          _("selBigPauseInterval"));
    wxStaticText *txtBigPauseDuration = new wxStaticText(pageSettings, wxID_ANY, langPack->Get("settings_big_pause_label_2"));
    _selBigPauseDuration = new wxComboBox(pageSettings,
                                          ID_SETTINGS_SEL_BIG_PAUSE_INTERVAL,
                                          wxEmptyString,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          sizeof(bigPauseDurationChoices) / sizeof(bigPauseDurationChoices[0]),
                                          bigPauseDurationChoices,
                                          wxCB_DROPDOWN | wxCB_READONLY,
                                          wxDefaultValidator,
                                          _("selBigPauseDuration"));

    wxToolTip *tooltip1_0 = new wxToolTip(langPack->Get("settings_big_pause_tooltip"));
    tooltip1_0->SetDelay(800);
    wxToolTip *tooltip1_1 = new wxToolTip(langPack->Get("settings_big_pause_tooltip"));
    tooltip1_1->SetDelay(800);
    wxToolTip *tooltip1_2 = new wxToolTip(langPack->Get("settings_big_pause_tooltip"));
    tooltip1_2->SetDelay(800);
    _selBigPauseInterval->SetToolTip(tooltip1_0);
    _selBigPauseDuration->SetToolTip(tooltip1_1);
    _chkBigPauses->SetToolTip(tooltip1_2);

    sizerBigPauses->Add(imgIcon, wxSizerFlags().Center());
    sizerBigPauses->AddSpacer(3);
    sizerBigPauses->Add(_chkBigPauses, wxSizerFlags().Center().Border(wxALL, 3));
    sizerBigPauses->Add(_selBigPauseInterval, wxSizerFlags().Center());
    sizerBigPauses->AddSpacer(6);
    sizerBigPauses->Add(txtBigPauseDuration, wxSizerFlags().Center());
    sizerBigPauses->AddSpacer(6);
    sizerBigPauses->Add(_selBigPauseDuration, wxSizerFlags().Center());

    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconWarning);
    wxBoxSizer *sizerWarnPauses = new wxBoxSizer(wxHORIZONTAL);
    _chkWarnPauses = new wxCheckBox(pageSettings,
                                    ID_SETTINGS_CHK_WARN_ABOUT_PAUSES,
                                    langPack->Get("settings_big_pause_warning_label"),
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    wxCHK_2STATE,
                                    wxDefaultValidator,
                                    _("chkWarnPauses"));

    _selWarnPauseDelays = new wxComboBox(pageSettings,
                                         ID_SETTINGS_SEL_WARN_BEFORE_TIME,
                                         wxEmptyString,
                                         wxDefaultPosition,
                                         wxDefaultSize,
                                         sizeof(bigWarnDelayChoices) / sizeof(bigWarnDelayChoices[0]),
                                         bigWarnDelayChoices,
                                         wxCB_DROPDOWN | wxCB_READONLY,
                                         wxDefaultValidator,
                                         _("selBigPauseInterval"));
    sizerWarnPauses->Add(imgIcon, wxSizerFlags().Center());
    sizerWarnPauses->AddSpacer(3);
    sizerWarnPauses->Add(_chkWarnPauses, wxSizerFlags().Center().Border(wxALL, 3));
    sizerWarnPauses->Add(_selWarnPauseDelays, wxSizerFlags().Center());

    wxToolTip *tooltip4_0 = new wxToolTip(langPack->Get("settings_big_pause_warning_tooltip"));
    tooltip4_0->SetDelay(800);
    wxToolTip *tooltip4_1 = new wxToolTip(langPack->Get("settings_big_pause_warning_tooltip"));
    tooltip4_1->SetDelay(800);
    _selWarnPauseDelays->SetToolTip(tooltip4_0);
    _chkWarnPauses->SetToolTip(tooltip4_1);

    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconShortBreak);
    wxBoxSizer *sizerMiniPauses = new wxBoxSizer(wxHORIZONTAL);
    _chkMiniPauses = new wxCheckBox(pageSettings,
                                    ID_SETTINGS_CHK_MINI_PAUSES,
                                    langPack->Get("settings_mini_pause_label"),
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    wxCHK_2STATE,
                                    wxDefaultValidator,
                                    _("chkMiniPauses"));
    _selMiniPauseInterval = new wxComboBox(pageSettings,
                                           ID_SETTINGS_SEL_MINI_PAUSE_INTERVAL,
                                           wxEmptyString,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           sizeof(miniPauseIntervalChoices) / sizeof(miniPauseIntervalChoices[0]),
                                           miniPauseIntervalChoices,
                                           wxCB_DROPDOWN | wxCB_READONLY,
                                           wxDefaultValidator,
                                           _("selBigPauseInterval"));
    wxStaticText *txtMiniPauseDuration = new wxStaticText(pageSettings, wxID_ANY, langPack->Get("settings_big_pause_label_2"));
    _selMiniPauseDuration = new wxComboBox(pageSettings,
                                           ID_SETTINGS_SEL_MINI_PAUSE_DURATION,
                                           wxEmptyString,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           sizeof(miniPauseDurationChoices) / sizeof(miniPauseDurationChoices[0]),
                                           miniPauseDurationChoices,
                                           wxCB_DROPDOWN | wxCB_READONLY,
                                           wxDefaultValidator,
                                           _("selBigPauseDuration"));

    sizerMiniPauses->Add(imgIcon, wxSizerFlags().Center());
    sizerMiniPauses->AddSpacer(3);
    sizerMiniPauses->Add(_chkMiniPauses, wxSizerFlags().Center().Border(wxALL, 3));
    sizerMiniPauses->Add(_selMiniPauseInterval, wxSizerFlags().Center());
    sizerMiniPauses->AddSpacer(6);
    sizerMiniPauses->Add(txtMiniPauseDuration, wxSizerFlags().Center());
    sizerMiniPauses->AddSpacer(6);
    sizerMiniPauses->Add(_selMiniPauseDuration, wxSizerFlags().Center());

    wxToolTip *tooltip2_0 = new wxToolTip(langPack->Get("settings_mini_pause_tooltip"));
    tooltip2_0->SetDelay(800);
    wxToolTip *tooltip2_1 = new wxToolTip(langPack->Get("settings_mini_pause_tooltip"));
    tooltip2_1->SetDelay(800);
    _selMiniPauseInterval->SetToolTip(tooltip2_0);
    _chkMiniPauses->SetToolTip(tooltip2_1);

    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconSound);
    wxBoxSizer *sizerEnableSounds = new wxBoxSizer(wxHORIZONTAL);
    _chkEnableSounds = new wxCheckBox(pageSettings,
                                      ID_SETTINGS_CHK_ENABLE_SOUNDS,
                                      langPack->Get("settings_enable_sounds_label"),
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxCHK_2STATE,
                                      wxDefaultValidator,
                                      _("chkEnableSounds"));

    sizerEnableSounds->Add(imgIcon, wxSizerFlags().Center());
    sizerEnableSounds->AddSpacer(3);
    sizerEnableSounds->Add(_chkEnableSounds, wxSizerFlags().Center().Border(wxALL, 3));

    //
    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconStrictMode);
    wxBoxSizer *sizerEnableStrictMode = new wxBoxSizer(wxHORIZONTAL);
    _chkEnableStrictMode = new wxCheckBox(pageSettings,
                                          ID_SETTINGS_CHK_ENABLE_STRICT_MODE,
                                          langPack->Get("settings_enable_strict_mode_label"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxCHK_2STATE,
                                          wxDefaultValidator,
                                          _("chkEnableStrictMode"));

    sizerEnableStrictMode->Add(imgIcon, wxSizerFlags().Center());
    sizerEnableStrictMode->AddSpacer(3);
    sizerEnableStrictMode->Add(_chkEnableStrictMode, wxSizerFlags().Center().Border(wxALL, 3));

    wxToolTip *tooltip3 = new wxToolTip(langPack->Get("settings_enable_strict_mode_tooltip"));
    tooltip3->SetDelay(800);
    _chkEnableStrictMode->SetToolTip(tooltip3);

    //
    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconCanCloseNotifs);
    wxBoxSizer *sizerCanCloseNotifications = new wxBoxSizer(wxHORIZONTAL);
    _chkCanCloseNotifications = new wxCheckBox(pageSettings,
                                               ID_SETTINGS_CHK_CAN_CLOSE_NOTIFICATIONS,
                                               langPack->Get("settings_can_close_notifications_label"),
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxCHK_2STATE,
                                               wxDefaultValidator,
                                               _("chkCanCloseNotifications"));

    sizerCanCloseNotifications->Add(imgIcon, wxSizerFlags().Center());
    sizerCanCloseNotifications->AddSpacer(3);
    sizerCanCloseNotifications->Add(_chkCanCloseNotifications, wxSizerFlags().Center().Border(wxALL, 3));

    wxToolTip *tooltip4 = new wxToolTip(langPack->Get("settings_can_close_notifications_tooltip"));
    tooltip4->SetDelay(800);
    _chkCanCloseNotifications->SetToolTip(tooltip4);

    //
    wxButton *btnTryShortBreak = new wxButton(pageSettings, ID_SETTINGS_BTN_TRY_SHORT_BREAK, langPack->Get("settings_try_short_break_button"));
    wxButton *btnTryLongBreak = new wxButton(pageSettings, ID_SETTINGS_BTN_TRY_LONG_BREAK, langPack->Get("settings_try_long_break_button"));
    wxBoxSizer *sizerTryButtons = new wxBoxSizer(wxHORIZONTAL);
    sizerTryButtons->Add(btnTryShortBreak);
    sizerTryButtons->AddSpacer(2);
    sizerTryButtons->Add(btnTryLongBreak);

    //
    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconWindow);
    _chkWindowNearby = new wxCheckBox(pageSettings,
                                      ID_SETTINGS_CHK_WINDOW_NEARBY,
                                      langPack->Get("settings_have_window_nearby"),
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxCHK_2STATE,
                                      wxDefaultValidator,
                                      _("chkWindowNearby"));
    wxBoxSizer *sizerWindowNearby = new wxBoxSizer(wxHORIZONTAL);

    sizerWindowNearby->Add(imgIcon, wxSizerFlags().Center());
    sizerWindowNearby->AddSpacer(3);
    sizerWindowNearby->Add(_chkWindowNearby, wxSizerFlags().Center().Border(wxALL, 3));

    wxToolTip *tooltip5 = new wxToolTip(langPack->Get("settings_have_window_nearby_tooltip"));
    tooltip5->SetDelay(800);
    _chkWindowNearby->SetToolTip(tooltip5);

    //
    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconWindow);
    _chkInactivityTracking = new wxCheckBox(pageSettings,
                                            ID_SETTINGS_CHK_ENABLE_INACTIVITY_TRACKING,
                                            langPack->Get("settings_can_enable_inactivity_tracking"),
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxCHK_2STATE,
                                            wxDefaultValidator,
                                            _("chkInactivityTracking"));
    wxBoxSizer *sizerInactivityTracking = new wxBoxSizer(wxHORIZONTAL);

    sizerInactivityTracking->Add(imgIcon, wxSizerFlags().Center());
    sizerInactivityTracking->AddSpacer(3);
    sizerInactivityTracking->Add(_chkInactivityTracking, wxSizerFlags().Center().Border(wxALL, 3));

    wxToolTip *tooltip6 = new wxToolTip(langPack->Get("settings_can_enable_inactivity_tracking_tooltip"));
    tooltip6->SetDelay(800);
    _chkInactivityTracking->SetToolTip(tooltip6);


    imgIcon = new wxStaticBitmap(pageSettings, wxID_ANY, _iconWarning);
    _chkShowNotifications = new wxCheckBox(pageSettings,
                                            ID_SETTINGS_CHK_ENABLE_INACTIVITY_TRACKING,
                                            langPack->Get("settings_show_notifications"),
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxCHK_2STATE,
                                            wxDefaultValidator,
                                            _("_chkShowNotifications"));
    wxBoxSizer *sizerShowNotifications = new wxBoxSizer(wxHORIZONTAL);

    sizerShowNotifications->Add(imgIcon, wxSizerFlags().Center());
    sizerShowNotifications->AddSpacer(3);
    sizerShowNotifications->Add(_chkShowNotifications, wxSizerFlags().Center().Border(wxALL, 3));

    wxToolTip *toolShowNotifications = new wxToolTip(langPack->Get("settings_show_notifications_tooltip"));
    toolShowNotifications->SetDelay(800);
    _chkShowNotifications->SetToolTip(toolShowNotifications);

    //
    sizerPanel->Add(sizerBigPauses, wxSizerFlags().Left().Border(wxALL, 4));
    sizerPanel->Add(sizerWarnPauses, wxSizerFlags().Left().Border(wxALL, 4));
    sizerPanel->Add(sizerMiniPauses, wxSizerFlags().Left().Border(wxALL, 4));
    sizerPanel->Add(sizerEnableSounds, wxSizerFlags().Left().Border(wxALL, 4));
    sizerPanel->Add(sizerEnableStrictMode, wxSizerFlags().Left().Border(wxALL, 4));
    sizerPanel->Add(sizerCanCloseNotifications, wxSizerFlags().Left().Border(wxALL, 4));
    sizerPanel->AddSpacer(5);
    sizerPanel->Add(sizerWindowNearby, wxSizerFlags().Left().Border(wxLEFT, 4));
    sizerPanel->AddSpacer(8);
    sizerPanel->Add(sizerInactivityTracking, wxSizerFlags().Left().Border(wxLEFT, 4));
    sizerPanel->AddSpacer(8);
    sizerPanel->Add(sizerShowNotifications, wxSizerFlags().Left().Border(wxLEFT, 4));
    sizerPanel->AddSpacer(8);
    sizerPanel->Add(sizerTryButtons, wxSizerFlags().Left().Border(wxALL, 4));

    sizerSettings->Add(sizerPanel, wxSizerFlags(1).Expand());

    wxButton *btnSaveAndQuit = new wxButton(pageSettings, ID_SETTINGS_BTN_SAVE_AND_QUIT, langPack->Get("settings_save_and_close_button"));

    sizerSettings->Add(btnSaveAndQuit, wxSizerFlags().Left().Border(wxALL, 5));
    pageSettings->SetSizer(sizerSettings);

    // PAGE INFORMATION
    wxBoxSizer *sizerInformation = new wxBoxSizer(wxVERTICAL);

    wxStaticText *textCtrl = new wxStaticText(pageInformation, wxID_ANY, GetInformation());
    wxFont font = textCtrl->GetFont();
    font.SetPointSize(10);
    textCtrl->SetFont(font);
    sizerInformation->Add(textCtrl, wxSizerFlags(1).Border(wxALL, 8).Expand());

    wxBoxSizer *sizerInformationButtons = new wxBoxSizer(wxHORIZONTAL);
    wxButton *btnVisitForum = new wxButton(pageInformation, ID_INFORMATION_BTN_GIVE_FEEDBACK, langPack->Get("information_give_feedback_button"));
    wxButton *btnMakeDonation = new wxButton(pageInformation, ID_INFORMATION_MAKE_DONATION, langPack->Get("information_make_donation_button"));
    sizerInformationButtons->Add(btnVisitForum, wxSizerFlags().Border(wxALL, 5));
    sizerInformationButtons->AddSpacer(5);
    sizerInformationButtons->Add(btnMakeDonation, wxSizerFlags().Border(wxALL, 5));

    sizerInformation->Add(sizerInformationButtons);

    pageInformation->SetSizer(sizerInformation);

    _notebook->AddPage(pageSettings, langPack->Get("settings_settings_tab"));
    _notebook->AddPage(pageInformation, langPack->Get("settings_information_tab"));

    _notebookImgList.Add(_iconSettings);
    _notebookImgList.Add(_iconInformation);
    _notebook->SetImageList(&_notebookImgList);
    _notebook->SetPageImage(0, 0);
    _notebook->SetPageImage(1, 1);

    sizerSettings->SetSizeHints(this);
    SetSize(GetSize().x + 70, GetSize().y + 70);

    Connect(wxEVT_SHOW, wxShowEventHandler(SettingsWindow::OnShow));
    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(SettingsWindow::OnClose));

    Connect(ID_SETTINGS_CHK_BIG_PAUSES, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingsWindow::OnBigPauseEnabledClicked));
    Connect(ID_SETTINGS_CHK_MINI_PAUSES, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingsWindow::OnMiniPauseEnabledClicked));
    Connect(ID_SETTINGS_CHK_WARN_ABOUT_PAUSES, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingsWindow::OnWarningEnabledClicked));
    Connect(ID_SETTINGS_CHK_ENABLE_SOUNDS, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingsWindow::OnSoundsEnabledClicked));
    Connect(ID_SETTINGS_CHK_ENABLE_STRICT_MODE, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingsWindow::OnStrictModeEnabledClicked));
    Connect(ID_SETTINGS_CHK_CAN_CLOSE_NOTIFICATIONS, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingsWindow::OnStrictModeEnabledClicked));

    Connect(ID_SETTINGS_BTN_SAVE_AND_QUIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingsWindow::OnSaveAndCloseClicked));
    Connect(ID_SETTINGS_BTN_TRY_SHORT_BREAK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingsWindow::OnTryShortBreakClicked));
    Connect(ID_SETTINGS_BTN_TRY_LONG_BREAK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingsWindow::OnTryLongBreakClicked));
    Connect(ID_INFORMATION_BTN_GIVE_FEEDBACK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingsWindow::OnGiveFeedbackClicked));
    Connect(ID_INFORMATION_MAKE_DONATION, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingsWindow::OnDonateClicked));
}

void SettingsWindow::OnShow(wxShowEvent &) {
    PullSettings();
}

void SettingsWindow::OnSaveAndCloseClicked(wxCommandEvent &) {
    PushSettings();
    getApp()->OnSettingsClosed();
    Close();
}

void SettingsWindow::OnTryShortBreakClicked(wxCommandEvent &) {
    getApp()->StartMiniPause();
}

void SettingsWindow::OnTryLongBreakClicked(wxCommandEvent &) {
    getApp()->StartBigPause(/*demo*/ true);
}

void SettingsWindow::OnBigPauseEnabledClicked(wxCommandEvent &) {
    bool value = _chkBigPauses->GetValue();

    _selBigPauseInterval->Enable(value);
    _selBigPauseDuration->Enable(value);
}

void SettingsWindow::OnMiniPauseEnabledClicked(wxCommandEvent &) {
    bool value = _chkMiniPauses->GetValue();
    _selMiniPauseInterval->Enable(value);
}

void SettingsWindow::OnWarningEnabledClicked(wxCommandEvent &) {
    bool value = _chkWarnPauses->GetValue();
    _selWarnPauseDelays->Enable(value);
}

void SettingsWindow::OnSoundsEnabledClicked(wxCommandEvent &) {
}

void SettingsWindow::OnStrictModeEnabledClicked(wxCommandEvent &) {
}

void SettingsWindow::OnCanCloseNotificationsClicked(wxCommandEvent &) {
}

void SettingsWindow::OnGiveFeedbackClicked(wxCommandEvent &) {
    wxLaunchDefaultBrowser(wxString::Format(L"http://%s/feedback", getApp()->getWebsiteString()));
}

void SettingsWindow::OnDonateClicked(wxCommandEvent &) {
    wxLaunchDefaultBrowser(wxString::Format(L"http://%s/support", getApp()->getWebsiteString()));
}

wxString SettingsWindow::GetInformation() const {
    wxString text = wxString(L"EyeLeo Version ") + getApp()->getVersionString() + L". " + langPack->Get("information_info_1");
    text += _("\n") + langPack->Get("information_author");
    text += _("\n") + langPack->Get("information_license");
    text += _("\n") + langPack->Get("information_info_2");
    text += _("\n") + langPack->Get("information_info_3");
    return text;
}

void SettingsWindow::SetBigPauseEnabled(bool value) {
    _chkBigPauses->SetValue(value);

    _selBigPauseInterval->Enable(value);
    _selBigPauseDuration->Enable(value);
}

void SettingsWindow::SetBigPauseInterval(int value) {
    int ind = 0;
    int size = sizeof(bigPauseIntervalValues) / sizeof(bigPauseIntervalValues[0]);
    for (; ind < size; ++ind)
        if (bigPauseIntervalValues[ind] == value)
            break;
    if (ind >= size)
        ind = 0;

    _selBigPauseInterval->SetSelection(ind);
}

void SettingsWindow::SetBigPauseDuration(int value) {
    int ind = 0;
    int size = sizeof(bigPauseDurationValues) / sizeof(bigPauseDurationValues[0]);
    for (; ind < size; ++ind)
        if (bigPauseDurationValues[ind] == value)
            break;
    if (ind >= size)
        ind = 0;

    _selBigPauseDuration->SetSelection(ind);
}

void SettingsWindow::SetMiniPauseEnabled(bool value) {
    _chkMiniPauses->SetValue(value);
    _selMiniPauseInterval->Enable(value);
}

void SettingsWindow::SetMiniPauseInterval(int value) {
    int ind = 0;
    int size = sizeof(miniPauseIntervalValues) / sizeof(miniPauseIntervalValues[0]);
    for (; ind < size; ++ind)
        if (miniPauseIntervalValues[ind] == value)
            break;
    if (ind >= size)
        ind = 0;

    _selMiniPauseInterval->SetSelection(ind);
}

void SettingsWindow::SetMiniPauseDuration(int value) {
    int ind = 0;
    int size = sizeof(miniPauseDurationValues) / sizeof(miniPauseDurationValues[0]);
    for (; ind < size; ++ind)
        if (miniPauseDurationValues[ind] == value)
            break;
    if (ind >= size)
        ind = 0;

    _selMiniPauseDuration->SetSelection(ind);
}

void SettingsWindow::SetWarningEnabled(bool value) {
    _chkWarnPauses->SetValue(value);
    _selWarnPauseDelays->Enable(value);
}

void SettingsWindow::SetWarningInterval(float value) {
    int ind = 0;
    int size = sizeof(bigWarnDelayValues) / sizeof(bigWarnDelayValues[0]);
    for (; ind < size; ++ind)
        if (fabsf(bigWarnDelayValues[ind] - value) < 0.00001f)
            break;
    if (ind >= size)
        ind = 0;

    _selWarnPauseDelays->SetSelection(ind);
}

void SettingsWindow::SetSoundsEnabled(bool value) {
    _chkEnableSounds->SetValue(value);
}

void SettingsWindow::SetStrictModeEnabled(bool value) {
    _chkEnableStrictMode->SetValue(value);
}

void SettingsWindow::SetWindowNearbySetting(bool value) {
    _chkWindowNearby->SetValue(value);
}

void SettingsWindow::SetInactivityTrackingEnabled(bool value) {
    _chkInactivityTracking->SetValue(value);
}

void SettingsWindow::SetShowNotificationsEnabled(bool value) {
    _chkShowNotifications->SetValue(value);
}

void SettingsWindow::SetCanCloseNotifications(bool value) {
    _chkCanCloseNotifications->SetValue(value);
}

bool SettingsWindow::GetBigPauseEnabled() const {
    return _chkBigPauses->GetValue();
}

int SettingsWindow::GetBigPauseInterval() const {
    int sel = _selBigPauseInterval->GetCurrentSelection();
    return bigPauseIntervalValues[sel];
}

int SettingsWindow::GetBigPauseDuration() const {
    int sel = _selBigPauseDuration->GetCurrentSelection();
    return bigPauseDurationValues[sel];
}

bool SettingsWindow::GetMiniPauseEnabled() const {
    return _chkMiniPauses->GetValue();
}

int SettingsWindow::GetMiniPauseInterval() const {
    int sel = _selMiniPauseInterval->GetCurrentSelection();
    return miniPauseIntervalValues[sel];
}

int SettingsWindow::GetMiniPauseDuration() const {
    int sel = _selMiniPauseDuration->GetCurrentSelection();
    return miniPauseDurationValues[sel];
}

bool SettingsWindow::GetWarningEnabled() const {
    return _chkWarnPauses->GetValue();
}

float SettingsWindow::GetWarningInterval() const {
    int sel = _selWarnPauseDelays->GetCurrentSelection();
    return bigWarnDelayValues[sel];
}

bool SettingsWindow::GetSoundsEnabled() const {
    return _chkEnableSounds->GetValue();
}

bool SettingsWindow::GetStrictModeEnabled() const {
    return _chkEnableStrictMode->GetValue();
}

bool SettingsWindow::GetCanCloseNotifications() const {
    return _chkCanCloseNotifications->GetValue();
}

bool SettingsWindow::GetWindowNearbySetting() const {
    return _chkWindowNearby->GetValue();
}

bool SettingsWindow::GetInactivityTrackingEnabled() const {
    return _chkInactivityTracking->GetValue();
}

bool SettingsWindow::GetShowNotificationsEnabled() const {
    return _chkShowNotifications->GetValue();
}

void SettingsWindow::PullSettings() {
    SetBigPauseEnabled(getApp()->GetBigPauseEnabled());
    SetBigPauseInterval(getApp()->GetBigPauseInterval());
    SetBigPauseDuration(getApp()->GetBigPauseDuration());
    SetWarningEnabled(getApp()->GetWarningEnabled());
    SetWarningInterval(getApp()->GetWarningInterval());
    SetMiniPauseInterval(getApp()->GetMiniPauseInterval());
    SetMiniPauseDuration(getApp()->GetMiniPauseDuration());
    SetMiniPauseEnabled(getApp()->GetMiniPauseEnabled());
    SetSoundsEnabled(getApp()->GetSoundsEnabled());
    SetStrictModeEnabled(getApp()->GetStrictModeEnabled());
    SetWindowNearbySetting(getApp()->GetWindowNearbySetting());
    SetCanCloseNotifications(getApp()->GetCanCloseNotificationsSetting());
    SetInactivityTrackingEnabled(getApp()->GetInactivityTrackingEnabled());
    SetShowNotificationsEnabled(getApp()->GetShowNotificationsEnabled());    
}

void SettingsWindow::PushSettings() {
    getApp()->SetBigPauseEnabled(GetBigPauseEnabled());
    getApp()->SetBigPauseInterval(GetBigPauseInterval());
    getApp()->SetBigPauseDuration(GetBigPauseDuration());
    getApp()->SetMiniPauseEnabled(GetMiniPauseEnabled());
    getApp()->SetMiniPauseInterval(GetMiniPauseInterval());
    getApp()->SetMiniPauseDuration(GetMiniPauseDuration());
    getApp()->SetWarningEnabled(GetWarningEnabled());
    getApp()->SetWarningInterval(GetWarningInterval());
    getApp()->SetSoundsEnabled(GetSoundsEnabled());
    getApp()->SetStrictModeEnabled(GetStrictModeEnabled());
    getApp()->SetWindowNearbySetting(GetWindowNearbySetting());
    getApp()->SetCanCloseNotificationsSetting(GetCanCloseNotifications());
    getApp()->SetInactivityTrackingEnabled(GetInactivityTrackingEnabled());
    getApp()->SetShowNotificationsEnabled(GetShowNotificationsEnabled());
    getApp()->SaveSettings();
}

void SettingsWindow::OnClose(wxCloseEvent &event) {
    PushSettings();
    getApp()->OnSettingsClosed();
    event.Skip();
}
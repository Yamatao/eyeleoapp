#include "eyecoach_resources.h"

wxBitmap * _backBitmap = 0;
wxBitmap * _backBitmap_long = 0;
wxBitmap * _backBitmap_notification = 0;
wxBitmap * _bmpTitle = 0;
wxBitmap * _bmpWindow = 0;
wxBitmap * _bmpNotificationLeopard = 0;

void LoadEyeLeoResources()
{
	_backBitmap = new wxBitmap("skin2.png", wxBITMAP_TYPE_PNG);
	_backBitmap_long = new wxBitmap("skin3.png", wxBITMAP_TYPE_PNG);
	_backBitmap_notification = new wxBitmap("skin4.png", wxBITMAP_TYPE_PNG);
	_bmpTitle = new wxBitmap("eyeleo_title.png", wxBITMAP_TYPE_PNG);
	_bmpWindow = new wxBitmap("minipause_window.png", wxBITMAP_TYPE_PNG);
	_bmpNotificationLeopard = new wxBitmap("notification_leopard.png", wxBITMAP_TYPE_PNG);
}
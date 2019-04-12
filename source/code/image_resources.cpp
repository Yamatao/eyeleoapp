#include "image_resources.h"

wxBitmap *_backBitmap = 0;
wxBitmap *_backBitmap_long = 0;
wxBitmap *_backBitmap_notification = 0;
wxBitmap *_bmpTitle = 0;
wxBitmap *_bmpWindow = 0;
wxBitmap *_bmpNotificationLeopard = 0;

void LoadEyeLeoResources() {
    _backBitmap = new wxBitmap("Resources/skin2.png", wxBITMAP_TYPE_PNG);
    _backBitmap_long = new wxBitmap("Resources/skin3.png", wxBITMAP_TYPE_PNG);
    _backBitmap_notification = new wxBitmap("Resources/skin4.png", wxBITMAP_TYPE_PNG);
    _bmpTitle = new wxBitmap("Resources/eyeleo_title.png", wxBITMAP_TYPE_PNG);
    _bmpWindow = new wxBitmap("Resources/minipause_window.png", wxBITMAP_TYPE_PNG);
    _bmpNotificationLeopard = new wxBitmap("Resources/notification_leopard.png", wxBITMAP_TYPE_PNG);
}
#ifndef LANGUAGE_SET_H
#define LANGUAGE_SET_H
#include "wx/string.h"
#include <map>

struct LanguagePack {
    wxString author;
    wxString contact;
    std::map<wxString, wxString> strings;

    wxString const &Get(wxString const &id);
    bool Has(wxString const &id) const;
};

bool LoadLanguagePack(wxString const &lang);
void DeleteLanguagePack();

extern LanguagePack *langPack;

#endif
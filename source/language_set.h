#ifndef LANGUAGE_SET_H
#define LANGUAGE_SET_H
#include <map>
#include "wx/string.h"

struct LanguagePack
{
	wxString author;
	wxString contact;
	std::map<wxString, wxString> strings;
	
	wxString const &	get(wxString const & id);
	bool				has(wxString const & id) const;
};

bool LoadLanguagePack(wxString const & lang);

extern LanguagePack * langPack;

#endif
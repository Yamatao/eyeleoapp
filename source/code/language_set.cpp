#include "language_set.h"
#include "pugixml.hpp"

LanguagePack * langPack = 0;

wxString const & LanguagePack::get(wxString const & id)
{
	return strings[id];
}

bool LanguagePack::has(wxString const & id) const
{
	return strings.find(id) != strings.end();
}

bool LoadLanguagePack(wxString const & lang)
{
	wxString filename = wxString::Format(L"Langpacks/langpack.%s.xml", lang);
	
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file(filename.wchar_str());
	
	if (res.status != pugi::status_ok)
		return false;
	
	pugi::xml_node nodeLangPack = doc.child(L"language_pack");
	if (nodeLangPack.empty())
		return false;

	LanguagePack * newPack = new LanguagePack();
	
	newPack->author = nodeLangPack.attribute(L"author").value();
	newPack->contact = nodeLangPack.attribute(L"contact").value();
	
	for (pugi::xml_node node = nodeLangPack.first_child(); node; node = node.next_sibling())
	{
		const wchar_t * name = node.name();
		if (wcscmp(name, L"string") != 0)
			continue;
		
		wxString id = node.attribute(L"id").value();
		wxString text = node.attribute(L"text").value();
		text.Replace(L"{n}", L"\n", true);
		newPack->strings[id] = text;
	}
	
	if (newPack->strings.empty())
	{
		delete newPack;
		return false;
	}
	
	if (langPack)
		delete langPack;
	
	langPack = newPack;
	return true;
}

void DeleteLanguagePack()
{
	if (langPack)
	{
		delete langPack;
		langPack = 0;
	}
}
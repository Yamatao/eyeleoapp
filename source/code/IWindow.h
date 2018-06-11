#pragma once

class IWindow
{
public:
	void SetName(const std::string & name) { assert(!name.empty()); _name = name; }
	const std::string & GetName() const { assert(!_name.empty()); return _name; }

private:
	std::string _name;
};
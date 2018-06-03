#pragma once

class IWindow
{
public:
	void setName(const std::string & name) { assert(!name.empty()); _name = name; }
	const std::string & getName() const { assert(!_name.empty()); return _name; }

private:
	std::string _name;
};
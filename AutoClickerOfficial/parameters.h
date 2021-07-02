#pragma once
#include <string>
#include <map>

class Parameters {


public:
	void ReadFile();
	std::string nova_version;
	std::map<std::string, std::string> variables;
};
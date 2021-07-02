#pragma once

#pragma once
#include <string>
#include <map>

class OCR {


public:
	int Extract_Text(const std::string& image_path, const std::string& database_path, std::string& output_string);
};
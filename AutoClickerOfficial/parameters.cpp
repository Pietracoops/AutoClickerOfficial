#include "parameters.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Parameters::ReadFile()
{

	std::ifstream infile("settings.txt");

    if (!infile)
    {
        std::cout << "No settings.txt file found." << std::endl;
    }
    else
    {
        std::cout << "Loading settings.txt file..." << std::endl;
    }

    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string a, b;
        if (!(iss >> a >> b)) { break; } // error
        if (a == "#") { continue; }
        b = line.erase(0, a.length() + 1);
           
        variables[a] = b;
    }

}
#pragma once

#include <Windows.h>
#include "parameters.h"
#include "ScreenCapture.h"
#include <vector>
#include <fstream>
#include "ClickFunctions.h"
#include <iostream>
#include <time.h>
#include <string>
#include <WinUser.h>
#include <atlimage.h>
#include <wincodec.h>

class PSU {

	HWND hPSUWindow;
	RECT PSURect;

	std::string m_psu_software_name;

	Parameters m_settings_vars;

	Click ClickObject;

	int m_power_on_x = 82;
	int m_power_on_y = 270;
	int m_power_off_x = 126;
	int m_power_off_y = 271;
	int m_power_set_x = 242;
	int m_power_set_y = 275;

	int m_power_supply_down_time;
	int m_power_supply_up_time;

	std::string m_plink_script;
	std::string m_plink_script_execution;

public:

	PSU(const Parameters& vars);
	~PSU();
	bool Initialize();
	bool Active();
	bool Power_off();
	bool Power_on();
	bool StartRTControl(std::string exe_path);
	bool PSCSForegroundValidation();
};
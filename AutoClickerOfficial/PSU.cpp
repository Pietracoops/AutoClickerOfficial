#include "PSU.h"


PSU::PSU(const Parameters& vars)
{

	// Parse parameters
	m_settings_vars = vars;

	if (m_settings_vars.variables["psu_name"].compare("") == 0)
	{
		m_psu_software_name = "Rating: ";
		std::cout << "Defaulting power supply software to: Rating:" << std::endl;
	}
	else
	{
		m_psu_software_name = m_settings_vars.variables["psu_name"];
	}

	if (m_settings_vars.variables["power_supply_down_time"].compare("") == 0)
	{
		m_power_supply_down_time = 10;
		std::cout << "Defaulting power supply down time to: 10 seconds" << std::endl;
	}
	else
	{
		m_power_supply_down_time = std::stoi(m_settings_vars.variables["power_supply_down_time"]);
	}

	if (m_settings_vars.variables["power_supply_up_time"].compare("") == 0)
	{
		m_power_supply_up_time = 20;
		std::cout << "Defaulting power supply up time to: 20 seconds" << std::endl;
	}
	else
	{
		m_power_supply_up_time = std::stoi(m_settings_vars.variables["power_supply_up_time"]);
	}

	if (m_settings_vars.variables["batch_commands"].compare("") == 0)
	{
		m_plink_script = "launch_rtcontrol.bat";
		std::cout << "Defaulting rtcontrol startup batch script name to launch_rtcontrol.bat" << std::endl;
	}
	else
	{
		m_plink_script = m_settings_vars.variables["batch_commands"];
	}

}




PSU::~PSU()
{

}

bool PSU::Initialize()
{

	hPSUWindow = FindWindow(NULL, m_psu_software_name.c_str());

	if (hPSUWindow == NULL)
	{
		std::cout << "not found powersupply" << std::endl;
		return false;
	}
	else
	{
		// Move window to top left corner in case ALT+Printscreen doesn't work
		SetWindowPos(hPSUWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);
		UpdateWindow(hPSUWindow);

		GetWindowRect(hPSUWindow, &PSURect);
		
		std::cout << "found powersupply" << std::endl;
		return true;
	}

}

bool PSU::Active()
{
	do
	{
		// Make PSCS active window
		SetForegroundWindow(hPSUWindow);
		Sleep(2000);

	} while (!PSCSForegroundValidation()); // Validate that it is popped
	
	return true;
}

bool PSU::PSCSForegroundValidation()
{
	HWND foreground_window = GetForegroundWindow();

	if (foreground_window == hPSUWindow)
	{
		std::cout << "PSCS window now active" << std::endl;
		return true;
	}
	else
	{
		std::cout << "PSCS window not active" << std::endl;
		return false;
	}
}

bool PSU::Power_off()
{
	std::cout << "Powering off power supply" << std::endl;
	Sleep(1000);
	// Click the power off button
	ClickObject.LeftClickPos(PSURect.left + m_power_off_x, PSURect.top + m_power_off_y);

	//Wait a second
	Sleep(1000);

	// Click the set on button
	ClickObject.LeftClickPos(PSURect.left + m_power_set_x, PSURect.top + m_power_set_y);
	Sleep(m_power_supply_down_time * 1000);
	return true;
}


bool PSU::Power_on()
{
	std::cout << "Powering on power supply" << std::endl;
	Sleep(1000);
	// Click the power on button
	ClickObject.LeftClickPos(PSURect.left + m_power_on_x, PSURect.top + m_power_on_y);

	//Wait a second
	Sleep(1000);

	// Click the set button
	ClickObject.LeftClickPos(PSURect.left + m_power_set_x, PSURect.top + m_power_set_y);
	Sleep(m_power_supply_up_time * 1000);
	return true;
}

bool PSU::StartRTControl(std::string exe_path)
{
	//plink test@10.0.20.57 - pw Kinova123!pkill - 9 rtcontrol
	//timeout / t 2 / nobreak
	//plink test@10.0.20.57 - pw Kinova123!/ usr / share / 3.1.4 / rtcontrol --ifacename = enp3s0f1 --actuator - count = 7
	//timeout / t 10 / nobreak

	m_plink_script_execution = "START /MIN CMD.EXE /C" + exe_path + "\\" + m_plink_script;
	system(m_plink_script_execution.c_str());
	Sleep(15000);
	return true;
}
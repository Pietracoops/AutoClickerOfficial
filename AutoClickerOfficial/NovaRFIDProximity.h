#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <Windows.h>
#include "parameters.h"
#include "ScreenCapture.h"
#include "PSU.h"
#include <vector>
#include <fstream>
#include "ClickFunctions.h"
#include <iostream>
#include <time.h>
#include <string>
#include <WinUser.h>
#include <atlimage.h>
#include <wincodec.h>
#include <experimental/filesystem>

class NovaProximity {
	HWND hNovaWindow;
	RECT NovaRect;

	Click ClickObject;
	ScreenCapture screen_capture_object;

	int m_rfid_proximity_test_offset_x = 1137;
	int m_rfid_proximity_test_offset_y = 493;

	std::string log_folder_name;
	std::string exe_folder_path;
	std::string output_folder_path;
	std::string ocr_output;

	std::vector<boolean> results;
	int m_passed_tests;
	int m_failed_tests;

	Parameters m_settings_vars;

	int m_proximity_test_duration;
	int m_preperation_time;
	int m_reboot_frequency;
	std::string m_nova_version;

	int m_total_runs;

	bool qnx_system;

	int m_nova_foreground_timeout_counter;


public:
	bool FindNovaWindow();
	void Run();
	void Run_Reboot(PSU& power_supply);
	bool AnalyzeResults(const std::string& ocr_string, const std::string& validation_string);
	void OutputResultsLogs(const std::string output_path);

	bool StartTest();
	bool PowerSupplyManip(PSU& power_supply);
	bool TerminateTest(const uint32_t image_counter);
	bool RTControlValidation();
	bool NovaWindowSetForeground();
	bool NovaWindowForegroundValidation();
	bool NovaWindowForegroundTimeoutValidator();
	NovaProximity(const Parameters& vars);
	~NovaProximity();
};
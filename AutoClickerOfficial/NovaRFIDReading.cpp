#include "NovaRFIDReading.h"


bool NovaReading::FindNovaWindow()
{
	//Getting Handles - note: go to properties and set multibyte characters in general tab for VS

	log_folder_name = "Automation_Logs_Reading";
	m_nova_foreground_timeout_counter = 0;

	// Create Required Paths
	std::experimental::filesystem::path pathobj(std::experimental::filesystem::current_path());
	exe_folder_path = std::experimental::filesystem::current_path().u8string();
	output_folder_path = pathobj.parent_path().u8string() + "\\" + log_folder_name;
	std::experimental::filesystem::create_directories(output_folder_path);

	hNovaWindow = FindWindow(NULL, m_nova_version.c_str());

	if (hNovaWindow == NULL)
	{
		return false;
	}
	else
	{
		
		// Move window to top left corner in case ALT+Printscreen doesn't work
		SetWindowPos(hNovaWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);
		UpdateWindow(hNovaWindow);
		GetWindowRect(hNovaWindow, &NovaRect);
		return true;
	}
}

bool NovaReading::PowerSupplyManip(PSU& power_supply)
{

	do
	{
		// Make Power Supply topmost window
		power_supply.Active();

		// Power off the power supply
		power_supply.Power_off();

		// Power on the power supply
		power_supply.Power_on();


		// Start RTControl
		power_supply.StartRTControl(exe_folder_path);

		do
		{
			// Make Nova active window
			NovaWindowSetForeground();
			NovaWindowForegroundTimeoutValidator();
			Sleep(2000);

		} while (!NovaWindowForegroundValidation()); // Validate that it is popped


	} while (!RTControlValidation());

	return true;
}

bool NovaReading::NovaWindowForegroundTimeoutValidator()
{
	m_nova_foreground_timeout_counter++;

	if (m_nova_foreground_timeout_counter == 5)
	{
		std::cout << "Attempting a nova click to bring to front" << std::endl;
		ClickObject.LeftClickPos(NovaRect.left + 1, NovaRect.top + 800);
		m_nova_foreground_timeout_counter = 0;
	}

	return true;
}

bool NovaReading::NovaWindowSetForeground()
{
	SetForegroundWindow(hNovaWindow);

	return true;
}

bool NovaReading::NovaWindowForegroundValidation()
{
	HWND foreground_window = GetForegroundWindow();

	if (foreground_window == hNovaWindow)
	{
		std::cout << "Nova window now active" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Nova window not active" << std::endl;
		return false;
	}

}


bool NovaReading::RTControlValidation()
{
	bool rtcontrol_started = false;
	std::vector<std::string> ocr_output_vect;

	// Take ScreenShot
	screen_capture_object.set_image_path(output_folder_path + "\\");
	screen_capture_object.set_ocr_database_path(exe_folder_path);


	for (unsigned int i = 0; i < 2; i++)
	{
		do
		{
			ocr_output = "";
			screen_capture_object.TakeScreenShot("tmp2.jpg", 2);

			try
			{
				ocr_output = screen_capture_object.ProcessImageText("tmp2.jpg", 3);
			}
			catch (...)
			{
				screen_capture_object.TakeScreenShot("tmp.jpg", 1);
				ocr_output = screen_capture_object.ProcessImageText("tmp2.jpg", 3);
			}

			Sleep(500);

		} while (ocr_output.compare("") == 0); // validate that the ocr output is not null (cropping failure due to incorrect active window)

		ocr_output_vect.push_back(ocr_output); // store 2 instances of the ocr output in order to validate that rtcontrol clock incrementing
	}

	std::cout << "First OCR reading: " << ocr_output_vect[0] << std::endl;
	std::cout << "First OCR reading: " << ocr_output_vect[1] << std::endl;

	if (ocr_output_vect[0].compare(ocr_output_vect[1]) != 0)
	{
		std::cout << "Rtcontrol correctly started!" << std::endl;
		rtcontrol_started = true;
	}
	else
	{
		std::cout << "Rtcontrol did not correctly start, powering off again" << std::endl;
	}

	return rtcontrol_started;
}

void NovaReading::Run_Reboot(PSU& power_supply)
{
	
	std::cout << "How many times will the test be run?" << std::endl;

	std::string Seconds;
	int run_number;
	std::cin >> Seconds;

	while (true)
	{
		if (stoi(Seconds) == NULL)
		{
			std::cout << "Incorrect option" << std::endl;
		}
		else
		{
			run_number = stoi(Seconds);
			break;
		}

	}

	m_total_runs = run_number;
	uint32_t total_time = run_number * m_reading_test_duration;

	std::cout << "Please have nova open, navigate to the RFID Testing Page and check the log box if required..." << std::endl;
	for (unsigned int i = m_preperation_time; i > 0; i--)
	{
		std::cout << "Bot starting in: " << i << " seconds" << std::endl;
		Sleep(1000);
	}


	uint32_t image_counter = 1;
	while (run_number != 0)
	{
		if ((run_number % m_reboot_frequency) == 0) // Decide after how many iterations we reboot
		{
			PowerSupplyManip(power_supply);
		}
		
		// Make Nova Active
		SetForegroundWindow(hNovaWindow);
		Sleep(2000);

		//std::cout << "Initializing Log" << std::endl;
		//ClickObject.LeftClickPos(NovaRect.left + 869, NovaRect.top + 397);

		std::cout << "Starting Reading test" << std::endl;
		StartTest();

		//get confirmation of click or else reclick
		std::cout << "Waiting for test to complete!" << std::endl;
		for (int i = m_reading_test_duration; i > 0; i--)
		{
			Sleep(1000);
			std::cout << "Test Run: " << m_total_runs - run_number + 1 << " of " << m_total_runs << " | Time until next test: " << i << " seconds" << " | Tests Failed: " << m_failed_tests << " | Estimated total time remaining: " << total_time << " seconds" << std::endl;
			total_time--;
		}

		TerminateTest(image_counter);


		image_counter++;
		run_number--;
		OutputResultsLogs(output_folder_path);
	}
	
}




void NovaReading::Run()
{


	std::cout << "How many times will the test be run?" << std::endl;

	std::string Seconds;
	int run_number;
	std::cin >> Seconds;

	while (true)
	{
		if (stoi(Seconds) == NULL)
		{
			std::cout << "Incorrect option" << std::endl;
		}
		else
		{
			run_number = stoi(Seconds);
			break;
		}

	}

	m_total_runs = run_number;
	uint32_t total_time = run_number * m_reading_test_duration;

	std::cout << "Please have nova open, navigate to the RFID Testing Page and check the log box if required..." << std::endl;
	for (unsigned int i = m_preperation_time; i > 0; i--)
	{
		std::cout << "Bot starting in: " << i << " seconds" << std::endl;
		Sleep(1000);
	}

	// Make Nova Active
	SetForegroundWindow(hNovaWindow);
	Sleep(2000);

	uint32_t image_counter = 1;
	while (run_number != 0)
	{

		//std::cout << "Initializing Log" << std::endl;
		//ClickObject.LeftClickPos(NovaRect.left + 869, NovaRect.top + 397);

		std::cout << "Starting Reading test" << std::endl;
		StartTest();

		//get confirmation of click or else reclick
		std::cout << "Waiting for test to complete!" << std::endl;
		for (int i = m_reading_test_duration; i > 0; i--)
		{
			Sleep(1000);
			std::cout << "Test Run: " << m_total_runs - run_number + 1 << " of " << m_total_runs << " | Time until next test: " << i << " seconds" << " | Tests Failed: " << m_failed_tests << " | Estimated total time remaining: " << total_time << " seconds" << std::endl;
			total_time--;
		}

		TerminateTest(image_counter);


		image_counter++;
		run_number--;
	}

	OutputResultsLogs(output_folder_path);
}

bool NovaReading::TerminateTest(const uint32_t image_counter)
{
	bool test_terminated = false;
	do
	{

		// Take ScreenShot
		screen_capture_object.set_image_path(output_folder_path + "\\");
		screen_capture_object.set_ocr_database_path(exe_folder_path);

		do
		{
			ocr_output = "";
			screen_capture_object.TakeScreenShot("result_" + std::to_string(image_counter) + ".jpg", 2);

			try
			{
				ocr_output = screen_capture_object.ProcessImageText("result_" + std::to_string(image_counter) + ".jpg", 2);
			}
			catch (...)
			{
				// This works every time
				std::cout << "Attempting full screen image instead of window image - click on window if possible" << std::endl;
				screen_capture_object.TakeScreenShot("result_" + std::to_string(image_counter) + ".jpg", 1);
				ocr_output = screen_capture_object.ProcessImageText("result_" + std::to_string(image_counter) + ".jpg", 2);
			}

		} while (ocr_output.compare("") == 0); // validate that the ocr output is not null (cropping failure due to incorrect active window)


		// Analyze the ocr results to see if its pass or fail and store result
		if (AnalyzeResults(ocr_output, "PASS"))
		{
			test_terminated = true;
			results.push_back(true);
			m_passed_tests++;
		}
		else
		{
			if (AnalyzeResults(ocr_output, "currently running"))
			{
				std::cout << "Test still not complete... waiting an additional 5 seconds..." << std::endl;
				Sleep(5000); //give another 5 seconds
			}
			else
			{
				results.push_back(false);
				m_failed_tests++;
				test_terminated = true;
			}

		}


	} while (!test_terminated);

	return true;

}


bool NovaReading::StartTest()
{
	bool test_started = false;
	std::string tmp_image_location;

	do 
	{
		// Click for the test
		std::cout << "clicking" << std::endl;
		ClickObject.LeftClickPos(NovaRect.left + m_rfid_read_test_offset_x, NovaRect.top + m_rfid_read_test_offset_y);

		Sleep(1000);

		// Check if test has started

		// Set photo paths
		screen_capture_object.set_image_path(output_folder_path + "\\");
		screen_capture_object.set_ocr_database_path(exe_folder_path);

		// Take ScreenShot of application only
		screen_capture_object.TakeScreenShot("tmp.jpg", 2);

		try
		{
			// Try to process image
			ocr_output = screen_capture_object.ProcessImageText("tmp.jpg", 2);
		}
		catch (...) // if failed take full screen image
		{
			screen_capture_object.TakeScreenShot("tmp.jpg", 1);
			ocr_output = screen_capture_object.ProcessImageText("tmp.jpg", 2);
		}

		test_started = AnalyzeResults(ocr_output, "currently running");

	} while (!test_started);

	tmp_image_location = output_folder_path + "\\" + "tmp.jpg";
	remove(tmp_image_location.c_str());
	return true;
}

bool NovaReading::AnalyzeResults(const std::string& ocr_string, const std::string& validation_string)
{
	std::size_t found = ocr_string.find(validation_string);
	if (found != std::string::npos)
	{

		return true;
	}
	else
	{
		return false;
	}

}

void NovaReading::OutputResultsLogs(const std::string output_path)
{
	std::ofstream myfile;
	myfile.open(output_path + "\\Nova_Automation_Log.txt");
	myfile << "Total Number of tests run: " << results.size() << std::endl;
	myfile << "Total Number of tests passed: " << m_passed_tests << std::endl;
	myfile << "Total Number of tests failed: " << m_failed_tests << std::endl;

	myfile << "Result Order:" << std::endl;

	for (unsigned int i = 0; i < results.size(); i++)
	{
		if (results[i])
		{
			myfile << i + 1 << ";" << "1" << std::endl;
		}
		else
		{
			myfile << i + 1 << ";" << "0" << std::endl;
		}
		
	}

	myfile.close();
}

NovaReading::NovaReading(const Parameters& vars)
{
	m_passed_tests = 0;
	m_failed_tests = 0;

	m_settings_vars = vars;

	// Parse parameters
	if (m_settings_vars.variables["version"].compare("") == 0)
	{
		m_nova_version = "NOVA - v3.2.3-3";
		std::cout << "Defaulting to NOVA - v3.2.3-3" << std::endl;
	}
	else
	{
		m_nova_version = "NOVA - " + m_settings_vars.variables["version"];
	}

	if (m_settings_vars.variables["reading_test_duration"].compare("") == 0)
	{
		m_reading_test_duration = 1005;
		std::cout << "Defaulting to reading test duration of 65 seconds" << std::endl;
	}
	else
	{
		m_reading_test_duration = std::stoi(m_settings_vars.variables["reading_test_duration"]);
	}

	if (m_settings_vars.variables["preperation_time"].compare("") == 0)
	{
		m_preperation_time = 10;
		std::cout << "Defaulting to preperation time of 10 seconds" << std::endl;
	}
	else
	{
		m_preperation_time = std::stoi(m_settings_vars.variables["preperation_time"]);
	}

	if (m_settings_vars.variables["reboot_frequency"].compare("") == 0)
	{
		m_reboot_frequency = 1;
		std::cout << "Defaulting to reboot frequency 1 time per iteration" << std::endl;
	}
	else
	{
		m_reboot_frequency = std::stoi(m_settings_vars.variables["reboot_frequency"]);
	}

}

NovaReading::~NovaReading()
{

}
#pragma once
#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <Shlwapi.h>
#include <fstream>
#include <vector>
#include <atlimage.h>
#include <iostream>
#include "OCR.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>

class ScreenCapture {

	std::string image_folder_path;
	std::string ocr_folder_path;

	enum RFIDOptions
	{
		RFID_PROXIMITY_TEST = 1,
		RFID_READING_TEST = 2,
		RTCONTROL_CYCLES = 3
	};

	enum PictureOptions
	{
		FULL_SCREEN = 1,
		APPLICATION_WINDOW = 2
	};

public:
	void TakeScreenShot(const std::string& filename, uint32_t pic_option);
	std::string ProcessImageText(const std::string& image_path, const int& option);

	void set_image_path(const std::string& path);
	void set_ocr_database_path(const std::string& path);

	HWND hNovaWindow;
	RECT NovaRect;
};
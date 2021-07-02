#include "ScreenCapture.h"

void ScreenCapture::TakeScreenShot(const std::string& filename, uint32_t pic_option)
{
	//setting to the screen shot
	if (pic_option == APPLICATION_WINDOW)
	{
		keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY, 0);
		keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
		keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
	else if (pic_option == FULL_SCREEN)
	{
		keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
		keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}

	

	//handler of the bitmap that save the screen shot
	HBITMAP hBitmap;

	//I have to give for it time to make it work
	Sleep(100);

	//take the screen shot
	OpenClipboard(NULL);

	//save the screen shot in the bitmap handler 
	hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);

	//relese the screen shot
	CloseClipboard();

	std::vector<BYTE> buf;
	IStream* stream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
	CImage image;
	ULARGE_INTEGER liSize;

	// screenshot to jpg and save to stream
	image.Attach(hBitmap);
	image.Save(stream, Gdiplus::ImageFormatJPEG);
	IStream_Size(stream, &liSize);
	DWORD len = liSize.LowPart;
	IStream_Reset(stream);
	buf.resize(len);
	IStream_Read(stream, &buf[0], len);
	stream->Release();

	// put the imapge in the file
	std::fstream fi;
	fi.open(image_folder_path + filename, std::fstream::binary | std::fstream::out);
	fi.write(reinterpret_cast<const char*>(&buf[0]), buf.size() * sizeof(BYTE));
	fi.close();
}


std::string ScreenCapture::ProcessImageText(const std::string& image_name, const int& option)
{

	std::string ocr_output;
	cv::Mat image1, image2;

	//Input for the program  
	image1 = cv::imread(image_folder_path + image_name);
	
	// specifies the region of interest in Rectangle form
	cv::Rect crop_region;
	if (option == RFID_PROXIMITY_TEST)
	{
		crop_region.x = 968;
		crop_region.y = 460;
		crop_region.width = 300;
		crop_region.height = 270;
	}
	else if (option == RFID_READING_TEST)
	{
		crop_region.x = 668;
		crop_region.y = 480;
		crop_region.width = 300;
		crop_region.height = 270;
	}
	else if (option == RTCONTROL_CYCLES)
	{
		crop_region.x = 1100;
		crop_region.y = 100;
		crop_region.width = 268;
		crop_region.height = 120;
	}
	
	
	//creates image2 by extracting the specified region of image1
	try
	{
		image2 = image1(crop_region);
	}
	catch (...)
	{
		std::cout << "Failed to crop image - Failed to ProcessImageText" << std::endl;
		return "";
	}
	

	// Window creation - For debugging purposes
	//cv::imshow("input image ", image1);
	//cv::imshow("cropped image ", image2);
	//cv::waitKey(0);

	std::string output_image_name = image_folder_path + "crop.jpg";
	cv::imwrite(output_image_name, image2);

	OCR ocrobj;
	ocrobj.Extract_Text(output_image_name, ocr_folder_path, ocr_output);

	// Cleanup and remove the file
	remove(output_image_name.c_str());

	return ocr_output;

}

void ScreenCapture::set_image_path(const std::string& path)
{
	image_folder_path = path;
}

void ScreenCapture::set_ocr_database_path(const std::string& path)
{
	ocr_folder_path = path;
}
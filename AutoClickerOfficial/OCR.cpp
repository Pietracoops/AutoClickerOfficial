#include "OCR.h"
#include "tesseract/baseapi.h"
#include <leptonica/allheaders.h>
#include <Windows.h>
#include <iostream>

int OCR::Extract_Text(const std::string& image_path, const std::string& database_path, std::string& output_string)
{
    char* outText;

    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(database_path.c_str(), "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix* image = pixRead(image_path.c_str());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    //printf("OCR output:\n%s", outText);

    output_string = outText;

    // Destroy used object and release memory
    api->End();
    delete[] outText;
    pixDestroy(&image);

    return 0;

}
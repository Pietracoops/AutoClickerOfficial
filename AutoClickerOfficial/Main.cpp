#include <iostream>
#include <string>
#include "CoordinateTracker.h"
#include "NovaRFIDProximity.h"
#include "NovaRFIDReading.h"
#include "parameters.h"
#include "PSU.h"

int main()
{
    Parameters params;
	std::cout << "BOT START" << std::endl;

    params.ReadFile();


	std::cout << "Available Options:" << std::endl;
    std::cout << "===========================================" << std::endl;
	std::cout << "1 - Coordinate Tracker:" << std::endl;
	std::cout << "2 - Nova RFID Proximity Bot - NO REBOOT" << std::endl;
    std::cout << "3 - Nova RFID Reading Bot - NO REBOOT" << std::endl;
    std::cout << "4 - Nova RFID Proximity Bot - REBOOT" << std::endl;
    std::cout << "5 - Nova RFID Reading Bot - REBOOT" << std::endl;
    std::cout << "===========================================" << std::endl;

    std::string Option;

    while (true)
    {
        std::cout << "Select option now: ";
        std::cin >> Option;

        try {
            if (stoi(Option) != 1 && stoi(Option) != 2 && stoi(Option) != 3 && stoi(Option) != 4 && stoi(Option) != 5)
            {
                std::cout << "Incorrect option" << std::endl;
            }
            else
            {
                break;
            }
        }
        catch (...)
        {
            std::cout << "Incorrect option" << std::endl;
        }

    }

	std::cout << "Initializing... Please Wait." << std::endl;

    CoordinateTracker CT;
    NovaProximity NovaBotProximity(params);
    NovaReading NovaBotReading(params);
    PSU PowerSupply(params);

    int switchcase = stoi(Option);
    switch (switchcase) {
    case 1:
        std::cout << "Option 1 Selected: Coordinate Tracker" << std::endl;
        CT.Execute();
        break;
    case 2:
        std::cout << "Option 2 Selected: Nova RFID Proximity Bot" << std::endl;
        if (!NovaBotProximity.FindNovaWindow())
        {
            std::cout << "Nova NOT found, please launch Nova and try again!" << std::endl;
            break;
        }
        
        NovaBotProximity.Run();

        break;
    case 3:
        std::cout << "Option 3 Selected: Nova RFID Reading Bot" << std::endl;
        if (!NovaBotReading.FindNovaWindow())
        {
            std::cout << "Nova NOT found, please launch Nova and try again!" << std::endl;
            break;
        }

        NovaBotReading.Run();


        break;
    case 4:
        std::cout << "Option 4 Selected: Nova RFID Proximity Bot with REBOOT" << std::endl;
        if (!NovaBotReading.FindNovaWindow())
        {
            std::cout << "Nova NOT found, please launch Nova and try again!" << std::endl;
            break;
        }
        if (!PowerSupply.Initialize())
        {
            std::cout << "PSCS Software not detected, please launch PSCS software and try again!" << std::endl;
            break;
        }

        NovaBotProximity.Run_Reboot(PowerSupply);

        break;
    case 5:
        std::cout << "Option 5 Selected: Nova RFID Reading Bot with REBOOT" << std::endl;
        if (!NovaBotReading.FindNovaWindow())
        {
            std::cout << "Nova NOT found, please launch Nova and try again!" << std::endl;
            break;
        }
        if (!PowerSupply.Initialize())
        {
            std::cout << "PSCS Software not detected, please launch PSCS software and try again!" << std::endl;
            break;
        }

        NovaBotReading.Run_Reboot(PowerSupply);
        break;
    case 6:
        break;
    case 7:
        break;
    }


    std::cout << "Program Terminated" << std::endl;
    system("pause");
	return 0;
}
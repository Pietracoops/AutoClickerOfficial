#include <iostream>
#include <Windows.h>

#include "CoordinateTracker.h"
using namespace std;






bool CoordinateTracker::FindNovaWindow()
{
	//Getting RS Handles - note: go to properties and set multibyte characters in general tab for VS
	hNovaWindow = FindWindow(NULL, "NOVA - v3.2.3-3");
	hPSUWindow = FindWindow(NULL, "Rating: ");

	if (hNovaWindow == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}

}


void CoordinateTracker::LaunchCTPSU()
{
	while (true)
	{
		GetWindowRect(hPSUWindow, &PSURect);
		Sleep(2000);
		cout << "Get cursor Position" << endl;

		cout << "Nova top Left corner X: " << NovaRect.left << endl;
		cout << "Nova top Left corner Y: " << NovaRect.top << endl;

		if (MOUSEEVENTF_LEFTDOWN)
		{
			GetCursorPos(&p);
			cout << "Cursor Position: X: " << p.x << "  Y: " << p.y << endl;
			cout << "Cursor Difference: X: " << p.x - PSURect.left << "  Y: " << p.y - PSURect.top << endl;
			cout << endl;
		}
	}


}


void CoordinateTracker::LaunchCT()
{
	while (true)
	{
		GetWindowRect(hNovaWindow, &NovaRect);
		Sleep(2000);
		cout << "Get cursor Position" << endl;

		cout << "Nova top Left corner X: " << NovaRect.left << endl;
		cout << "Nova top Left corner Y: " << NovaRect.top << endl;

		if (MOUSEEVENTF_LEFTDOWN)
		{
			GetCursorPos(&p);
			cout << "Cursor Position: X: " << p.x << "  Y: " << p.y << endl;
			cout << "Cursor Difference: X: " << p.x - NovaRect.left << "  Y: " << p.y - NovaRect.top << endl;
			cout << endl;
		}
	}


}

void CoordinateTracker::Execute()
{
	cout << "Launching Coordinate Tracker..." << endl;

	if (FindNovaWindow()) {
		LaunchCT();
	}
	else {
		cout << "Unable to find Nova window" << endl;
	}

}

CoordinateTracker::CoordinateTracker()
{



}
#include <iostream>
#include <Windows.h>
#include <string>
#include "ClickFunctions.h"
using namespace std;


void Click::LeftClick()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Click::RightClick()
{
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}

void Click::LeftClickPos(int XPos, int YPos)
{
	SetCursorPos(XPos, YPos);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Click::LeftDoubleClickPos(int XPos, int YPos)
{
	SetCursorPos(XPos, YPos);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Click::RightClickPos(int XPos, int YPos)
{
	SetCursorPos(XPos, YPos);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}

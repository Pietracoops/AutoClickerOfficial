#ifndef CLICK_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define CLICK_H

//#include <iostream>
//#include <Windows.h>
//#include <string>
class Click {
public:
	void LeftClick();
	void RightClick();
	void LeftClickPos(int XPos, int YPos);
	void LeftDoubleClickPos(int XPos, int YPos);
	void RightClickPos(int XPos, int YPos);
};


#endif
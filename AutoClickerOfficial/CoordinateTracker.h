#include <Windows.h>

class CoordinateTracker {

	HWND hNovaWindow;
	RECT NovaRect;
	HWND hPSUWindow;
	RECT PSURect;
	POINT p;

public:
	bool FindNovaWindow();
	void LaunchCT();
	void LaunchCTPSU();
	void Execute();
	CoordinateTracker();

};



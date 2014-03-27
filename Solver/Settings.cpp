#include "StdAfx.h"
#include "Settings.h"

Settings::Settings(void)
{
	group = 0;
	superGroup = false; 
	solving = false;
	//generator = NULL;
	edit = false;

	lockViewer = false;
	//cubePos = new CubePosition();
}

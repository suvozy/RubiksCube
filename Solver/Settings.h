#pragma once

#include "CubePosition.h"

class Settings
{
public:
	Settings(void);

	int group;	// current cube group
	bool superGroup; // set if centre orientation visible
	bool solving; // set while some solver is busy
	MoveSequence generator;	// movesequence returned from solver
	bool edit;	// set when edit mode, else play mode.

	bool lockViewer;	// set by cubie to disable user interaction on viewer
	// current cube position
	CubePosition cubePos;
};

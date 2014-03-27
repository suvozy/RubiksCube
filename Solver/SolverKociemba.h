#pragma once

#include <iostream>
#include "time.h"
#include "Solver.h"

class SolverKociemba : public Solver
{
private:
	bool search1();
	bool solve2();
	bool search2();

	int gettransEdgeOri( int pos, int m );
	int gettransCornOri(int pos, int m);
	int gettransCornPerm(int pos, int m);
	int gettransEdgePerm( int pos, int m );
	int gettransSliceFull( int pos, int m );
	int gettransSlicePerm( int pos, int m );
	int gettransChoice( int pos, int m );

protected:
	void init();
	bool solve();

public:
	SolverKociemba(void);
	~SolverKociemba(void);

	MoveSequence getGenerator();
	void mix( CubePosition *pCubePos );
	bool setPosition( CubePosition cubePos, bool test );
};

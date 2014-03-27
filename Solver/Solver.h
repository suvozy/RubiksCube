#pragma once

#include "Settings.h"

class Solver
{
protected:
	bool prepared;	// flag is set when tables are prepared
	bool wanttostop;	// flag is set when solver should be stopped soon
	bool running;

	int sollen;	// number of moves in the move list
	int solmoves[40];	// faces of moves performed during search
	int solamount[40];	// exponent of moves performed during search
	int positionlist[40][10];	// encoded positions

	Settings settings;

	// Start/continue search
	virtual bool solve(){return false;}
	// initialise tables
	virtual void init(){}

	void swap(int *pr, int i, int j);
	void cycle(int *pr, int i, int j, int k, int l);
	bool parityOdd(int *pieces, int start, int len);
	void num2perm(int *pieces, int start, int len, int pos);
	void num2partperm(int *pieces, int start, int len, int np, int p0, int pos);
	void num2ori(int *pieces, int start, int len, int val, int pos);
	int ori2num(int *pieces, int start, int len, int val);
	int perm2num(int *pieces, int start, int len);
	int partperm2num( int *perm, int len, int start, int p0, int np );

public:
	Solver(void);
	~Solver(void);

	// Set position to be solved - initialise search if new position
	virtual bool setPosition( CubePosition cubePos, bool test ){return false;}
	// mix cube within this cube group
	virtual void mix( CubePosition cubePos ){}
	// convert movelist into string solution
	virtual MoveSequence getGenerator(){return MoveSequence();}

	void setSettings(Settings s){ settings=s; }
	void run();
	void stopSolving();

	// access function prepared flag
	bool isPrepared();
	bool isRunning();
};

#pragma once

#include <iostream>
#include "time.h"
#include "MoveSequence.h"

class CubePosition
{
private:
	void doMove( int m, int *p0, int *o0, int *f0 );
	void doReflect(int *p0, int *o0, int *f0);
	void doSym( int m, int *p0, int *o0, int *f0 );
	bool checkSym( int m );
	bool mixRest(int pt, int *p0, int *o0, int *f0, int t, bool centres, bool twoCol, bool doOri);
	bool testSym(int *p0,int *o0,int *f0,int t0,bool centres,bool twoCol);
	void getFaceletColors(int *p0, int *o0, int *f0, int *fc, int *fo);

public:
	// internal general cube representation
	// Note that the last six are not really the centres. They actually indicate the
	// colours used on the sides of the whole cube, and also of the other pieces. A
	// permutation of them is hence a recolouring, not a spot pattern. Their
	// orientation is given and used in supergroup solves (0 is solved ori).
	int cubeletPerm[26]; // {0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19, 20,21,22,23,24,25};
	int cubeletOri[26]; // {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0};
	// This is fixed orientation the supergroup markers are supposed to have,
	// relative to the initial fixed choice made. Is used to adjust markers when cube turned
	// and relabeled.
	int faceOri[6]; // {0,0,0,0,0,0};

	int faceletColor[54]; // {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5};


	CubePosition(void);
	~CubePosition(void);

	void reset();
	void resetView();

	void doMove( int m, int q, bool allowRot );
	void doMove( int m );
	void doSequence( MoveSequence ms );
	void doSequence( MoveSequence ms, int l );
	void editMove( int c1, int o1, int c2, int o2 );
	void doSym( int m, bool fixCentres );
	int getSym();
	void mix(int t, bool centres, bool twoCol );

	void getFaceletColors();
};

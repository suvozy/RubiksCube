#pragma once

//#include <cstring>
#include <iostream>

using namespace std;

class MoveSequence
{
private:
	int len;
	int *moves;
	int *amount;

	void simplify();

public:
	MoveSequence(void);
	MoveSequence(int l, int *mvs, int *amt );
	~MoveSequence(void);

	int* getMoves(){return(moves);}
	int* getAmount(){return(amount);}
	int getLength(){return(len);}

	string toString(bool inverse);
	string toString(bool inverse, int pos);
	void parse( string inp, bool inverse );
	void doSym( int s );
};

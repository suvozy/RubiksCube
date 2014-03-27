#include "StdAfx.h"
#include "Solver.h"

Solver::Solver(void)
{
	prepared=false;	// flag is set when tables are prepared
	wanttostop=false;	// flag is set when solver should be stopped soon
	running=false;

	sollen = 0;	// number of moves in the move list
	for(int i=0;i<40;i++) solamount[i]=solmoves[i]=0;
}

Solver::~Solver(void)
{
}

// start thread - either calc tables or solve
void Solver::run()
{
	if( !prepared ){
		init();
		prepared=true;
		//doEvent(0);
	}else if( !settings.solving && !wanttostop){
		settings.solving=true;
		running=true;
		//doEvent(4);
		bool r = solve(); // do it!
		running=false;
		settings.solving=false;
		if( wanttostop ){
			wanttostop = false;
			//doEvent(2);
		}else if( !r ){
			//doEvent(3);
		}else{
			// if found solution, and not interrupted
			settings.generator = getGenerator();
			//doEvent(1);
		}
	}
}

void Solver::stopSolving(){	if(running) wanttostop = true; }

// access function prepared flag
bool Solver::isPrepared(){ return(prepared); }
bool Solver::isRunning(){ return(running); }

// --- General utilities ---
void Solver::swap(int *pr, int i, int j){
	int c=pr[i];
	pr[i]=pr[j];
	pr[j]=c;
}
void Solver::cycle(int *pr, int i, int j, int k, int l){
	int c=pr[i];
	pr[i]=pr[j];
	pr[j]=pr[k];
	pr[k]=pr[l];
	pr[l]=c;
}

// return true if permutation is odd
bool Solver::parityOdd(int *pieces, int start, int len){
	int i,j;
	bool p=false;
	for( i=0;i<len;i++ ){
		for( j=0;j<i;j++ ){
			p^=( pieces[start+i]<pieces[start+j] );
		}
	}
	return(p);
}
// Convert number to permutation
void Solver::num2perm(int *pieces, int start, int len, int pos)
{
	// convert number pos into permutation of 0..len-1 and put it
	// in array pieces[start..start+len-1]
	int i,r;
	int w[]={0,1,2,3,4,5,6,7,8,9,10,11};
	for( i=0; i<len; i++){
		r=pos%(len-i);
		pos=(pos-r)/(len-i);
		pieces[start+i]=w[r];	// use r'th remaining piece
		while( ++r<len ) w[r-1]=w[r];	// remove piece from list
	}
}
// Convert number to partial permutation
void Solver::num2partperm(int *pieces, int start, int len, int np, int p0, int pos)
{
	// convert number pos into permutation of np pieces numbered p0..p0+np-1
	// amongst len places 0..len-1 and put it
	// in array pieces[start..start+len-1]
	int i, j, r;
	for( i=0; i<np; i++){
		r=pos%(len-i);
		pos=(pos-r)/(len-i);
		for(j=start; j<start+len && ( (pieces[j]>=p0 && pieces[j]<p0+np) || r>0); j++){
			if(pieces[j]<p0 || pieces[j]>=p0+np) r--;
		}
		pieces[j]=p0+i;
	}
}
// Convert number to permutation
void Solver::num2ori(int *pieces, int start, int len, int val, int pos)
{
	// convert number pos into orientation of 0..val-1 and put it
	// in array pieces[start..start+len-1]
	int i,j=0,k;
	for( i=0;i<len-1; i++){
		k=pos%val;
		j+=val-k;
		pos=(pos-k)/val;
		pieces[start+i]=k;
	}
	pieces[start+len-1]=j%val;
}

// Convert orientation to number
int Solver::ori2num(int *pieces, int start, int len, int val)
{
	int i,j=0;
	for( i=len-2;i>=0; i--) j=j*val+(pieces[start+i]%val);
	return(j);
}

// Convert full permutation to number
int Solver::perm2num(int *pieces, int start, int len)
{
	// convert permutation of 0..len-1 in array pieces[start..start+len-1]
	// into number
	int i,j,r;
	int p=0;
	for( i=len-1; i>=0; i-- ){
		r=0;
		for( j=i+1; j<len; j++ ){
			if( pieces[start+j]<pieces[start+i]) { r++; }
		}
		p=p*(len-i)+r;
	}
	return p;
}
int Solver::partperm2num( int *perm, int len, int start, int p0, int np )
{
	int i,j,r,pos=0;
	for( i=np-1; i>=0; i--){
		r=0;
		for( j=0; j<len && perm[start+j]!=p0+i; j++ ){
			if( perm[start+j]<p0 || perm[start+j]>p0+i ) r++;
		}
		pos=pos*(len-i)+r;
	}
	return(pos);
}

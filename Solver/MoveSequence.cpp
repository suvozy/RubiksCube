#include "StdAfx.h"
#include "MoveSequence.h"

MoveSequence::MoveSequence(void)
{
}

MoveSequence::~MoveSequence(void)
{
}

MoveSequence::MoveSequence(int l, int *mvs, int *amt )
{
	// given a solution, create generating move sequence
	int i;
	len=l;
	moves = new int[len];
	amount = new int[len];
	for( i=0; i<l; i++ ){
		amount[i]=4-amt[l-1-i];
		moves[i]=mvs[l-1-i];
	}
	simplify();
}


// m=0-5, normal turn
// m=6-8, middle layer turn
// m=9-11, cube turn
// m=12-14, slice move
// m=15-17, anti-slice move
string MoveSequence::toString(bool inverse)
{
	return(toString(inverse,-1));
}
string MoveSequence::toString(bool inverse, int pos)
{
	int i,j,m,di,qtm=0,ftm=0,stm=0,q;
	string sol="";
	if( len==0 ) return("");
	if( inverse ){
		i=len-1; di=-1;
	}else{
		i=0; di=1;
	}
	while( i>=0 && i<len){
		if( (i==pos && di>0) || ( i+1==pos && di<0 ) ){
			sol.append("_ ");
		}
		q=amount[i];m=moves[i];
		if(inverse) q=4-q;
		if(m<6){
			sol += string("LUFRDB")[m];
			j=1;
		}else{
			sol += string("LUF")[m%3];
			sol += string("xxmcsa")[m/3];
			j = (m<9 || m>11) ? 2 : 0;	//j=#faces moved
		}
		if(q>1) sol += string("2'")[q-2];

		if(q==2) qtm+=j;
		qtm+=j; ftm+=j; if(j!=0)stm++;
		i+=di;
		sol.append(" ");
	}
	if( (pos==len && di>0) || ( pos==0 && di<0 ) ){
		sol.append("_ ");
	}
	if( qtm!=0 ){
		//sol += "("+ftm;
		//if(ftm!=qtm) sol += ","+qtm+"q";
		//if(stm!=ftm) sol += ","+stm+"s";
		//sol.append(")");
	}
	return sol;
}

void MoveSequence::parse( string inp, bool inverse ){
	// moves parsed so far
	int mv[80];
	int am[80];
	int ln = 0;

	int c, m=-1, q=-1, t=-1, i=0, p=0;
	while( ( i!=0 || p<inp.length()) && ln<80 ){
		// get next character
		if( p<inp.length() ) c=inp[p]; else c=0;
		if( i==0 ){ // parse next move
			p++;
			m=string("LUFRDBTlufrdbt")[c];
			if( m<0 ) continue;	// ignore bogus character
			if( m>6 ) m-=7;
			if(m==6) m=1;
			i++;
		}else if( i==1 ){ // parse type
			t=string("mcsaMCSA")[c];
			if(t>=4) t-=4;
			if(t>=0) p++; // found type character, skip it.
			i++;
		}else if( i==2 ){
			q=string("1+23'-")[c];
			if( q>=0 ) p++; else q=1;
			if( q>3 ) q=3;
			else if( q<=1 ) q=1;

			// choose canonical face (LUF) in abnormal moves
			if(t>=0 && m>2){
				m-=3;
				if(t!=3) q=4-q;
			}
			// append move to list
			mv[ln]=(t<0) ? m: m+6+3*t;
			am[ln]=q;
			ln++;
			i=0;
		}
	}

	// save result
	len = ln;
	moves = new int[ln];
	amount = new int[ln];
	for( i=0; i<ln; i++ ){
		moves[i] = mv[i];
		amount[i] = am[i];
	}

	simplify();

	if( inverse ){
		// invert whole sequence
		for( i=0, t=len-1; i<t; i++,t--){
			q=moves[i];moves[i]=moves[t];moves[t]=q;
			q=amount[i];amount[i]=4-amount[t];amount[t]=4-q;
		}
		if( i==t) amount[i]=4-amount[i];
	}
}

void MoveSequence::simplify()
{
	int *axis=new int[len];
	int *type=new int[len];
	int i,j,k;
	int turns[3];
	//create list of axes/movetypes
	for(i=0; i<len; i++){
		axis[i]=moves[i]%3;
		type[i]=(moves[i]-axis[i])/3;
	}

	// do simplification on each move
	i=0;
	while(i<len){
		//collect moves on same axis as current move
		turns[0]=turns[1]=turns[2]=0;
		for(j=i; j<len && axis[i]==axis[j]; j++){
			switch(type[j]){
				case 0:	//normal, near face
					turns[0]+=amount[j]; break;
				case 1:	//normal, far face
					turns[2]+=amount[j]; break;
				case 2:	//mid slice
					turns[1]+=amount[j]; break;
				case 3:	// whole cube
					turns[0]+=amount[j];
					turns[1]+=amount[j];
					turns[2]-=amount[j];
					break;
				case 4:	//slice move
					turns[0]+=amount[j];
					turns[2]-=amount[j];
					break;
				case 5:	//anti-slice move
					turns[0]+=amount[j];
					turns[2]+=amount[j];
					break;
			}
		}

		// only one move on this axis, so just leave it
		if( j<=i+1 ) {
			i++;
			continue;
		}

		// simplify
		turns[0]&=3; turns[1]&=3; turns[2]&=3;
		if( turns[0]==0 && turns[1]==0 && turns[2]==0 ){
			// annihilation, ok
		}else if( turns[0]==turns[1] && turns[0]+turns[2]==4 ){
			//cube turn, ok
			amount[i]=turns[0];
			type[i] = 3;
			moves[i]= 9+axis[i];
			i++;
		}else if( turns[1]==0 && turns[2]==0 ){
			//normal, near face, ok
			amount[i]=turns[0];
			type[i] = 0;
			moves[i]= axis[i];
			i++;
		}else if( turns[0]==0 && turns[1]==0 ){
			//normal, far face, ok
			amount[i]=turns[2];
			type[i] = 1;
			moves[i]= 3+axis[i];
			i++;
		}else if( turns[0]==0 && turns[2]==0 ){
			//middle slice, ok
			amount[i]=turns[1];
			type[i] = 2;
			moves[i]= 6+axis[i];
			i++;
		}else if( turns[1]==0 && turns[0]+turns[2]==4 ){
			//slice turn, ok
			amount[i]=turns[0];
			type[i] = 4;
			moves[i]= 12+axis[i];
			i++;
		}else if( turns[1]==0 && turns[0]==turns[2] ){
			//anti-slice turn, ok
			amount[i]=turns[0];
			type[i] = 5;
			moves[i]= 15+axis[i];
			i++;
		}else if( turns[0]==turns[1] ){
			// cube + far face, ok
			//cube turn
			amount[i]=turns[0];
			type[i] = 3;
			moves[i]= 9+axis[i];
			i++;
			// far
			amount[i]=(turns[2]+turns[0])&3;
			type[i] = 1;
			moves[i]= 3+axis[i];
			i++;
		}else if( turns[2]+turns[1]==4 ){
			// cube + near face, ok
			//cube turn
			amount[i]=turns[1];
			type[i] = 3;
			moves[i]= 9+axis[i];
			i++;
			// near
			amount[i]=(turns[0]+turns[2])&3;
			type[i] = 0;
			moves[i]= axis[i];
			i++;
		}else if( turns[2]+turns[0]==4 ){
			// cube + slice, ok
			// cube turn
			amount[i]=turns[1];
			type[i] = 3;
			moves[i]= 9+axis[i];
			i++;
			// slice
			amount[i]=(turns[0]-turns[1])&3;
			type[i] = 4;
			moves[i]= 12+axis[i];
			i++;
		}else if( ((turns[0]-2*turns[1]-turns[2])&3)==0 ){
			// cube + anti-slice, ok
			// cube turn
			amount[i]=turns[1];
			type[i] = 3;
			moves[i]= 9+axis[i];
			i++;
			// anti-slice
			amount[i]=(turns[0]-turns[1])&3;
			type[i] = 5;
			moves[i]= 15+axis[i];
			i++;
		}else if( turns[0]==0 || turns[1]==0 || turns[2]==0 ){
			//2 layer turns
			if( turns[0]!=0 ){
				//near
				amount[i]=turns[0];
				type[i] = 0;
				moves[i]= axis[i];
				i++;
			}
			if( turns[1]!=0 ){
				//middle
				amount[i]=turns[1];
				type[i] = 2;
				moves[i]= 6+axis[i];
				i++;
			}
			if( turns[2]!=0 ){
				//far
				amount[i]=turns[2];
				type[i] = 1;
				moves[i]= 3+axis[i];
				i++;
			}
		}else{
			//2 face turns + cube
			//near
			amount[i]=(turns[0]-turns[1])&3;
			type[i] = 0;
			moves[i]= axis[i];
			i++;
			//cube
			amount[i]=turns[1];
			type[i] = 3;
			moves[i]= 9+axis[i];
			i++;
			//far
			amount[i]=(turns[1]+turns[2])&3;
			type[i] = 1;
			moves[i]= 3+axis[i];
			i++;
		}

		//remove used-up moves
		len-=j-i;
		for( k=i; k<len; k++){
			moves[k]=moves[k+j-i];
			amount[k]=amount[k+j-i];
			axis[k]=axis[k+j-i];
			type[k]=type[k+j-i];
		}
	}
}

int symPerm[29][6]={
	{3,4,5,0,1,2},
	{5,1,3,2,4,0},{0,2,1,3,5,4},{4,3,2,1,0,5},{2,1,0,5,4,3},{0,5,4,3,2,1},{1,0,2,4,3,5},
	{0,4,2,3,1,5},{3,1,2,0,4,5},{0,1,5,3,4,2},
	{2,4,3,5,1,0},{3,2,4,0,5,1},{4,0,5,1,3,2},

	{3,1,5,0,4,2},{0,4,5,3,1,2},{3,4,2,0,1,5},
	{5,1,0,2,4,3},{0,5,1,3,2,4},{1,3,2,4,0,5},
	{5,0,4,2,3,1},{1,2,0,4,5,3},{4,5,0,1,2,3},{5,3,1,2,0,4},
	{5,4,3,2,1,0},{3,2,1,0,5,4},{4,3,5,1,0,2},
	{2,4,0,5,1,3},{3,5,4,0,2,1},{1,0,5,4,3,2}
};
void MoveSequence::doSym( int s ){
	int q,m,t;
	for( int i=0; i<len; i++ ){
		q=amount[i];m=moves[i];
		if(s<13) q=4-q;
		if(m<6){
			m=symPerm[s][m];
		}else{
			t=(m-6)/3;
			m=m%3;
			m=symPerm[s][m];
			if( m>2 ) {
				m-=3;
				if( t<3 ) q=4-q;
			}
			m=6+3*t+m;
		}
		amount[i]=q;moves[i]=m;
	}
}

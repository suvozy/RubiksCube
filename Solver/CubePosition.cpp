#include "StdAfx.h"
#include "CubePosition.h"

CubePosition::CubePosition(void)
{
	srand(time(NULL));

	for(int i = 0; i < 26; i ++)
	{
		cubeletPerm[i] = i; cubeletOri[i] = 0;
	}

	for(int i = 0; i < 6 ; i++) faceOri[i] = 0;

	for(int i = 0; i < 54; i++) faceletColor[i] = i / 9;
}

CubePosition::~CubePosition(void)
{
}

//  reset routines
//reset to start position
// note: does not reset the centre colours or their default markings
void CubePosition::reset()
{
	int j;
	for( j=0; j<20; j++){
		cubeletPerm[j]=j;
		cubeletOri[j]=0;
	}
	for( j=0; j<6; j++){
		cubeletOri[j+20]=0;
	}
}

//reset cube orientation
// reorients the cube so that all their default markings line up
void CubePosition::resetView()
{
	while( faceOri[1]!=0 ) doMove(7);
	if( faceOri[0]==1 || faceOri[0]==3 ) doMove(6);
	else if( faceOri[0]==2 ) { doMove(6);doMove(6); }

	while( faceOri[1]!=0 ) doMove(7);
}

// conversion from move number to action on internal cube representation
// L U F R D B Lc Uc Fc
int movePerm[9][5][26]={
	// piece perm/ori, relabel perm/ori, color perm
	{//L
		{0,1,3,7,4,5,2,6, 8,9,15,11,12,13,10,18,16,17,14,19, 20,21,22,23,24,25},
		{0,0,2,1,0,0,1,2, 0,0,1,0,0,0,1,1,0,0,1,0, 3,0,0,0,0,0},
		{0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0},
		{0,0,0,0,0,0}
	},{//U
		{3,0,1,2,4,5,6,7, 11,8,9,10,12,13,14,15,16,17,18,19, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,3,0,0,0,0},
		{0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0},
		{0,0,0,0,0,0}
	},{//F
		{0,2,6,3,4,1,5,7, 8,14,10,11,12,9,17,15,16,13,18,19, 20,21,22,23,24,25},
		{0,2,1,0,0,1,2,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,3,0,0,0},
		{0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0},
		{0,0,0,0,0,0}
	},{//R
		{1,5,2,3,0,4,6,7, 13,9,10,11,8,16,14,15,12,17,18,19, 20,21,22,23,24,25},
		{2,1,0,0,1,2,0,0, 1,0,0,0,1,1,0,0,1,0,0,0, 0,0,0,3,0,0},
		{0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0},
		{0,0,0,0,0,0}
	},{//D
		{0,1,2,3,5,6,7,4, 8,9,10,11,12,13,14,15,17,18,19,16, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,3,0},
		{0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0},
		{0,0,0,0,0,0}
	},{//B
		{4,1,2,0,7,5,6,3, 8,9,10,12,19,13,14,11,16,17,18,15, 20,21,22,23,24,25},
		{1,0,0,2,2,0,0,1, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,3},
		{0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0},
		{0,0,0,0,0,0}
	},{//Lc
		{4,0,3,7,5,1,2,6, 12,11,15,19, 16,8,10,18, 13,9,14,17, 20,25,21,23,22,24},
		{2,1,2,1,1,2,1,2, 1,1,1,1,1,1,1,1,1,1,1,1, 0,0,0,0,0,0},
		{1,5,6,2,0,4,7,3, 13,17,14,9, 8,16,18,10, 12,19,15,11, 20,21,22,23,24,25},
		{1,2,1,2,2,1,2,1, 1,1,1,1,1,1,1,1,1,1,1,1, 0,0,0,0,0,0},
		{3,2,0,1,0,2}
	},{//Uc
		{3,0,1,2,7,4,5,6, 11,8,9,10,15,12,13,14,19,16,17,18, 22,21,23,25,24,20},
		{0,0,0,0,0,0,0,0, 0,0,0,0,1,1,1,1,0,0,0,0, 0,0,0,0,0,0},
		{1,2,3,0,5,6,7,4, 9,10,11,8,13,14,15,12,17,18,19,16, 20,21,22,23,24,25},
		{0,0,0,0,0,0,0,0, 0,0,0,0,1,1,1,1,0,0,0,0, 0,0,0,0,0,0},
		{0,3,0,0,1,0}
	},{//Fc
		{3,2,6,7,0,1,5,4, 10,14,18,15, 11,9,17,19, 8,13,16,12, 24,20,22,21,23,25},
		{1,2,1,2,2,1,2,1, 1,0,1,0,0,0,0,0,1,0,1,0, 0,0,0,0,0,0},
		{4,5,1,0,7,6,2,3, 16,13,8,12, 19,17,9,11, 18,14,10,15, 20,21,22,23,24,25},
		{2,1,2,1,1,2,1,2, 1,0,1,0,0,0,0,0,1,0,1,0, 0,0,0,0,0,0},
		{3,3,3,3,3,1}
	}
};

//--- move routines ---
// m=0-5, normal turn
// m=6-8, middle layer turn
// m=9-11, cube turn
// m=12-14, slice move
// m=15-17, anti-slice move
// q=+/-1 or +-2.
void CubePosition::doMove( int m, int q, bool allowRot ){
	if(q<0) q+=4;
	while( q>0 ){
		if( m>=15 ){	//anti-slice
			doMove(m-15);
			doMove(m-12);
		}else if(m>=12){	//slice
			doMove(m-12);
			doMove(m-9);doMove(m-9);doMove(m-9);
		}else if(m>=9){		//cube turn
			if(allowRot) doMove(m-3);
		}else if(m>=6){		//middle slice turn
			if(allowRot) doMove(m);	//turn whole cube
			doMove(m-3);	//far side back
			doMove(m-6);doMove(m-6);doMove(m-6); //near side back
		}else{		//normal face turn
			doMove(m);
		}
		q--;
	}
}

void CubePosition::doMove( int m ){
	doMove(m, cubeletPerm, cubeletOri, faceOri);
}
void CubePosition::doMove( int m, int *p0, int *o0, int *f0 )
{
	// m=0..8, does single quarter turn of relevant face, or whole cube
	int i;
	int perm[26], ori[26], fc[6];

	for( i=0; i<26; i++ )
	{
		//do permutation
		perm[i]=p0[ movePerm[m][0][i] ];
		ori[i]= o0[ movePerm[m][0][i] ];
		if(ori[i]>=0) ori[i]+= movePerm[m][1][i];
		//relabel pieces - reorient cube
		if( perm[i]>=0){
			perm[i]= movePerm[m][2][perm[i]];
			if(ori[i]>=0) ori[i]+= movePerm[m][3][perm[i]];
		}
		if(ori[i]>=0){
			if( i<8 ){
				while(ori[i]>2) ori[i]-=3;
			}else if( i<20 ){
				while(ori[i]>1) ori[i]-=2;
			}else{
				while(ori[i]>3) ori[i]-=4;
			}
		}
	}
	for( i=0; i<6; i++ )
	{
		//Permute and adjust face base orientations
		fc[i]= f0[ movePerm[m][0][i+20] -20 ] + movePerm[m][4][i];
		while(fc[i]>3) fc[i]-=4;
	}
	// copy new position to current
	for( i=0; i<26; i++ ){
		p0[i]=perm[i];
		o0[i]=ori[i];
	}
	for( i=0; i<6; i++ ){
		f0[i]=fc[i];
	}
}

void CubePosition::doSequence( MoveSequence ms ){
	doSequence( ms, ms.getLength() );
}
void CubePosition::doSequence( MoveSequence ms, int l ){
	int i;
	reset();
	for(i=0;i<l; i++){
		doMove( ms.getMoves()[i], ms.getAmount()[i], true);
	}
}
void CubePosition::editMove( int c1, int o1, int c2, int o2 ){
	int i;
	if(c1<8 && c2<8){
		// swap corners
		if( c1 == c2 ){
			//change orientation of piece c1;
			cubeletOri[c1]+=o1+3-o2;
			while(cubeletOri[c1]>2)cubeletOri[c1]-=3;
		}else{
			i=cubeletPerm[c1];cubeletPerm[c1]=cubeletPerm[c2];cubeletPerm[c2]=i;
			i=cubeletOri[c1];cubeletOri[c1]=cubeletOri[c2];cubeletOri[c2]=i;
			cubeletOri[c1]+=o2+3-o1;
			cubeletOri[c2]+=o1+3-o2;
			while(cubeletOri[c1]>2)cubeletOri[c1]-=3;
			while(cubeletOri[c2]>2)cubeletOri[c2]-=3;
		}
	}else if(c1>=8 && c2>=8 && c1<20 && c2<20 ){
		// swap edges
		if( c1 == c2 ){
			//change orientation of piece c1;
			cubeletOri[c1]+=o2+o1;
			while(cubeletOri[c1]>1)cubeletOri[c1]-=2;
		}else{
			i=cubeletPerm[c1];cubeletPerm[c1]=cubeletPerm[c2];cubeletPerm[c2]=i;
			i=cubeletOri[c1];cubeletOri[c1]=cubeletOri[c2];cubeletOri[c2]=i;
			cubeletOri[c1]+=o2+o1;
			cubeletOri[c2]+=o1+o2;
			while(cubeletOri[c1]>1)cubeletOri[c1]-=2;
			while(cubeletOri[c2]>1)cubeletOri[c2]-=2;
		}
	}else if(c1>=20 && c2>=20 ){
		// swap centres
		if( c1 == c2 ){
			//change orientation of piece c1;
			if(cubeletOri[c1]<=0) cubeletOri[c1]=3;
			else cubeletOri[c1]--;
		}else{
			i=cubeletPerm[c1];cubeletPerm[c1]=cubeletPerm[c2];cubeletPerm[c2]=i;
			i=cubeletOri[c1];cubeletOri[c1]=cubeletOri[c2];cubeletOri[c2]=i;
		}
	}
}

void CubePosition::doReflect(int *p0, int *o0, int *f0)
{
	int refPerm[]={
		// piece perm/ori, relabel perm/ori, color perm
		6,7,4,5,2,3,0,1, 18,19,16,17,14,15,12,13,10,11,8,9, 23,24,25,20,21,22,
	};

	int i;
	int perm[26], ori[26], fc[6];

	for( i=0; i<26; i++ )
	{
		//do permutation, swapping opposite pieces
		perm[i]=p0[ refPerm[i] ];
		ori[i] =o0[ refPerm[i] ];
		//reverse its orientation
		if( ori[i]>0 ){
			if( i<8 ) ori[i]^=3;
			else if( i>=20 ) ori[i]=4-ori[i];
		}

		//relabel pieces to their opposites
		if( i<20 && perm[i]>=0) perm[i]= refPerm[perm[i]];
	}
	for( i=0; i<6; i++ )
	{
		// swap & mirror opposite base orientations
		fc[i]=4-f0[ refPerm[i+20] -20];
		if( i!=1 && i!=4 ) fc[i]^=2;
	}

	// copy new position to current
	for( i=0; i<26; i++ ){
		p0[i]=perm[i];
		o0[i]=ori[i];
	}
	// copy new face base orientations
	for( i=0; i<6; i++ ) f0[i]=fc[i];
}
void CubePosition::doSym( int m, bool fixCentres ){
	if(fixCentres){
		int saveCentres[6];
		for(int i=0; i<6; i++) saveCentres[i]=cubeletPerm[20+i];
		doSym(m, cubeletPerm, cubeletOri, faceOri);
		for(int i=0; i<6; i++) cubeletPerm[20+i]=saveCentres[i];
	}else{
		doSym(m, cubeletPerm, cubeletOri, faceOri);
	}
}
void CubePosition::doSym( int m, int *p0, int *o0, int *f0 )
{
	// m=0..28, does relevant reflection/rotation
	switch(m){
			case 0:
				doReflect(p0,o0,f0); break;
			case 1: case 2: case 3:
				doSym(26+(m-1),p0,o0,f0);doReflect(p0,o0,f0); break;
			case 4: case 5: case 6:
				doSym(23+(m-4),p0,o0,f0);doReflect(p0,o0,f0); break;
			case 7: case 8: case 9: case 10: case 11: case 12:
				doSym(13+(m-7),p0,o0,f0);doReflect(p0,o0,f0); break;

			case 13: case 14: case 15:
				doSym(16+(m-13),p0,o0,f0);doSym(16+(m-13),p0,o0,f0); break;
			case 16:
				doMove(7,p0,o0,f0); break;
			case 17:
				doMove(6,p0,o0,f0); doMove(6,p0,o0,f0); doMove(6,p0,o0,f0); break;
			case 18:
				doMove(8,p0,o0,f0); break;

			case 19: doMove(6,p0,o0,f0);doMove(7,p0,o0,f0); break;
			case 20: doMove(7,p0,o0,f0);doMove(6,p0,o0,f0); break;
			case 21: doMove(7,p0,o0,f0);doSym(17,p0,o0,f0); break;
			case 22: doMove(7,p0,o0,f0);doMove(8,p0,o0,f0); break;

			case 23: doSym(22,p0,o0,f0);doMove(8,p0,o0,f0); break;
			case 24: doSym(20,p0,o0,f0);doMove(8,p0,o0,f0); break;
			case 25: doSym(22,p0,o0,f0);doSym(17,p0,o0,f0); break;

			case 26: doSym(21,p0,o0,f0);doSym(17,p0,o0,f0); break;
			case 27: doSym(19,p0,o0,f0);doMove(7,p0,o0,f0); break;
			case 28: doSym(20,p0,o0,f0);doMove(7,p0,o0,f0); break;
	}
}

int CubePosition::getSym(){
	int t=0, m=1;
	for( int i=0; i<29; i++){
		if(checkSym(i)) t|=m;
		m<<=1;
	}
	return(t);
}
bool CubePosition::checkSym( int m )
{
	int i;
	int p[26], o[26], f[6];

	for( i=0; i<26; i++ ) {
		p[i]=cubeletPerm[i];
		o[i]=cubeletOri[i];
	}
	for( i=0; i<6; i++ ) f[i]=faceOri[i];
	doSym(m,p,o,f);
	bool ret=true;
	for( i=0; i<20 && ret; i++ ) {
		ret &= (p[i]==cubeletPerm[i]) && (o[i]==cubeletOri[i]);
	}
	return ret;
}

const int pceTypesLength = 9;
static int pceTypes[pceTypesLength]={ 20,26,4, 0,8,3, 8,20,2 };
void CubePosition::mix(int t, bool centres, bool twoCol ){
	int p0[26], o0[26], f0[6];
	int i;
	for(i=0; i<20; i++) p0[i]=o0[i]=-1;
	for(i=20; i<26; i++) {
		f0[i-20]=-1;
		p0[i]=i;
		o0[i]= centres? -1 : 0;
	}

	if( mixRest(0,p0,o0,f0,t,centres,twoCol, false) ){
		for(i=0; i<26; i++){
			if(i<20)cubeletPerm[i]=p0[i];
			cubeletOri[i]=o0[i];
		}
	}else{
		printf("Programming error - no mixed position found\n");
	}
}
bool CubePosition::mixRest(int pt, int *p0, int *o0, int *f0, int t, bool centres, bool twoCol, bool doOri){
	int i,j,fs,ll;
	int *lst;

	//save a copy of position
	int p1[26];
	int o1[26];
	for(i=0; i<26; i++){
		p1[i]=p0[i];
		o1[i]=o0[i];
	}

	// check current position can be extended to right symmetry
	if( testSym(p0,o0,f0,t,centres,twoCol) ) {

		// find first slot
		for(fs=pceTypes[pt]; fs<pceTypes[pt+1] &&
			( (!doOri && p0[fs]>=0) ||
			(doOri && o0[fs]>=0) ); fs++);
		if(fs>=pceTypes[pt+1]){
			if( !doOri ) pt+=3;
			if( pt>=pceTypesLength || mixRest(pt,p0,o0,f0,t,centres,twoCol,doOri) )
				return true;
		}else{
			// make list of all available options for the empty slot
			ll=0;
			if(doOri){
				lst=new int[pceTypes[pt+2]];
				for(i=0; i<pceTypes[pt+2]; i++) lst[ll++]=i;
			}else{
				lst=new int[12];
				for(i=0;i<12;i++)lst[i]=0;
				//build full list and...
				for(i=pceTypes[pt]; i<pceTypes[pt+1]; i++) lst[ll++]=i;
				//...remove used pieces
				for(i=pceTypes[pt]; i<pceTypes[pt+1]; i++){
					if(p0[i]>=0){
						for(j=0; j<ll && lst[j]!=p0[i]; j++);
						lst[j]=lst[ll-1];
						ll--;
					}
				}
			}

			// try each piece
			while( ll>0 ){
				// choose random piece
				i=(int)(rand() % 2);
				if(doOri){
					o0[fs]=lst[i];
				}else{
					p0[fs]=lst[i];
				}
				// try to complete mix
				if( mixRest(pt,p0,o0,f0,t,centres,twoCol,!doOri) ) return true;
				//failed, remove piece from list
				ll--;
				lst[i]=lst[ll];
			}
		}
	}

	// all fails. restore position
	for(i=0; i<26; i++){
		p0[i]=p1[i];
		o0[i]=o1[i];
	}
	return false;
}

bool CubePosition::testSym(int *p0,int *o0,int *f0,int t0,bool centres,bool twoCol){
	int p1[26], o1[26];
	int i,j,k;
	bool tryAgain;
	do{
		tryAgain=false;
		for( i=0; i<29; i++){
			if( (t0&(1<<i))!=0 ){
				//make copy
				for(j=0; j<26; j++){
					p1[j]=p0[j];
					o1[j]=o0[j];
				}
				//apply symmetry to one of them
				doSym(i,p0,o0,f0);
				//merge the two / check compatible
				for(j=0; j<26; j++){
					if(p0[j]>=0){
						if( p1[j]>=0 && p1[j]!=p0[j] && j<20 ){
							//conflict
							return false;
						}
					}else if(p1[j]>=0){
						// merge it
						p0[j]=p1[j];
						tryAgain=true;
					}
					if(o0[j]>=0){
						if( o1[j]>=0 && o1[j]!=o0[j]){
							//conflict
							return false;
						}
					}else if(o1[j]>=0){
						// merge it
						o0[j]=o1[j];
						tryAgain=true;
					}
				}
			}
		}
	}while(tryAgain);
	// position has right symmetry, but check if it is valid position
	// first check for duplicate pieces
	for(i=0; i<20; i++){
		if(p0[i]>=0){
			for(j=i+1; j<20; j++){
				if(p0[i]==p0[j]) return false;
			}
		}
	}
	// now check corner orientation
	j=0;
	for(i=0; i<8 && o0[i]>=0; i++) j+=o0[i];
	if( i>=8 && (j%3)!=0 ) return false;

	// now check edge orientation
	j=0;
	for(i=8; i<20 && o0[i]>=0; i++) j+=o0[i];
	if( i>=20 && (j&1)!=0 ) return false;

	// check piece parity
	tryAgain=false;
	for(i=0; i<20; i++){
		if(p0[i]<0) break;
		for(j=i+1;j<20;j++){
			if( p0[j]<p0[i] )tryAgain=!tryAgain;
		}
	}
	if(i>=20 && tryAgain) return false;

	// now check center orientation
	// first get corner parity
	if(centres){
		tryAgain=false;
		for(i=0; i<8; i++){
			for(j=i+1;j<8;j++){
				if( p0[j]<p0[i] )tryAgain=!tryAgain;
			}
		}
		for(i=20; i<26 && o0[i]>=0; i++)
			if((o0[i]&1)!=0) tryAgain=!tryAgain;
		if( i>=26 && tryAgain ) return false;
	}

	// finally check for two colours per face
	if( twoCol ){
		int fc[54], fo[54];
		for(i=0;i<54;i++) fc[i]=fo[i]=0;
		getFaceletColors(p0, o0, f0, fc, fo);
		//check each face
		int lst[7];
		for(i=0;i<7;i++)lst[i]=0;
		int ll;
		for(i=0; i<54; i+=9){
			ll=0;
			for(j=i; j<i+9; j++){
				if( fc[j]>=0){
					for(k=0; k<ll && lst[k]!=fc[j]; k++);
					if(k>=ll) lst[ll++]=fc[j];
				}
			}
			if(ll>2) return false;
		}
	}
	return true;
}


int faceletOri[54];
static int faceletOriDiff[] = {
	0,0,1,3,0,1,3,2,2,  0,0,1,3,0,1,3,2,2,  0,0,1,3,0,1,3,2,2,
	0,0,1,3,0,1,3,2,2,  0,0,1,3,0,1,3,2,2,  0,0,1,3,0,1,3,2,2
};

static int cubeletColors[][3] = {
	{1,5,3},{1,3,2},{1,2,0},{1,0,5},
	{4,3,5},{4,2,3},{4,0,2},{4,5,0},
	{1,3,-1},{1,2,-1},{1,0,-1},{1,5,-1},
	{3,5,-1},{3,2,-1},{0,2,-1},{0,5,-1},
	{4,3,-1},{4,2,-1},{4,0,-1},{4,5,-1},
};
static int cubelet2facelet[][3] = {
	{11,45,29},{17,27,20},{15,18,2},{9,0,47},
	{44,35,51},{38,26,33},{36,8,24},{42,53,6},
	{14,28,-1},{16,19,-1},{12,1,-1},{10,46,-1},
	{32,48,-1},{30,23,-1},{5,21,-1},{3,50,-1},
	{41,34,-1},{37,25,-1},{39,7,-1},{43,52,-1},
	{ 4,-1,-1},{13,-1,-1},{22,-1,-1},{31,-1,-1},{40,-1,-1},{49,-1,-1}
};

// convert internal cube representation to cube facelets colours
// and facelet marking orientation
void CubePosition::getFaceletColors(){
	getFaceletColors(cubeletPerm,cubeletOri,faceOri,faceletColor,faceletOri);
}
void CubePosition::getFaceletColors(int *p0, int *o0, int *f0, int *fc, int *fo)
{
	int i,j,k,o,c;

	for(i=0; i<54; i++) fc[i]=-1;
	for(i=0; i<20; i++){
		//get cubelet that is in position i
		k=p0[i];
		o=o0[i];
		if( k>=0 && o>=0 ){
			for( j=0; j<3 && (j<2 || i<8); j++){

				//get facelet color on each side
				c=cubeletColors[k][o];
				//color the correct cube facelet
				fc[ cubelet2facelet[i][j] ] = c;

				//get the orientation the facelet has in its home position
				c=f0[c];
				//adjust by movement to current position
				c+= faceletOriDiff[ cubelet2facelet[k][o] ]-faceletOriDiff[ cubelet2facelet[i][j] ];
				c&=3;
				//save the facelet orientation
				fo[ cubelet2facelet[i][j] ] = c;

				o=( o==2 || (o==1 && i>=8)) ? 0 : o+1;
			}
		}
	}
	for(i=20; i<26; i++){
		//get cubelet that is in position i
		o=o0[i];

		//color the centre facelet
		fc[ cubelet2facelet[i][0] ] = i-20;
		if( o>=0 ){
			//get the orientation the facelet should have
			c=f0[i-20];
			// adjust by current orientation
			c+=o;
			c&=3;
			//save the facelet orientation
			fo[ cubelet2facelet[i][0] ] = c;
		}
	}
}

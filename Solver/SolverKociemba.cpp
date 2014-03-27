#include "StdAfx.h"
#include "SolverKociemba.h"

SolverKociemba::SolverKociemba(void)
{
	srand(time(NULL));
}

SolverKociemba::~SolverKociemba(void)
{
}

//position = cornerperm 40320, cornerori 2187, edgeori 2048,
//           midslicefull 11880=12!/8! for all three slices
//             in phase 2 two slices become one set 40320.
//           centresori 4096 (redundancy factor of 2)
//Pruning:
//phase 1. orientations 2^11=2048, 3^7=2187, slicechoice:12C4=495
//phase 2. corners 8!=40320, edgesUD 8!=40320, edgesM 4!=24
int transEdgeOri[2048][6];
int transCornOri[2187][6];
int transCornPerm[40320][6];
int transEdgePerm[40320][6];
int transSliceFull[11880][6];
int transChoice[2048][6];
int transSlicePerm[24][6];
int transFace[4096][6];

char pruneEdgeOri[2048];
char pruneCornOri[2187];
char pruneChoice[2048];
char pruneCornPerm[40320];
char pruneEdgePerm[40320];
char pruneSlicePerm[24];
char pruneFace1[4096];
char pruneFace2[4096];
int phase1len=0;	// current search depth for phase1
int phase2len=0;	// current search depth for phase2
int maxdepth = 25;	// length of best solution found (phase1+phase2)

MoveSequence SolverKociemba::getGenerator()
{
	return( MoveSequence( sollen, solmoves, solamount ) );
}

void SolverKociemba::mix( CubePosition *cubePos )
{
	int i;
	// direct method, not random moves
	((*cubePos)).reset();
	do{
		num2perm( (*cubePos).cubeletPerm, 0, 8, (int)(40320*( (float)(rand() % 100) / 100 )) );
		num2perm( (*cubePos).cubeletPerm, 8,12, (int)(479001600*( (float)(rand() % 100) / 100 )));
		for( i=8; i<20; i++) (*cubePos).cubeletPerm[i]+=8;
	}while( parityOdd((*cubePos).cubeletPerm,0,20) );

	num2ori( (*cubePos).cubeletOri, 0,8,3, (int)(2187*( (float)(rand() % 100) / 100 )) );
	num2ori( (*cubePos).cubeletOri, 8,12,2, (int)(2048*( (float)(rand() % 100) / 100 )) );
	num2ori( (*cubePos).cubeletOri,20,6,4, (int)(1024*( (float)(rand() % 100) / 100 )) );
	if( ( (float)(rand() % 100) / 100 )<.5 ) (*cubePos).cubeletOri[25]^=2;
	if( parityOdd((*cubePos).cubeletPerm,0,8) ) (*cubePos).cubeletOri[25]^=1;
}

bool SolverKociemba::setPosition( CubePosition cubePos, bool test ){
	int i,j=0;
	for( i=0;i<8;i++ ){
		j+=cubePos.cubeletOri[i]; if( j>2 )j-=3;
	}
	if( j!=0 ) return(false);
	j=0;
	for( i=8;i<20;i++ ){
		j+=cubePos.cubeletOri[i]; if( j>1 )j-=2;
	}
	if( j!=0 )return(false);
	if( settings.superGroup ){
		bool f=parityOdd( cubePos.cubeletPerm,0,8);
		for( i=20;i<26;i++ ){
			if((cubePos.cubeletOri[i]&1)!=0) f=!f;
		}
		if( f )return(false);
	}
	if( parityOdd( cubePos.cubeletPerm,0,20 ))return(false);

	if( test ) return(true);
	if( !prepared ) return(false);

	// convert position to numbers
	int newpos[8];
	newpos[0]=0; for( i=10;i>=0; i--) newpos[0]=newpos[0]*2+cubePos.cubeletOri[8+i];
	newpos[1]=0; for( i=6;i>=0; i--) newpos[1]=newpos[1]*3+cubePos.cubeletOri[i];
	newpos[2]=perm2num(cubePos.cubeletPerm,0,8);
	newpos[3]=partperm2num( cubePos.cubeletPerm, 12, 8, 8, 4 );
	newpos[4]=partperm2num( cubePos.cubeletPerm, 12, 8, 12, 4 );
	newpos[5]=partperm2num( cubePos.cubeletPerm, 12, 8, 16, 4 );
	newpos[6]=0; for( i=10;i>=0; i--) newpos[6]=newpos[6]*2+( (cubePos.cubeletPerm[8+i]>=12 && cubePos.cubeletPerm[8+i]<16)?1:0);
	if( settings.superGroup ){
		newpos[7]=0; for( i=20;i<26; i++) newpos[7]=newpos[7]*4+cubePos.cubeletOri[i];
	}else{
		newpos[7]=-1;
	}

	// clear search history if are using a different position
	//if( positionlist == NULL ){
	//	for(int i = 0; i < 40; i++) for(int j = 0; j < 10; j ++) positionlist[i][j] = 0;
	//	sollen=phase1len=phase2len=0;
	//	maxdepth=25;
	//	solmoves[0]=-1;
	//	solamount[0]=3;
	//}else 
	//	if( positionlist[0][0]!=newpos[0] ||
	//	positionlist[0][1]!=newpos[1] ||
	//	positionlist[0][2]!=newpos[2] ||
	//	positionlist[0][3]!=newpos[3] ||
	//	positionlist[0][4]!=newpos[4] ||
	//	positionlist[0][5]!=newpos[5] ||
	//	positionlist[0][6]!=newpos[6] ||
	//	positionlist[0][9]!=newpos[7] ){
	for(i=0;i<40;i++) for(j=0;j<10;j++) positionlist[i][j]=0;
			sollen=phase1len=phase2len=0;
			maxdepth=25;
			solmoves[0]=-1;
			solamount[0]=3;
	//}
	positionlist[0][0]=newpos[0];	// edge ori
	positionlist[0][1]=newpos[1];	// corn ori
	positionlist[0][2]=newpos[2];	// corn perm
	positionlist[0][3]=newpos[3];	// edge perm1
	positionlist[0][4]=newpos[4];	// edge perm2
	positionlist[0][5]=newpos[5];	// edge perm3
	positionlist[0][6]=newpos[6];	// choice
	positionlist[0][9]=newpos[7];	// face centres
	return(true);
}

bool SolverKociemba::solve()
{
	// do IDA* - search depth first for each phase1len.

	// quit if no better solution possible
	if( phase1len>=maxdepth ) return(false);

	printf("p1 len=%d\n",phase1len);
	while( !search1() ){
		if( wanttostop ) return(false);
		phase1len++;
		if( phase1len>=maxdepth ) return(false);
		printf("p1 len=%d\n",phase1len);
	}
	// found a solution
	maxdepth = sollen-1;
	return(true);
}
bool SolverKociemba::search1()
{
	//do a depth search through all phase1 positions at depth phase1len.
	// return false if no solution, or break out with true if solved.
	int m,nxt;

	// Check if had already completed phase 1 section
	if( sollen>=phase1len ){
		// if have phase1 solution at end of phase1, then try alternative phase2 solution first
		if( positionlist[phase1len][0]==0 &&
			positionlist[phase1len][1]==0 &&
			positionlist[phase1len][6]==240 &&
			(! settings.superGroup || (positionlist[phase1len][9]&0x451)==0 ) )
		{
			// find next phase2 solution if possible
			if( solve2() ) return(true);
			// no alternative phase 2 solution found. continue with phase1
		}
		sollen=phase1len;
	}

	while( sollen>=0 ){
		// at this point positionlist[0..sollen] are visited positions
		// and solmoves[0..sollen-1] are moves performed.
		// Note solmoves[sollen] is last tried (rejected) next move
		nxt=sollen+1;
		// add extra quarter turn to last move
		m = solmoves[sollen];
		if( m>=0 ){ // ignore dummy -1 move
			positionlist[nxt][0]=transEdgeOri[positionlist[nxt][0]][m];
			positionlist[nxt][1]=transCornOri[positionlist[nxt][1]][m];
			positionlist[nxt][2]=transCornPerm[positionlist[nxt][2]][m];
			positionlist[nxt][3]=transSliceFull[positionlist[nxt][3]][m];
			positionlist[nxt][4]=transSliceFull[positionlist[nxt][4]][m];
			positionlist[nxt][5]=transSliceFull[positionlist[nxt][5]][m];
			positionlist[nxt][6]=transChoice[positionlist[nxt][6]][m];
			if( settings.superGroup )
				positionlist[nxt][9]=transFace[positionlist[nxt][9]][m];
		}else{
			positionlist[nxt][0]= positionlist[sollen][0];
			positionlist[nxt][1]= positionlist[sollen][1];
			positionlist[nxt][2]= positionlist[sollen][2];
			positionlist[nxt][3]= positionlist[sollen][3];
			positionlist[nxt][4]= positionlist[sollen][4];
			positionlist[nxt][5]= positionlist[sollen][5];
			positionlist[nxt][6]= positionlist[sollen][6];
			positionlist[nxt][9]= positionlist[sollen][9];
		}
		solamount[sollen]++;
		// if done full turn, then move to next face
		if( solamount[sollen]>3 ){
			// next face, 0 turns yet
			solamount[sollen]=0;
			do{
				solmoves[sollen]++;
			}while( sollen!=0 && ( solmoves[sollen]==solmoves[sollen-1] || solmoves[sollen]==solmoves[sollen-1]+3 ));
			if( solmoves[sollen]>=6 ){
				// done all faces. backtrack
				sollen--;
				continue;
			}
			continue; // loop back to do a quarter turn of this new face
		}

		// check pruning for proposed new position
		if( sollen+pruneEdgeOri[positionlist[nxt][0]]<phase1len+1 &&
			sollen+pruneCornOri[positionlist[nxt][1]]<phase1len+1 &&
			sollen+pruneChoice[positionlist[nxt][6]]<phase1len+1 &&
			(!settings.superGroup || sollen+pruneFace1[positionlist[nxt][9]]<phase1len+1 )
			){
				//its ok, officially add to movelist
				// append dummy move to list for later extension.
				solmoves[nxt]=-1;
				solamount[nxt]=3;
				sollen=nxt;
				// check if have found phase1 solution of required length;
				if( sollen >= phase1len ){
					// find phase2 solution if possible
					if( solve2() ) return(true);
					// no phase 2 solution found. continue with phase1
					//sollen=phase1len;
				}
		}
		if( wanttostop ) return(false);
		//loop back to shift to next move sequence
	}
	// reset for next search
	solmoves[0]=-1;
	solamount[0]=3;
	sollen=0;
	return(false);
}
bool SolverKociemba::solve2()
{
	// should not search further if last phase1 move
	//   could have been done in phase 2
	if( sollen>0 ){
		if( solamount[sollen-1]==2 || solmoves[sollen-1]==1 || solmoves[sollen-1]==4 ) return(false);
	}
	// quick prune on corners before doing position conversion
	if( phase1len+pruneCornPerm[positionlist[phase1len][2]] > maxdepth+1 ||
		(settings.superGroup && phase1len+pruneFace2[positionlist[phase1len][9]] > maxdepth+1 ))
	{
		return(false);
	}

	// get depth to start phase 2 search with
	phase2len = sollen - phase1len;
	if( sollen>maxdepth ){
		phase2len = maxdepth - phase1len;
	}

	// if just finished phase1, then convert phase1 end position
	// to phase2 start position
	if( phase2len==0 ){
		// check if already finished
		if( positionlist[phase1len][2]==0 &&
			positionlist[phase1len][3]==0 &&
			positionlist[phase1len][4]==5860 &&
			positionlist[phase1len][5]==11720 &&
			positionlist[phase1len][9]<=0 ) return(true);

		//convert edge pos numbers into full edge permutation
		int edges[]={-1,-1,-1,-1,  -1,-1,-1,-1,  -1,-1,-1,-1 };
		num2partperm(edges, 0, 12, 4, 0, positionlist[phase1len][3] );
		num2partperm(edges, 0, 12, 4, 4, positionlist[phase1len][4] );
		num2partperm(edges, 0, 12, 4, 8, positionlist[phase1len][5] );

		// check that face centres can be solved given this middle slice
		if( settings.superGroup ){
			bool par= parityOdd(edges,4,4);
			if( ( positionlist[phase1len][9]&   2)!=0 ) par=!par;
			if( ( positionlist[phase1len][9]&  32)!=0 ) par=!par;
			if( ( positionlist[phase1len][9]& 128)!=0 ) par=!par;
			if( ( positionlist[phase1len][9]&2048)!=0 ) par=!par;
			if( par ) return(false);
		}

		// convert middle slice to number
		positionlist[phase1len][8] = perm2num(edges, 4, 4);
		edges[4]=edges[8]; edges[5]=edges[9];
		edges[6]=edges[10]; edges[7]=edges[11];
		// convert UD edges to number
		positionlist[phase1len][7] = perm2num(edges, 0, 8);

	}

	// do IDA* - search depth first for each phase2len.
	printf("p2 len=%d\n",phase2len);
	while( !search2() ){
		phase2len++;
		if( phase1len+phase2len>maxdepth ) return(false);
		printf("p2 len=%d\n",phase2len);
	}
	return(true);
}
bool SolverKociemba::search2()
{
	//do a phase2 depth search through all positions at depth phase2len.
	// return false if no solution, or break out with true if solved.
	int m,nxt;
	while( sollen>=phase1len ){
		// at this point positionlist[0..sollen] are visited positions
		// and solmoves[0..sollen-1] are moves performed.
		// Note solmoves[sollen] is last tried (rejected) next move
		nxt=sollen+1;
		// add extra turn to last move
		m = solmoves[sollen];
		if( m==1 || m==4 ){ // U or D move
			positionlist[nxt][2]=transCornPerm[positionlist[nxt][2]][m];
			positionlist[nxt][7]=transEdgePerm[positionlist[nxt][7]][m];
			// positionlist[nxt][8]=transSlicePerm[positionlist[nxt][8]][m];  // slice is unaffected anyway
			if( settings.superGroup )
				positionlist[nxt][9]=transFace[positionlist[nxt][9]][m];
		}else if( m>=0 ){
			positionlist[nxt][2]=transCornPerm[transCornPerm[positionlist[nxt][2]][m]][m];
			positionlist[nxt][7]=transEdgePerm[positionlist[nxt][7]][m];
			positionlist[nxt][8]=transSlicePerm[positionlist[nxt][8]][m];
			if( settings.superGroup )
				positionlist[nxt][9]=transFace[transFace[positionlist[nxt][9]][m]][m];
		}else{ // ignore dummy -1 move
			positionlist[nxt][2]= positionlist[sollen][2];
			positionlist[nxt][7]= positionlist[sollen][7];
			positionlist[nxt][8]= positionlist[sollen][8];
			positionlist[nxt][9]= positionlist[sollen][9];
		}
		solamount[sollen]+= (m==1 || m==4)? 1:2;
		// if done full turn, then move to next face
		if( solamount[sollen]>3 ){
			// next face, 0 turns yet
			solamount[sollen]=0;
			do{
				solmoves[sollen]++;
			}while( sollen!=0 && ( solmoves[sollen]==solmoves[sollen-1] || solmoves[sollen]==solmoves[sollen-1]+3 ));
			if( solmoves[sollen]>=6 ){
				// done all faces. backtrack
				sollen--;
				continue;
			}
			continue; // loop back to do a turn of this new face
		}

		// check pruning for proposed new position
		if( sollen+pruneEdgePerm[positionlist[nxt][7]]<phase1len+phase2len+1 &&
			sollen+pruneCornPerm[positionlist[nxt][2]]<phase1len+phase2len+1 &&
			sollen+pruneSlicePerm[positionlist[nxt][8]]<phase1len+phase2len+1 &&
			( !settings.superGroup || sollen+pruneFace2[positionlist[nxt][9]]<phase1len+phase2len+1) ){
				//its ok, officially add to movelist
				// append dummy move to list for later extension.
				solmoves[nxt]=-1;
				solamount[nxt]=3;
				sollen=nxt;
				// check if have found phase2 solution of required length;
				if( sollen >= phase1len+phase2len ) return(true);
		}

		if( wanttostop ) return(false);
		//loop back to shift to next move sequence
	}
	// reset for next search
	solmoves[phase1len]=-1;
	solamount[phase1len]=3;
	sollen=phase1len;
	return(false);
}

void SolverKociemba::init(){
	int i,m;

	for(i=0;i<2048;i++)pruneEdgeOri[i]=pruneChoice[i]=0;
	for(i=0;i<2187;i++)pruneCornOri[i]=0;
	for(i=0;i<2048;i++)pruneCornPerm[i]=pruneEdgePerm[i]=0;
	for(i=0;i<24;i++)pruneSlicePerm[i]=0;
	for(i=0;i<4096;i++)pruneFace1[i]=pruneFace2[i]=0;

	// calc transition tables
	for( i=0; i<2048; i++){	// edge orientation transitions
		for( m=0;m<6;m++ ) transEdgeOri[i][m]=gettransEdgeOri(i,m);
	}
	for( i=0; i<2187; i++){	// corner orientation transitions
		for( m=0;m<6;m++ ) transCornOri[i][m]=gettransCornOri(i,m);
	}
	for( i=0; i<40320; i++){	// corner permutation transitions
		for( m=0;m<6;m++ ) transCornPerm[i][m]=gettransCornPerm(i,m);
	}
	for( i=0; i<40320; i++){	// edge permutation transitions
		for( m=0;m<6;m++ ) transEdgePerm[i][m]=gettransEdgePerm(i,m);
	}
	for( i=0; i<11880; i++){	// slice permutation transitions
		for( m=0;m<6;m++ ) transSliceFull[i][m]=gettransSliceFull(i,m);
	}
	for( i=0; i<2048; i++){	// slice choice transitions
		for( m=0;m<6;m++ ) transChoice[i][m]=gettransChoice(i,m);
	}
	for( i=0; i<24; i++){	// slice permutation transitions
		for( m=0;m<6;m++ ) transSlicePerm[i][m]=gettransSlicePerm(i,m);
	}

	for( i=0; i<4096; i++){	// face centre orientation transitions
		transFace[i][0]=((i+(3<<10))&((1<<12)-1));
		transFace[i][1]=((i+(3<<8 ))&((1<<10)-1))+(i&((1<<12)-(1<<10)));
		transFace[i][2]=((i+(3<<6 ))&((1<<8 )-1))+(i&((1<<12)-(1<<8 )));
		transFace[i][3]=((i+(3<<4 ))&((1<<6 )-1))+(i&((1<<12)-(1<<6 )));
		transFace[i][4]=((i+(3<<2 ))&((1<<4 )-1))+(i&((1<<12)-(1<<4 )));
		transFace[i][5]=((i+(3    ))&((1<<2 )-1))+(i&((1<<12)-(1<<2 )));
	}


	//calculate pruning tables. First edge orientation
	int p,q,k;
	int l=1;
	pruneEdgeOri[0]=1;
	do{
		k=0;
		for(i=0;i<2048;i++){
			if( pruneEdgeOri[i]==l){
				for(m=0;m<6;m++){
					p=i;
					for(q=0;q<3;q++){
						p=transEdgeOri[p][m];
						if( pruneEdgeOri[p]==0){
							pruneEdgeOri[p]=(char)(l+1);
							k++;
						}
					}
				}
			}
		}
		l++;
	}while(k!=0);

	// corner orientation pruning
	l=1;
	pruneCornOri[0]=1;
	do{
		k=0;
		for(i=0;i<2187;i++){
			if( pruneCornOri[i]==l){
				for(m=0;m<6;m++){
					p=i;
					for(q=0;q<3;q++){
						p=transCornOri[p][m];
						if( pruneCornOri[p]==0){
							pruneCornOri[p]=(char)(l+1);
							k++;
						}
					}
				}
			}
		}
		l++;
	}while(k!=0);

	// slice choice pruning
	l=1;
	pruneChoice[240]=1;
	do{
		k=0;
		for(i=0;i<2048;i++){
			if( pruneChoice[i]==l){
				for(m=0;m<6;m++){
					p=i;
					for(q=0;q<3;q++){
						p=transChoice[p][m];
						if( pruneChoice[p]==0){
							pruneChoice[p]=(char)(l+1);
							k++;
						}
					}
				}
			}
		}
		l++;
	}while(k!=0);

	// corner perm pruning
	l=1;
	pruneCornPerm[0]=1;
	do{
		k=0;
		for(i=0;i<40320;i++){
			if( pruneCornPerm[i]==l){
				for(m=0;m<6;m++){
					p=i;
					for(q=0;q<3;q++){
						p=transCornPerm[p][m];
						if( m==1 || m==4 || q==1 ){
							if( pruneCornPerm[p]==0){
								pruneCornPerm[p]=(char)(l+1);
								k++;
							}
						}
					}
				}
			}
		}
		l++;
	}while(k!=0);

	// edge perm pruning
	l=1;
	pruneEdgePerm[0]=1;
	do{
		k=0;
		for(i=0;i<40320;i++){
			if( pruneEdgePerm[i]==l){
				for(m=0;m<6;m++){
					p=i;
					for(q=0;q<3;q++){
						p=transEdgePerm[p][m];
						if( pruneEdgePerm[p]==0){
							pruneEdgePerm[p]=(char)(l+1);
							k++;
						}
					}
				}
			}
		}
		l++;
	}while(k!=0);

	// slice perm pruning
	l=1;
	pruneSlicePerm[0]=1;
	do{
		k=0;
		for(i=0;i<24;i++){
			if( pruneSlicePerm[i]==l){
				for(m=0;m<6;m++){
					p=i;
					for(q=0;q<3;q++){
						p=transSlicePerm[p][m];
						if( pruneSlicePerm[p]==0){
							pruneSlicePerm[p]=(char)(l+1);
							k++;
						}
					}
				}
			}
		}
		l++;
	}while(k!=0);

	// face centre orientation pruning table
	for( i=0; i<4096; i++){
		pruneFace1[i]=1;
		if((i&   1)!=0) pruneFace1[i]++;
		if((i&  16)!=0) pruneFace1[i]++;
		if((i&  64)!=0) pruneFace1[i]++;
		if((i&1024)!=0) pruneFace1[i]++;
		pruneFace2[i]=1;
		if((i&   3)!=0) pruneFace2[i]++;
		if((i&  12)!=0) pruneFace2[i]++;
		if((i&  48)!=0) pruneFace2[i]++;
		if((i& 192)!=0) pruneFace2[i]++;
		if((i& 768)!=0) pruneFace2[i]++;
		if((i&3072)!=0) pruneFace2[i]++;
	}

	prepared=true;
}


int SolverKociemba::gettransEdgeOri( int pos, int m )
{
	//convert pos number into orientation
	int edges[12];
	num2ori(edges, 0,12,2, pos);

	//do move  	  L U F R D B
	if(m==3){
		cycle( edges, 0,5,8,4 );
		edges[0]^=1;edges[5]^=1;edges[8]^=1;edges[4]^=1;
	}else if(m==2){
		cycle( edges, 1,6,9,5 );
	}else if(m==0){
		cycle( edges, 2,7,10,6 );
		edges[2]^=1;edges[7]^=1;edges[10]^=1;edges[6]^=1;
	}else if(m==5){
		cycle( edges, 3,4,11,7 );
	}else if(m==1){
		cycle( edges, 3,2,1,0 );
	}else if(m==4){
		cycle( edges, 8,9,10,11 );
	}
	//convert back to number
	return(ori2num(edges, 0,12,2));
}
int SolverKociemba::gettransCornOri(int pos, int m)
{
	//convert pos number into permutation
	int corners[8];
	num2ori( corners,0,8,3,pos);

	//do move  	  L U F R D B
	if(m==3){
		cycle( corners, 0,1,5,4 );
		corners[0]+=2;corners[1]++;corners[5]+=2;corners[4]++;
	}else if(m==2){
		cycle( corners, 1,2,6,5 );
		corners[1]+=2;corners[2]++;corners[6]+=2;corners[5]++;
	}else if(m==0){
		cycle( corners, 2,3,7,6 );
		corners[2]+=2;corners[3]++;corners[7]+=2;corners[6]++;
	}else if(m==5){
		cycle( corners, 3,0,4,7 );
		corners[3]+=2;corners[0]++;corners[4]+=2;corners[7]++;
	}else if(m==1){
		cycle( corners, 3,2,1,0 );
	}else if(m==4){
		cycle( corners, 4,5,6,7 );
	}
	//convert back to number
	return(ori2num(corners,0,8,3));
}

int SolverKociemba::gettransCornPerm(int pos, int m)
{
	//convert pos number into permutation
	int corners[8];
	num2perm(corners,0,8,pos);
	//do move  	  L U F R D B
	if(m==3){
		cycle( corners, 0,1,5,4 );
	}else if(m==2){
		cycle( corners, 1,2,6,5 );
	}else if(m==0){
		cycle( corners, 2,3,7,6 );
	}else if(m==5){
		cycle( corners, 3,0,4,7 );
	}else if(m==1){
		cycle( corners, 3,2,1,0 );
	}else if(m==4){
		cycle( corners, 4,5,6,7 );
	}
	//convert back to number
	return(perm2num(corners,0,8));
}


int SolverKociemba::gettransEdgePerm( int pos, int m )
{
	//convert pos number into permutation
	int edges[8];
	num2perm(edges,0,8,pos);
	//do move  	  L U F R D B
	if(m==3){
		swap( edges, 0,4 );
	}else if(m==2){
		swap( edges, 1,5 );
	}else if(m==0){
		swap( edges, 2,6 );
	}else if(m==5){
		swap( edges, 3,7 );
	}else if(m==1){
		cycle( edges, 3,2,1,0 );
	}else if(m==4){
		cycle( edges, 4,5,6,7 );
	}
	//convert back to number
	return(perm2num(edges,0,8));
}

int SolverKociemba::gettransSliceFull( int pos, int m )
{
	int i,j,r;
	//convert pos number into permutation
	int edges[]={0,0,0,0, 0,0,0,0, 0,0,0,0};
	for( i=0; i<4; i++){
		r=pos%(12-i);
		pos=(pos-r)/(12-i);
		for(j=0; j<12 && (edges[j]!=0 || r>0); j++){
			if(edges[j]==0) r--;
		}
		edges[j]=i+1;
	}

	//do move  	  L U F R D B
	if(m==3){
		cycle( edges, 0,5,8,4 );
	}else if(m==2){
		cycle( edges, 1,6,9,5 );
	}else if(m==0){
		cycle( edges, 2,7,10,6 );
	}else if(m==5){
		cycle( edges, 3,4,11,7 );
	}else if(m==1){
		cycle( edges, 3,2,1,0 );
	}else if(m==4){
		cycle( edges, 8,9,10,11 );
	}
	//convert back to number
	pos=0;
	for( i=3; i>=0; i--){
		r=0;
		for( j=0; j<12 && edges[j]!=i+1; j++ ){
			if( edges[j]==0 || edges[j]>i+1 ) r++;
		}
		pos=pos*(12-i)+r;
	}
	return(pos);
}

int SolverKociemba::gettransSlicePerm( int pos, int m )
{
	//convert pos number into permutation
	int edges[4];
	num2perm(edges,0,4,pos);
	//do move  	  L U F R D B
	if(m==3){
		swap( edges, 0,1 );
	}else if(m==2){
		swap( edges, 1,2 );
	}else if(m==0){
		swap( edges, 2,3 );
	}else if(m==5){
		swap( edges, 3,0 );
	}
	//convert back to number
	return(perm2num(edges,0,4));
}

int SolverKociemba::gettransChoice( int pos, int m )
{
	//convert pos number into orientation
	int edges[12];
	num2ori( edges, 0,12,2, pos);

	//do move  	  L U F R D B
	if(m==3){
		cycle( edges, 0,5,8,4 );
	}else if(m==2){
		cycle( edges, 1,6,9,5 );
	}else if(m==0){
		cycle( edges, 2,7,10,6 );
	}else if(m==5){
		cycle( edges, 3,4,11,7 );
	}else if(m==1){
		cycle( edges, 3,2,1,0 );
	}else if(m==4){
		cycle( edges, 8,9,10,11 );
	}
	//convert back to number
	return(ori2num(edges,  0,12,2));
}

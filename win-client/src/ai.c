#pragma GCC optimize ("O3")
#include "ai.h"
#include <string.h> 
// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0
//AlphaBeta����
#define INTINITY 10000

int alpha=-INTINITY,beta=INTINITY;
char copboard1[BOARD_SIZE][BOARD_SIZE]={0};
const int DIR[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

BOOL isInBound(int x, int y) {
  return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}
struct Command myboard;
int history[12][12][8]={0};
struct Node
{
	int x;
	int y;
	int dir;
	int history;
};
/*
 * YOUR CODE BEGIN
 * ��Ĵ��뿪ʼ
 */
 
/* 
 * You can define your own struct and variable here
 * ����������ﶨ�����Լ��Ľṹ��ͱ���
 */
 

/*
 * ������������ʼ�����AI
 */
void initAI(int me) {

}
int Evaluate(char sboard[BOARD_SIZE][BOARD_SIZE],int flag)
{  
   int number=0;
   int card=0;//���ͼӷ� 
   int i,j;//�ٴα������ӣ������͡� 
   int tiao=0,jia=0;
   int close=0;
   int record=0; 
   int count=0;
   int otherflag=3-flag;
   for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			if (sboard[i][j] == flag){count++;
			//
			/*if(sboard[i+1][j+1]==flag&&sboard[i-1][j-1]==flag){card++;}
			if(sboard[i+1][j-1]==flag&&sboard[i-1][j+1]==flag){card++;}*/ 
			//����λʹ����һ���������в 
			if((sboard[i-1][j-1]==otherflag)&&(sboard[i-1][j+1]==otherflag)&&(sboard[i-1][j]==EMPTY)){tiao++;}
			if((sboard[i+1][j-1]==otherflag)&&(sboard[i+1][j+1]==otherflag)&&(sboard[i+1][j]==EMPTY)){tiao++;}
			if((sboard[i+1][j-1]==otherflag)&&(sboard[i-1][j-1]==otherflag)&&(sboard[i][j-1]==EMPTY)){tiao++;}
			if((sboard[i+1][j+1]==otherflag)&&(sboard[i-1][j+1]==otherflag)&&(sboard[i][j+1]==EMPTY)){tiao++;}
			if((sboard[i-2][j]==otherflag)&&(sboard[i][j+2]==otherflag)&&(sboard[i-1][j+1]==EMPTY)){tiao++;}
			if((sboard[i-2][j]==otherflag)&&(sboard[i][j-2]==otherflag)&&(sboard[i-1][j-1]==EMPTY)){tiao++;}
			if((sboard[i+2][j]==otherflag)&&(sboard[i][j+2]==otherflag)&&(sboard[i+1][j+1]==EMPTY)){tiao++;}
			if((sboard[i+2][j]==otherflag)&&(sboard[i][j-2]==otherflag)&&(sboard[i+1][j-1]==EMPTY)){tiao++;}
			//
			if(sboard[i][j-1]==otherflag&&sboard[i-1][j-1]==flag&&(sboard[i+1][j-1]==EMPTY||sboard[i][j-2]==EMPTY)){jia++;}
			if(sboard[i-1][j]==otherflag&&sboard[i-1][j-1]==flag&&(sboard[i-2][j]==EMPTY||sboard[i-1][j+1]==EMPTY)){jia++;}
			if(sboard[i][j+1]==otherflag&&sboard[i-1][j+1]==flag&&(sboard[i+1][j+1]==EMPTY||sboard[i][j+2]==EMPTY)){jia++;}
			if(sboard[i-1][j]==otherflag&&sboard[i-1][j+1]==flag&&(sboard[i-2][j]==EMPTY||sboard[i-1][j-1]==EMPTY)){jia++;}
			//
			if(sboard[i][j-1]==otherflag&&(sboard[i-1][j-2]==flag||sboard[i+1][j-2]==flag)&&sboard[i][j-2]==EMPTY){jia++;}
			if(sboard[i-1][j]==otherflag&&(sboard[i-2][j-1]==flag||sboard[i-2][j+1==flag])&&sboard[i-2][j]==EMPTY){jia++;}
			if(sboard[i][j+1]==otherflag&&(sboard[i-1][j+2]==flag||sboard[i+1][j+2]==flag)&&sboard[i][j+2]==EMPTY){jia++;}
			if(sboard[i+1][j]==otherflag&&(sboard[i+2][j-1]==flag||sboard[i+2][j+1]==flag)&&sboard[i+2][j]==EMPTY){jia++;}
			//
			if(sboard[i-3][j-3]==flag&&(sboard[i-1][j-1]==otherflag&&sboard[i-2][j-2]==EMPTY)||
			(sboard[i-1][j-1]==EMPTY&&sboard[i-2][j-2]==otherflag)){jia++;}
			if(sboard[i+3][j+3]==flag&&(sboard[i+1][j+1]==otherflag&&sboard[i+2][j+2]==EMPTY)||
			(sboard[i+1][j+1]==EMPTY&&sboard[i+2][j+2]==otherflag)){jia++;}
			if(sboard[i+3][j-3]==flag&&(sboard[i+1][j-1]==otherflag&&sboard[i+2][j-2]==EMPTY)||
			(sboard[i+1][j-1]==EMPTY&&sboard[i+2][j-2]==otherflag)){jia++;}
			if(sboard[i-3][j+3]==flag&&(sboard[i-1][j+1]==otherflag&&sboard[i-2][j+2]==EMPTY)||
			(sboard[i-1][j+1]==EMPTY&&sboard[i-2][j+2]==otherflag)){jia++;}
			if(sboard[i-3][j]==flag&&(sboard[i-1][j]==otherflag&&sboard[i-2][j]==EMPTY)||(sboard[i-1][j]==EMPTY&&sboard[i-2][j]==otherflag))
			{jia++;}
			if(sboard[i+3][j]==flag&&(sboard[i+1][j]==otherflag&&sboard[i+2][j]==EMPTY)||(sboard[i+1][j]==EMPTY&&sboard[i+2][j]==otherflag))
			{jia++;}
			if(sboard[i][j-3]==flag&&(sboard[i][j-1]==otherflag&&sboard[i][j-2]==EMPTY)||(sboard[i][j-1]==EMPTY&&sboard[i][j-2]==otherflag))
			{jia++;}
			if(sboard[i][j+3]==flag&&(sboard[i][j+1]==otherflag&&sboard[i][j+2]==EMPTY)||(sboard[i][j+1]==EMPTY&&sboard[i][j+2]==otherflag))
			{jia++;}
			//��ɱ�󷨣��ڶԷ����������ϵ������뷽���У��ܳԵ��Է�һ�֡� 
			if(sboard[i-3][j-1]==flag){
			if(sboard[i-2][j-1]==otherflag)record++;
			if(sboard[i-2][j]==otherflag)record++;
			if(sboard[i-1][j-1]==otherflag)record++;
			if(sboard[i-1][j]==otherflag)record++;
			if(record>=2)card++;record=0;
			}
			if(sboard[i-3][j+1]==flag){
			if(sboard[i-2][j+1]==otherflag)record++;
			if(sboard[i-2][j]==otherflag)record++;
			if(sboard[i-1][j+1]==otherflag)record++;
			if(sboard[i-1][j]==otherflag)record++;
			if(record>=2)card++;record=0;
			}
			if(sboard[i-1][j-3]==flag){
			if(sboard[i-1][j+2]==otherflag)record++;
			if(sboard[i][j+2]==otherflag)record++;
			if(sboard[i-1][j+1]==otherflag)record++;
			if(sboard[i][j+1]==otherflag)record++;
			if(record>=2)card++;record=0;
			}
			if(sboard[i+1][j+3]==flag){
			if(sboard[i+1][j+2]==otherflag)record++;
			if(sboard[i][j+2]==otherflag)record++;
			if(sboard[i+1][j+1]==otherflag)record++;
			if(sboard[i][j+1]==otherflag)record++;
			if(record>=2)card++;record=0;
			}
			//�������ˡ� 
			int x=-1,y=-1;
			for(x=-1;x<=1;x++)
			  for(y=-1;y<=1;y++)
			  {
			  	if(sboard[i+x][j+y]==otherflag) close++;
			  }
		    }
	    }
    }
	number=count*400+jia*6+tiao*12+card*5+close;
	return number;
}
void MakeNextMove(int x,int y,int flag,int direction,char copboard[BOARD_SIZE][BOARD_SIZE])
{//Ϊ�˷��㡣����ʵ����sboard���� 
	int newx,newy;
	int otherflag=3-flag;
	newx = x + DIR[direction][0];
	newy = y + DIR[direction][1];
	copboard[x][y] = EMPTY;
	copboard[newx][newy] = flag;
	int intervention_dir[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
	int i;
	for (i = 0; i < 4; i++) {
		int x1 = newx + intervention_dir[i][0];
		int y1 = newy + intervention_dir[i][1];
		int x2 = newx - intervention_dir[i][0];
		int y2 = newy - intervention_dir[i][1];
		if (isInBound(x1, y1) && isInBound(x2, y2) && copboard[x1][y1] == otherflag && copboard[x2][y2] == otherflag) {
			copboard[x1][y1] = flag;
			copboard[x2][y2] = flag;
		}
	}

	// �?
	int custodian_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

	for (i = 0; i < 8; i++) {
		int x1 = newx + custodian_dir[i][0];
		int y1 = newy + custodian_dir[i][1];
		int x2 = newx + custodian_dir[i][0] * 2;
		int y2 = newy + custodian_dir[i][1] * 2;
		if (isInBound(x1, y1) && isInBound(x2, y2) && copboard[x2][y2] == flag && copboard[x1][y1] == otherflag) {
			copboard[x1][y1] = flag;
		}
	}
}
void UnmakeMove(char board1[BOARD_SIZE][BOARD_SIZE],char board2[BOARD_SIZE][BOARD_SIZE])
{
	int x,y; 
	for (x = 0; x < BOARD_SIZE; x++) {
		for (y = 0; y < BOARD_SIZE; y++) {
			board1[x][y] = board2[x][y];
		}
	}
}
int Comp(const void*p1,const void*p2)
{
    return ((*(struct Node*)p2).history>(*(struct Node*)p1).history)?1:-1;
}
int GetHistory(int i,int j,int direction)
{
	return history[i][j][direction];
}
void resetHistory(int i,int j,int direction,int score)
{
	history[i][j][direction]=score;
}
void clearHistory()
{
	memset(history,0,sizeof(history));
}
//ʹcount Ϊ���п����߷�����
int legalMove(int flag,char boardq[BOARD_SIZE][BOARD_SIZE],struct Node qifa[12*12*8])
{   
    int i,j,direction,count=0;
    int newx,newy;
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			if (boardq[i][j] == flag)/*�ҵ��ҵ��塣*/{
				for(direction=0;direction<8;direction++){
					newx=i+DIR[direction][0];
					newy=j+DIR[direction][1];
					if (isInBound(newx, newy) && boardq[newx][newy]==EMPTY){
					qifa[count].x=i;qifa[count].y=j;qifa[count].dir=direction;
					qifa[count].history=GetHistory(i,j,direction);
					count++;}}}}}
	return count;
}
int AlphaBeta(int depth, int alpha, int beta,int flag,char boardq[BOARD_SIZE][BOARD_SIZE]) {
  if (depth == 4) {
     return Evaluate(boardq,flag);
  }
  int i=0;
  int bestx,besty,bestdir;
  int count;
  struct Node qifa[12*12*8];
  int val;
  char newboard[BOARD_SIZE][BOARD_SIZE]={0};//�������̣����㳷�� 
  UnmakeMove(newboard,boardq);
  count=legalMove(flag,boardq,qifa);
  qsort(qifa,count,sizeof(qifa[0]),Comp);
  for(i=0;i<count;i++){
  	  int x=qifa[i].x,y=qifa[i].y,direction=qifa[i].dir;
  	  MakeNextMove(x,y,flag,direction,boardq);                        
      val = -AlphaBeta(depth + 1, -beta, -alpha,3-flag,boardq);
      UnmakeMove(boardq,newboard);
      if (val >= beta) 
	  {
      	resetHistory(x,y,direction,15<<(5-depth));   
        return beta;
      }
      if (val > alpha) {
	  bestx=x;besty=y;bestdir=direction;                        
	  alpha = val;
      }
  }
  if(alpha!=-10000)
  {resetHistory(bestx,besty,bestdir,alpha);}
  return alpha;
}
struct Command findValidPos(char sboard[BOARD_SIZE][BOARD_SIZE], int flag){ 
	int max=-30000;//ȷ�����շ��� 
	char copboard[BOARD_SIZE][BOARD_SIZE]={0};//�������̣����㳷�� 
	UnmakeMove(copboard,sboard); 
	int i=0;
    int count;
    struct Node qifa[12*12*8];
    int val;
    count=legalMove(flag,sboard,qifa);
    qsort(qifa,count,sizeof(qifa[0]),Comp);
    for(i=0;i<count;i++){
  	                  int x=qifa[i].x,y=qifa[i].y,direction=qifa[i].dir;
  	                  MakeNextMove(x,y,flag,direction,sboard);                        
                      val = -AlphaBeta(1, -beta, -alpha,3-flag,sboard);
                      UnmakeMove(sboard,copboard);
					  if(val>=max){max=val;myboard.x=x;myboard.y=y;myboard.option=direction;}
					}//������ó��Ա�Ƚ� 
	clearHistory();
    return myboard;
}


/*
 * �ֵ������ӡ�
 * ������0��ʾ�հף�1��ʾ���壬2��ʾ����
 * me��ʾ�������������(1��2) 
 * ����Ҫ����һ���ṹ��Position����x���Ժ�y������������Ҫ���ӵ�λ�á� 
 */
struct Command aiTurn(const char board[BOARD_SIZE][BOARD_SIZE], int me){
	struct Command preferenPos;
	int x, y;//stand for the position
	for (x = 0; x < BOARD_SIZE; x++) {
		for (y = 0; y < BOARD_SIZE; y++) {
			copboard1[x][y] = board[x][y];
		}
	}
	preferenPos = findValidPos(copboard1, me);
	return preferenPos;
}

/*
 * ��Ĵ������ 
 */

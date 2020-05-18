#pragma GCC optimize ("O3")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/time.h>

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2

// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0

// option
typedef int OPTION;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define UP_LEFT 4
#define UP_RIGHT 5
#define DOWN_LEFT 6
#define DOWN_RIGHT 7

#define MAX_BYTE 10000

#define START "START"
#define PLACE "PLACE"
#define TURN  "TURN"
#define END   "END"
//AlphaBeta参数
#define INTINITY 10000

int alpha=-INTINITY,beta=INTINITY;
char copboard1[BOARD_SIZE][BOARD_SIZE]={0};

struct Command
{
	int x;
	int y;
	OPTION option;
};
struct Command myboard;
int history[12][12][8]={0};
struct Node
{
	int x;
	int y;
	int dir;
	int history;
};

char buffer[MAX_BYTE] = { 0 };
char board[BOARD_SIZE][BOARD_SIZE] = { 0 };
int me_flag;
int other_flag;

 

const int DIR[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

const int DI[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };



void debug(const char *str) {
	printf("DEBUG %s\n", str);
	fflush(stdout);
}

void printBoard() {
	char visual_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == EMPTY) {
				visual_board[i][j] = '.';
			}
			else if (board[i][j] == BLACK) {
				visual_board[i][j] = 'O';
			}
			else if (board[i][j] == WHITE) {
				visual_board[i][j] = 'X';
			}
		}
		printf("%s\n", visual_board[i]);
	}
}

BOOL isInBound(int x, int y) {
	return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

/**
 * YOUR CODE BEGIN
 * 浣犵殑浠ｇ爜寮�濮?
 */

 /**
  * You can define your own struct and variable here
  * 浣犲彲浠ュ湪杩欓噷瀹氫箟浣犺嚜宸辩殑缁撴瀯浣撳拰鍙橀噺
  */


  /**
   * 浣犲彲浠ュ湪杩欓噷鍒濆鍖栦綘鐨凙I
   */
void initAI(int me) {

}
int Evaluate(char sboard[BOARD_SIZE][BOARD_SIZE],int flag)
{  
   int number=0;
   int card=0;//阵型加分 
   int i,j;//再次遍历数子，构阵型。 
   int tiao=0,jia=0;
   int close=0; 
   int count=0;
   int otherflag=3-flag;
   for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			if (sboard[i][j] == flag){count++;
			//
			/*if(sboard[i+1][j+1]==flag&&sboard[i-1][j-1]==flag){card++;}
			if(sboard[i+1][j-1]==flag&&sboard[i-1][j+1]==flag){card++;}*/ 
			//挑夹位使得下一步可造成威胁 
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
			//必杀阵法，在对方有两个以上的子落入方形中，能吃掉对方一字。 
			if(sboard[i-3][j-1]==flag&&(sboard[i-2][j-1]==otherflag||sboard[i-2][j]==otherflag||sboard[i-1][j-1]==otherflag||
			sboard[i-1][j]==otherflag)) {card++;}
			if(sboard[i-3][j+1]==flag&&(sboard[i-2][j+1]==otherflag||sboard[i-2][j]==otherflag||sboard[i-1][j+1]==otherflag||
			sboard[i-1][j]==otherflag)) {card++;}
			if(sboard[i-1][j+3]==flag&&(sboard[i-1][j+2]==otherflag||sboard[i][j+2]==otherflag||sboard[i-1][j+1]==otherflag||
			sboard[i][j+1]==otherflag)) {card++;}
			if(sboard[i+1][j+3]==flag&&(sboard[i+1][j+2]==otherflag||sboard[i][j+2]==otherflag||sboard[i+1][j+1]==otherflag||
			sboard[i][j+1]==otherflag)) {card++;}
			//靠近敌人。 
			int x=-1,y=-1;
			for(x=-1;x<=1;x++)
			  for(y=-1;y<=1;y++)
			  {
			  	if(sboard[i+x][j+y]==otherflag) close++;
			  }
		    }
	    }
    }
	number=count*400+jia*5+tiao*10+card*4+close;
	return number;
}
void MakeNextMove(int x,int y,int flag,int direction,char copboard[BOARD_SIZE][BOARD_SIZE])
{
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

	// 澶?
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
//使count 为所有可能走法数。
int legalMove(int flag,char boardq[BOARD_SIZE][BOARD_SIZE],struct Node qifa[12*12*8])
{   
    int i,j,direction,count=0;
    int newx,newy;
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			if (boardq[i][j] == flag)/*找到我的棋。*/{
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
  struct Node qifa[12*12*8];//记录每一种走法 
  int val;
  char newboard[BOARD_SIZE][BOARD_SIZE]={0};//复制棋盘，方便撤销 
  UnmakeMove(newboard,boardq);
  count=legalMove(flag,boardq,qifa);
  qsort(qifa,count,sizeof(qifa[0]),Comp);//快排实现剪枝走法或估分高的走法先走，提高剪枝效率 
  for(i=0;i<count;i++){
  	  int x=qifa[i].x,y=qifa[i].y,direction=qifa[i].dir;
  	  MakeNextMove(x,y,flag,direction,boardq);                        
      val = -AlphaBeta(depth + 1, -beta, -alpha,3-flag,boardq);
      UnmakeMove(boardq,newboard);//撤销走步。 
      if (val >= beta) 
	  {
      	resetHistory(x,y,direction,15<<(5-depth));   
        return beta;//将能触发剪枝的看作一种好的走法，加入历史表 
      }
      if (val > alpha) {
	  bestx=x;besty=y;bestdir=direction;                        
	  alpha = val;
      }
  }//记录最高alpha得分及走法（看作一种好的走法） 
  if(alpha!=-10000)
  {resetHistory(bestx,besty,bestdir,alpha);}
  return alpha;
}
struct Command findValidPos(char sboard[BOARD_SIZE][BOARD_SIZE], int flag){ 
	int max=-30000;//确定最终方向。 
	char copboard[BOARD_SIZE][BOARD_SIZE]={0};//复制棋盘，方便撤销 
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
					}//最外层拿出以便比较 
	clearHistory();
    return myboard;
}

/**
 * 杞埌浣犺惤瀛愩�?
 * 妫嬬洏涓?琛ㄧず绌虹櫧锛?琛ㄧず榛戞锛?琛ㄧず鐧芥棗
 * me琛ㄧず浣犳墍浠ｈ〃鐨勬瀛?1鎴?)
 * 浣犻渶瑕佽繑鍥炰竴涓粨鏋勪綋Command锛屽湪x灞炴�у拰y灞炴�у～涓婁綘鎯宠绉诲姩鐨勬瀛愮殑浣嶇疆锛宱ption濉笂浣犳兂瑕佺Щ鍔ㄧ殑鏂瑰悜銆?
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
/**
 * 浣犵殑浠ｇ爜缁撴潫
 */


BOOL place(int x, int y, OPTION option, int cur_flag) {
	// 绉诲姩涔嬪墠鐨勪綅缃病鏈夋垜鏂规瀛?
	if (board[x][y] != cur_flag) {
		return FALSE;
	}

	int new_x = x + DIR[option][0];
	int new_y = y + DIR[option][1];
	// 绉诲姩涔嬪悗鐨勪綅缃秴鍑鸿竟鐣? 鎴栬�呬笉鏄┖鍦?
	if (!isInBound(new_x, new_y) || board[new_x][new_y] != EMPTY) {
		return FALSE;
	}

	board[x][y] = EMPTY;
	board[new_x][new_y] = cur_flag;

	int cur_other_flag = 3 - cur_flag;
	// 鎸?
	int intervention_dir[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
	int i;
	for (i = 0; i < 4; i++) {
		int x1 = new_x + intervention_dir[i][0];
		int y1 = new_y + intervention_dir[i][1];
		int x2 = new_x - intervention_dir[i][0];
		int y2 = new_y - intervention_dir[i][1];
		if (isInBound(x1, y1) && isInBound(x2, y2) && board[x1][y1] == cur_other_flag && board[x2][y2] == cur_other_flag) {
			board[x1][y1] = cur_flag;
			board[x2][y2] = cur_flag;
		}
	}

	// 澶?
	int custodian_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

	for (i = 0; i < 8; i++) {
		int x1 = new_x + custodian_dir[i][0];
		int y1 = new_y + custodian_dir[i][1];
		int x2 = new_x + custodian_dir[i][0] * 2;
		int y2 = new_y + custodian_dir[i][1] * 2;
		if (isInBound(x1, y1) && isInBound(x2, y2) && board[x2][y2] == cur_flag && board[x1][y1] == cur_other_flag) {
			board[x1][y1] = cur_flag;
		}
	}

	return TRUE;
}


//........
//........
//..XXXX..
//XXXX....
//....OOOO
//..OOOO..
//........
//........
void start(int flag) {
	memset(board, 0, sizeof(board));
	int i;

	for (i = 0; i < 3; i++) {
		board[2][2 + i] = WHITE;
		board[6][6 + i] = WHITE;
		board[5][3 + i] = BLACK;
		board[9][7 + i] = BLACK;
	}

	for (i = 0; i < 2; i++) {
		board[8 + i][2] = WHITE;
		board[2 + i][9] = BLACK;
	}

	initAI(flag);
}

void turn() {
	// AI
	struct Command command = aiTurn((const char(*)[BOARD_SIZE])board, me_flag);
	place(command.x, command.y, command.option, me_flag);
	printf("%d %d %d\n", command.x, command.y, command.option);
	fflush(stdout);
}

void end(int x) {

}

void loop() {
	//  freopen("../input", "r", stdin);
	while (TRUE)
	{
		memset(buffer, 0, sizeof(buffer));
		gets(buffer);

		if (strstr(buffer, START))
		{
			char tmp[MAX_BYTE] = { 0 };
			sscanf(buffer, "%s %d", tmp, &me_flag);
			other_flag = 3 - me_flag;
			start(me_flag);
			printf("OK\n");
			fflush(stdout);
		}
		else if (strstr(buffer, PLACE))
		{
			char tmp[MAX_BYTE] = { 0 };
			int x, y;
			OPTION option;
			sscanf(buffer, "%s %d %d %d", tmp, &x, &y, &option);
			place(x, y, option, other_flag);
		}
		else if (strstr(buffer, TURN))
		{
			turn();
		}
		else if (strstr(buffer, END))
		{
			char tmp[MAX_BYTE] = { 0 };
			int x;
			sscanf(buffer, "%s %d", tmp, &x);
			end(x);
		}
		//    printBoard();
	}
}


int main(int argc, char *argv[])
{
	/* 定义两个结构体 */
    /*struct timeval start;
    struct timeval end;
    unsigned long timer;
    /* 程序开始之前计时start */
    //gettimeofday(&start, NULL);
	loop();
	/* 程序块结束后计时end */
    //gettimeofday(&end, NULL);
    /* 统计程序段运行时间(unit is usec)*/
    /*timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("timer = %ld us\n", timer);*/ 
    return 0;
}

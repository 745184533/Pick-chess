#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

#define BOARD_SIZE 12
#define EMPTY      0
#define BLACK      1
#define WHITE      2

typedef int OPTION;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define UP_LEFT 4
#define UP_RIGHT 5
#define DOWN_LEFT 6
#define DOWN_RIGHT 7

struct globalArgs_t {
    int port;
} globalArgs;

static const char *optString = "p:h";

const int DIR[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

char board[BOARD_SIZE][BOARD_SIZE] = {0};
char buffer[MAXBYTE] = {0};
int turn = BLACK;
int nowx, nowy, nextx, nexty, option;

SOCKET servSock, blackSock, whiteSock;

/*
 * Ip Utils
 */
 
BOOL isPort(const int port)
{
    return (port >= 0 && port <= 65535);
}


char *getIp()
{
    PHOSTENT hostinfo;
    char name[255];
    char* ip;
    if(gethostname(name, sizeof(name)) == 0)
    {
        if((hostinfo = gethostbyname(name)) != NULL)
        {
            ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
            return ip;
        }
    }
    return NULL;
} 

void sendTo(SOCKET *sock, const char *message)
{
    send(*sock, message, strlen(message) + sizeof(char), 0);
    Sleep(100);
}

void retry(SOCKET *sock)
{
    sendTo(sock, "TURN\n");
}

BOOL inBoard(int x, int y)
{
    return x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE;
}

void handle(SOCKET *me, int meFlag, SOCKET *other, int otherFlag)
{
	int i, j, k;
    memset(buffer, 0, sizeof(buffer));
    recv(*me, buffer, MAXBYTE, 0);
    sscanf(buffer, "%d %d %d\n", &nowx, &nowy, &option);
    printf("client %d : %d %d %d\n", meFlag, nowx, nowy, option);
    
    // Judge if legal
    if (option < UP || option > DOWN_RIGHT)
    {
        retry(me);
        return;
	}
    
    nextx = nowx + DIR[option][0];
	nexty = nowy + DIR[option][1];
    
    // Judge if legal
    if (!inBoard(nowx, nowy) || !inBoard(nextx, nexty) || board[nowx][nowy] != meFlag || board[nextx][nexty] != EMPTY)
    {
        retry(me);
        return;
    }
    
    board[nowx][nowy] = EMPTY;
    board[nextx][nexty] = meFlag;
    
    
    // Yak
    if (inBoard(nextx - 1, nexty - 1) && inBoard(nextx + 1, nexty + 1) && board[nextx - 1][nexty - 1] == otherFlag && board[nextx + 1][nexty + 1] == otherFlag)
    {
        board[nextx - 1][nexty - 1] = board[nextx + 1][nexty + 1] = meFlag;
    }
    if (inBoard(nextx + 1, nexty - 1) && inBoard(nextx - 1, nexty + 1) && board[nextx + 1][nexty - 1] == otherFlag && board[nextx - 1][nexty + 1] == otherFlag)
    {
        board[nextx + 1][nexty - 1] = board[nextx - 1][nexty + 1] = meFlag;
    }
    if (nexty - 1 >= 0 && nexty + 1 < BOARD_SIZE && board[nextx][nexty - 1] == otherFlag && board[nextx][nexty + 1] == otherFlag)
    {
        board[nextx][nexty - 1] = board[nextx][nexty + 1] = meFlag;
    }
    if (nextx - 1 >= 0 && nextx + 1 < BOARD_SIZE && board[nextx - 1][nexty] == otherFlag && board[nextx + 1][nexty] == otherFlag)
    {
        board[nextx - 1][nexty] = board[nextx + 1][nexty] = meFlag;
    }

    // Mak
    for (i = 0; i < 8; i++)
    {
        if (inBoard(nextx + 2 * DIR[i][0], nexty + 2 * DIR[i][1]) &&
            board[nextx + DIR[i][0]][nexty + DIR[i][1]] == otherFlag && board[nextx + 2 * DIR[i][0]][nexty + 2 * DIR[i][1]] == meFlag)
        {
            board[nextx + DIR[i][0]][nexty + DIR[i][1]] = meFlag;
        }
    }
    
    switch (meFlag)
    {
        case BLACK:
            printf("BLACK move chess piece from (%d, %d) to (%d, %d)\n", nowx, nowy, nextx, nexty);
            break;
        case WHITE:
            printf("WHITE move chess piece from (%d, %d) to (%d, %d)\n", nowx, nowy, nextx, nexty);
            break;
    }
    
    // Forward
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "PLACE %d %d %d\n", nowx, nowy, option);
    sendTo(other, buffer);
    
    // Count Enemy's Chess Pieces
    int enemyCount = 0;
	BOOL enemyCannotMove = TRUE;
    for (i = 0; i < BOARD_SIZE; i++)
    {
    	for (j = 0; j < BOARD_SIZE; j++)
    	{
    		if (board[i][j] == otherFlag)
    		{
    			enemyCount ++;
    			for (k = 0; k < 8; k++)
    			{
    				if (enemyCannotMove && inBoard(i+DIR[k][0],j+DIR[k][1]) && board[i+DIR[k][0]][j+DIR[k][1]] == EMPTY)
    				{
    					enemyCannotMove = FALSE;
					}
				}
			}
		}
	}
	
    // Judge result
    if (enemyCount == 0 || enemyCannotMove)
    {
        sendTo(me, "WIN\n");
        sendTo(other, "LOSE\n");
        if (BLACK == meFlag)
        {
            printf("BLACK win!\n");
        }
        else
        {
            printf("WHITE win!\n");
        }
        return;
    }
    else
    {
    	sendTo(other, "TURN\n");
	}
    
    turn = otherFlag;
}

void startSock()
{
    // Initial DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void initSock(int port)
{
    // Open socket
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // Bind socket
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;  // Use ipv4 address 
    sockAddr.sin_addr.s_addr = inet_addr("0.0.0.0");  // Bind to LAN
    sockAddr.sin_port = htons(port);  // Bind port
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    
    // Listen
    listen(servSock, 20);
    
    char *ip = getIp();
    if (NULL != ip)
    {
        printf("Listening on %s:%d\n", ip, port);
    }
    else
    {
        printf("Listening...\n");
    }
    
    int nSize;
    
    // Connect to BLACK
    SOCKADDR blackAddr;
    nSize = sizeof(SOCKADDR);
    blackSock = accept(servSock, (SOCKADDR*)&blackAddr, &nSize);
    printf("Client Connected\n");
    
    // Connect to WHITE
    SOCKADDR whiteAddr;
    nSize = sizeof(SOCKADDR);
    whiteSock = accept(servSock, (SOCKADDR*)&whiteAddr, &nSize);
    printf("Client Connected\n");
    
    // Initial BLACK
    sendTo(&blackSock, "START 1\n");
    
    // Initial WHITE
    sendTo(&whiteSock, "START 2\n");
    
    // Begin gaming...
	sendTo(&blackSock, "TURN\n");
}

void closeSock()
{
    // Close socket
    closesocket(blackSock);
    closesocket(whiteSock);
    closesocket(servSock);
    
    // Close DLL
    WSACleanup();
}

void loop()
{   
    while (TRUE)
    {
        switch (turn)
        {
            case BLACK:
                handle(&blackSock, BLACK, &whiteSock, WHITE);
                break;
            case WHITE:
                handle(&whiteSock, WHITE, &blackSock, BLACK);
                break;
        }
    }
}

void display_usage(char *exe)
{
    printf("Usage: %s [OPTIONS] \n", exe);
    printf("  -p port           Server port\n");
}

void initArgs(int argc, char *argv[])
{
    int opt = 0;
    globalArgs.port = 23333;
    
    opt = getopt(argc, argv, optString);
    while (opt != -1)
    {
        switch (opt)
        {
            case 'p':
                globalArgs.port = atoi(optarg);
                break;
            case 'h':
                display_usage(argv[0]);
                exit(0);
                break;
            default:
                // Illegal!
                break;
        }
        
        opt = getopt(argc, argv, optString);
    }
}

void initBoard()
{
	memset(board, 0, sizeof(board));
	
	int i;
	for (i = 0; i < 3; i++)
	{
    	board[2][2+i] = board[6][6+i] = WHITE;
    	board[5][3+i] = board[9][7+i] = BLACK;
	}
	for (i = 0; i < 2; i++)
	{
    	board[8+i][2] = WHITE;
    	board[2+i][9] = BLACK;
	}
}

int main(int argc, char *argv[]){
    
    startSock();
    
    initArgs(argc, argv);
    
    initSock(globalArgs.port);
    
    initBoard();
    
    loop();
    
    closeSock();
    
    return 0;
}

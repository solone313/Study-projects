#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include "block.h"

int curPosX;
int curPosY;//커서들의 위치
int block_id;//블록의 인덱스
int Speed;//속도
int score = 0,level=0;
#define A 97
#define S 115
#define D 100
#define Z 122
#define XX 120
#define C 99
#define Q 113
#define W 119
#define E 101
#define J 106
#define K 107
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define GBOARD_WIDTH 10
#define GBOARD_HEIGHT 20

#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2
int gameBlockInfo[GBOARD_HEIGHT+1][GBOARD_WIDTH+2];
void SetCurrentCursorPos(int x, int y){//커서의 위치를 옮겨주는 함수
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD GetCurrentCursorPos(void){//커서의 현재위치를 알려주는 함수
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void RemoveCursor(void)
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&curInfo);
    curInfo.bVisible=0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&curInfo);
}//커서 제거함수

void ShowBlock(char blockInfo[4][4]){//블록을 그려주는 함수
	int x, y;
	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			SetCurrentCursorPos(curPosX + (x * 2), curPosY + y);
			if (blockInfo[y][x] == 1)
				printf("■"); 
		} 
	}
	SetCurrentCursorPos(curPosX, curPosY);
}

void DeleteBlock(char blockInfo[4][4])//블록을 지우는 함수
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1) printf("  ");
		}
	}
	SetCurrentCursorPos(curPosX, curPosY);
}
int CollisionCheck(int q, int w, char BlockModel[4][4]) {//충돌 감지하는 함수
	int x, y;
	int arrX = q / 2 - GBOARD_ORIGIN_X/2;
	int arrY = w - GBOARD_ORIGIN_Y;
	for (x = 0; x < 4; x++) {
		for (y = 0; y < 4; y++) {
			if (gameBlockInfo[arrY + y][arrX + x] == 1 && BlockModel[y][x] == 1)
				return 0;
		}
	}

	return 1;
}
void ShiftLeft()
{
	if (CollisionCheck(curPosX - 2 , curPosY, blockModel[block_id]) == 0) {
		return;
	}
	COORD curPos = GetCurrentCursorPos();
	DeleteBlock(blockModel[block_id]);
	curPosX -= 2;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[block_id]);
}

void ShiftRight()
{
	if (CollisionCheck(curPosX + 2, curPosY, blockModel[block_id]) == 0) {
		return;
	}
	COORD curPos = GetCurrentCursorPos();
	DeleteBlock(blockModel[block_id]);
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[block_id]);
}

int BlockDown()
{
	SetCurrentCursorPos(curPosX, curPosY);
	if (!CollisionCheck(curPosX + 1, curPosY+1, blockModel[block_id])) {
		return 0;
	}
	COORD curPos = GetCurrentCursorPos();
	DeleteBlock(blockModel[block_id]);
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[block_id]);
	return 1;
}
void BlockUp()
{
	COORD curPos = GetCurrentCursorPos();
	DeleteBlock(blockModel[block_id]);
	curPosY -= 1;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[block_id]);
}

void RotateBlock()
{
	int block_base= block_id - block_id % 4;; 
	int block_rotated = block_base + (block_id + 1) % 4;
	COORD curPos = GetCurrentCursorPos();
	if (CollisionCheck(curPos.X, curPos.Y, blockModel[block_rotated]) == 0)
	{
		return;
	}

	DeleteBlock(blockModel[block_id]);
	block_id = block_rotated;
	ShowBlock(blockModel[block_id]);

}

void ReverseRotateBlock()
{
	int block_base;
	COORD curPos = GetCurrentCursorPos();
	

	DeleteBlock(blockModel[block_id]);

	block_base = block_id - block_id%4;

	if(block_id%4 == 0)
	{
		block_id = block_base + (block_id+3)%4;
	}
	else
	{
		block_id = block_base + (block_id-1)%4;
	}
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[block_id]);
}
void spaceDown() {
	while (BlockDown());

}
void RedrawBlocks()
{
	int x, y;
	int cursX, cursY;
	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(cursX, cursY);

			if (gameBlockInfo[y][x] == 1)
			{
				printf("■");
			}
			else
			{
				printf("  ");
			}
		}
	}
}
void ProcessKeyInput()
{
	int key;
	for(int i=0;i<20;i++)
		{		
		if(_kbhit()!=0)
		{	
			key = _getch();
			switch(key)
			{
			case LEFT:
				ShiftLeft();
				break;
			case RIGHT:
				ShiftRight();
				break;
			case DOWN:
				BlockDown();
				break;
			case UP:
				if (CollisionCheck(curPosX , curPosY , blockModel[block_id])==1) {
					RotateBlock();
				}
				break;
			case 32:
				spaceDown();
			}
		}
		Sleep(Speed);
	}	
}

void DrawgameBoard()
{
	for(int y=0;y<=GBOARD_HEIGHT;y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);

		if(y==GBOARD_HEIGHT)
			printf("└");
		else
			printf("│");

	}
	for(int x=1;x<=GBOARD_WIDTH+1;x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X+2*x, GBOARD_ORIGIN_Y+20);


			printf("─");

	}
	for(int y=0;y<=GBOARD_HEIGHT;y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X+22, GBOARD_ORIGIN_Y + y);

		if(y==GBOARD_HEIGHT)
			printf("┘");
		else
			printf("│");

	}
	for(int y=0; y<=GBOARD_HEIGHT; y++){
		gameBlockInfo[y][0]=1;
		gameBlockInfo[y][GBOARD_WIDTH+1]=1;
	}

	for(int x=0; x<= GBOARD_WIDTH; x++){
		gameBlockInfo[GBOARD_HEIGHT][x]=1;
	}
	
	SetCurrentCursorPos(12, 0);

}
int isGameOver() {
	if (CollisionCheck(curPosX, curPosY, blockModel[block_id])==0) {
		return 1;
	}
	return 0;

}
void AddBlockToBoard() {
	int x, y, arrCurX, arrCurY;
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			arrCurX = (curPosX - GBOARD_ORIGIN_X) / 2;
			arrCurY = curPosY - GBOARD_ORIGIN_Y;

			if (blockModel[block_id][y][x] == 1)
				gameBlockInfo[arrCurY + y][arrCurX + x] = 1;//게임 보드에 1을 써넣어 블록추가
		}
	
	}
}
void RemoveFillUpLine()
{
	int x, y, line;

	for (y = GBOARD_HEIGHT - 1; y > 0; y--)      //아래부터검사
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			if (gameBlockInfo[y][x] != 1)      //빈칸이있다면
				break;
		}

		if (x == GBOARD_WIDTH + 1)      //끝까지 검사완료시
		{
			for (line = 0; y > line; line++)      //없어진윗줄부터 처음까지
			{
				memcpy(&gameBlockInfo[y - line][1], &gameBlockInfo[y - line - 1][1], sizeof(int)*GBOARD_WIDTH);
			}
			y++;      //y값 조정(제거해서 한칸씩 아래로 내려와서)
			score += 10;
			level = score / 15;
		}
	}
	RedrawBlocks();
	
}
/*memcpy( ) 함수 사용 법
memcpy(dest, src, size )
dest - address of memory recorded(destination)
src - address of memory read(source)
size - size of byte*/
void printScore() {
	SetCurrentCursorPos(30+GBOARD_ORIGIN_X, 2+GBOARD_ORIGIN_Y);
	printf("현재 점수: %d\n", score);
	SetCurrentCursorPos(30 + GBOARD_ORIGIN_X, 3 + GBOARD_ORIGIN_Y);
	printf("현재 레벨: %d", level);
}
int main(void)
{
	
	RemoveCursor();//커서 깜빡이는 것 제거
	DrawgameBoard();
	int key;
	curPosX = 8 + GBOARD_ORIGIN_X, curPosY = -1+GBOARD_ORIGIN_Y;
	COORD curPos = GetCurrentCursorPos();
	while(1)
	{
		Speed = 20-level*1;
		printScore();
		if (isGameOver() == 1) {
			SetCurrentCursorPos(30 + GBOARD_ORIGIN_X, 4 + GBOARD_ORIGIN_Y);
			printf("GAME OVER");
			break;
		}
		
		srand((unsigned int)time(NULL));
		block_id = (rand() % 7) *4;
		
		while (1) {
			if(BlockDown() ==0){
				AddBlockToBoard();
				RemoveFillUpLine();
				printScore();
				break;
			}
			ProcessKeyInput();
		}
		curPosX = 8 + GBOARD_ORIGIN_X;
		curPosY = -2+ GBOARD_ORIGIN_Y;
	}

	return 0;
}
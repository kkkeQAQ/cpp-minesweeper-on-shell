#include <cstdio>                                      /********************/
#include <iostream>                                    /*   made by kkke   */
#include <time.h>                                      /********************/
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "getch.h"
#include <stdlib.h>

#define MAXY 20
#define MAXX 20
#define MINE_NUM 40

using namespace std;

const string version="v1.3";

int mymap[MAXX][MAXY];
bool mymine[MAXX][MAXY];
bool mysight[MAXX][MAXY];
bool myflag[MAXX][MAXY];
int nowy, nowx;
int the_rest_of_mine=MINE_NUM;
int the_rest_of_square=MAXY*MAXX;
char c_input;
bool firstMove;

void print_map(int finished=0)
{
	for(int i=-2;i<MAXY*3;i++)cout<<'-';
	cout<<endl;
	for(int i=0;i<MAXX;i++)
	{
		cout<<'|';
		for(int j=0;j<MAXY;j++)
		{
			if((i==nowx)&&(j==nowy))cout<<'[';
			else cout<<' ';
			if(myflag[i][j])cout<<'@';
			else if(mysight[i][j])
			{
				if(mymine[i][j])cout<<'*';
				else if(mymap[i][j])cout<<mymap[i][j];
				else cout<<" ";
			}
			else cout<<'.';
			if((i==nowx)&&(j==nowy))cout<<']';
			else cout<<' ';
		}
		cout<<'|';
		switch (i)
		{
		case 0:		cout<<"    ***************";break;
		case 1:		cout<<"    * minesweeper *";break;
		case 2:		cout<<"    *     "<<version<<"    *";break;
		case 3:		cout<<"    ***************";break;
		case 5:		cout<<"    up     :w";break;
		case 6:		cout<<"    down   :s";break;
		case 7:		cout<<"    left   :a";break;
		case 8:		cout<<"    right  :d";break;
		case 9:		cout<<"    flag   :j";break;
		case 10:	cout<<"    sweep  :space";break;
		case 11:	cout<<"    restart:r";break;
		case 12:	cout<<"    quit   :q";break;
		case 15:	cout<<"  rest square:"<<the_rest_of_square;break;
		case 16:	cout<<"  rest mine  :"<<the_rest_of_mine;break;
		}
		cout<<endl;
	}
	for(int j=-2;j<MAXY*3;j++)cout<<'-';
	cout<<endl;
}

bool sweep_mine(int x,int y)
{
	if(myflag[x][y])return true;
	if(firstMove)
	{
		firstMove=false;
		if(mymine[x][y])
		{
			for(int i=x-1;i<x+2;i++)
				for(int j=y-1;j<y+2;j++)
					if(j>=0&&i>=0&&j<MAXY&&i<MAXX)
						mymap[i][j]--;
			int mine_y,mine_x;
			do{
				mine_x=rand()%MAXX;
				mine_y=rand()%MAXY;
			}while(mymine[mine_x][mine_y]);
			mymine[x][y]=false;
			mymine[mine_x][mine_y]=true;
			for(int i=mine_x-1;i<mine_x+2;i++)
				for(int j=mine_y-1;j<mine_y+2;j++)
					if(j>=0&&i>=0&&j<MAXY&&i<MAXX)
						mymap[i][j]++;
		}
	}
	else if(mymine[x][y])
	{
		mysight[x][y]=true;
		return false;
	}
	bool flag=true;
	if(mysight[x][y])
	{
		int cnt=0;
		for(int i=x-1;i<x+2;i++)
			for(int j=y-1;j<y+2;j++)
				if(i>=0&&j>=0&&i<MAXX&&j<MAXY&&myflag[i][j])
					cnt++;
		if(cnt!=mymap[x][y])flag=false;
	}
	else
	{
		the_rest_of_square--;
		mysight[x][y]=true;
		if(mymap[x][y])flag=false;
	}
	if(flag)
	{
		for(int i=x-1;i<x+2;i++)
			for(int j=y-1;j<y+2;j++)
				if(i>=0&&j>=0&&i<MAXX&&j<MAXY&&!mysight[i][j])
					if(!sweep_mine(i,j))
						flag=false;
		return flag;
	}
	return true;
}

void init()
{
	firstMove=true;
	memset(mymine,false,sizeof(mymine));
	memset(mysight,false,sizeof(mysight));
	//memset(mysight,true,sizeof(mysight));
	memset(mymap,0,sizeof(mymap));
	memset(myflag,false,sizeof(myflag));
	int mine_y,mine_x;
	for(int k=0;k<MINE_NUM;k++)
	{
		mine_y=rand()%MAXY;
		mine_x=rand()%MAXX;
		if(mymine[mine_x][mine_y])k--;
		else
		{
			mymine[mine_x][mine_y]=true;
			for(int i=mine_x-1;i<mine_x+2;i++)
				for(int j=mine_y-1;j<mine_y+2;j++)
					if(j>=0&&i>=0&&j<MAXY&&i<MAXX)
						mymap[i][j]++;
		}
	}
	the_rest_of_mine=MINE_NUM;
	the_rest_of_square=MAXY*MAXX;
}

bool get_input()
{
	while(1)
	{
		c_input=getch();
		switch (c_input)
		{
		case 'w':
			if(nowx>0)nowx--;
			return true;
		case 'a':
			if(nowy>0)nowy--;
			return true;
		case 's':
			if(nowx<MAXX-1)nowx++;
			return true;
		case 'd':
			if(nowy<MAXY-1)nowy++;
			return true;
		case 'j':
			if(mysight[nowx][nowy])break;
			if(myflag[nowx][nowy]){myflag[nowx][nowy]=false;the_rest_of_mine++;the_rest_of_square++;}
			else {myflag[nowx][nowy]=true;the_rest_of_mine--;the_rest_of_square--;}
			return true;
		case ' ':
			if(myflag[nowx][nowy])break;
			return sweep_mine(nowx,nowy);
		case 'r':
			init();
			return true;
		case 'q':
			exit(0);
		}
	}
	return false;//will never run
}

bool win_game()
{
	bool wingame=true;
	for(int i=0;i<MAXX&&wingame;i++)
		for(int j=0;j<MAXY&&wingame;j++)
			if(!(mymine[i][j]||mysight[i][j]))wingame=false;
	return wingame;
}

void game_start()
{
	do{
		system("clear");
		if(win_game())
		{
			cout<<"you win!"<<endl;
			print_map(1);
			return;
		}
		print_map();
	}while(get_input());
	system("clear");
	print_map(-1);
	cout<<"you lose!"<<endl;
}

bool new_game_start()
{
	cout<<"press y to start a new game"<<endl;
	while(getch()!='y');
	return true;
}

void real_init()
{
	srand(time(NULL));
}

int main()
{
	real_init();
	do{
		init();
		game_start();
	}while(new_game_start());
	return 0;
}

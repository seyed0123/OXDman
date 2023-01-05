#include<bits/stdc++.h>
#include <conio.h>
#include<windows.h> 
#include <unistd.h>
#include<fstream>

using namespace std;
int maze[50][50]={};
struct point
{
	int x,y;
};

point input();
void pacmanMove(point &pacman , point direction , int &superpower , int record , point before);
void gostmove(point &gost , int superpower, point &pacman , point gostArr[] , int &hp , int &record , int& , point , point);
void gostINTELEGENCEmove(point &gost , int superpower, point &pacman , point gostArr[] , int &hp , int &record , int& , point , point);
int move(int , int , point , point , int);
void gostRespawn(point &gost , point gostRespawnPoint , int &numJail ,int & record);
void pacmanRespawn(point &pacman , point pacmanRespawnPoint , int &hp ,point gost[] ,int & numJail , point gostRespawnPoint);
void print( int length,int height, point pacman , point gost[]);
bool checkContradiction(point &gost , point &pacman , int superpower , int &hp , int &record ,point pacmanRespawnPoint , point gostRespawnPoint );

int main()
{	
	int l=0;
	for(int i=1; i<= 31 ; i++)
	{
		for(int j=1 ; j<=29 ; j++)
		{
			cin>>maze[i][j];
			//cout<<i<<'/'<<j<<'/'<<maze[i][j]<<endl;
			if(maze[i][j]==-3)
				break;
			l++;
		}
	}
	cout<<"\n\n\n";
	point pacmanRespawnPoint={18,15},gostRespawnPoint={14,14};
	move(31,28,pacmanRespawnPoint , gostRespawnPoint , 0);
	return 0;
}
void pacmanRespawn(point &pacman , point pacmanRespawnPoint , int &hp ,point gost[] ,int & numJail , point gostRespawnPoint )
{
	pacman=pacmanRespawnPoint;
	hp--;
	numJail=0;
	for(int i = 1 ; i <= 4 ; i++)
	{
		gost[i]=gostRespawnPoint;
	}
	system("cls");
	cout<<"you are busted \n";
	sleep(2);
	return;
}
void gostRespawn(point &gost , point gostRespawnPoint , int &numJail ,int & record)
{
	gost=gostRespawnPoint;
	numJail++;
	record+=200;
	cout<<"+200 point\n";
	sleep(2);
	return;
}
bool checkContradiction(point &gost ,point gostArr[] , point &pacman , int superpower , int &hp , int &record ,int & numJail,point pacmanRespawnPoint , point gostRespawnPoint )
{
	if((gost.x==pacman.x) && (gost.y==pacman.y))
	{
		if(superpower>0)
		{
			gostRespawn( gost , gostRespawnPoint , numJail , record);
		}else
		{
			pacmanRespawn( pacman ,  pacmanRespawnPoint ,  hp , gostArr , numJail , gostRespawnPoint );
		}
		return 1;
	}
	return 0;
}
void gostINTELEGENCEmove(point &gost , int superpower, point &pacman , point gostArr[]  , int &hp , int &record , int &numJail , point pacmanRespawnPoint ,point gostRespawnPoint)
{
	if(checkContradiction(gost , gostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , gostRespawnPoint))
	{
		return;
	}
	point direction={pacman.x-gost.x,pacman.y-gost.y};
	if(direction.x>0)
	{
		direction={1,0};
	}else if(direction.x==0)
	{
		if(direction.y>0)
		{
			direction={0,1};
		}else
		{
			direction={0,-1};
		}
	}else
	{
		direction={-1,0};
	}
	if(superpower>0)
	{
		direction.x*=-1;
		direction.y*=-1;
	}
	gost.x+=direction.x;
	gost.y+=direction.y;
	if(checkContradiction(gost , gostArr, pacman , superpower , hp , record , numJail , pacmanRespawnPoint , gostRespawnPoint))
	{
		return;
	}
	if(maze[gost.x][gost.y]==1)
	{
		gost.x-=direction.x;
		gost.y-=direction.y;
		return;
	}else if(maze[gost.x][gost.y]==-1)
	{
		gost.x-=direction.x;
		gost.y-=direction.y;
		return;
	}else 
	{
		for(int i = 1 ,count = 0; i <= 4 ;i++)
		{
			if((gost.x==gostArr[i].x) && (gost.y==gostArr[i].y))
			{
				count++;
			}
			if(count==2)
			{
				gost.x-=direction.x;
				gost.y-=direction.y;
				direction.x*=-1;
				direction.y*=-1;
				gost.x+=direction.x;
				gost.y+=direction.y;
				break;
			}
		}
	}
	return;
}
void gostmove(point &gost , int superpower, point &pacman , point gostArr[] , int &hp , int &record , int &numJail , point pacmanRespawnPoint , point gostRespawnPoint)
{
	if(checkContradiction(gost , gostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , gostRespawnPoint))
	{
		return;
	}	
	point direction;
	while(1)
	{	
		direction.x=(rand()%3)-1;
		if(direction.x==0)
		{
			direction.y=(rand()%3)-1;
			if(direction.y)
			{
				break;
			}
		}else
		{
			break;
		}
	}
	gost.x+=direction.x;
	gost.y+=direction.y;
	if(checkContradiction(gost , gostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , gostRespawnPoint))
	{
		return;
	}
	if(maze[gost.x][gost.y]==1)
	{
		gost.x-=direction.x;
		gost.y-=direction.y;
		return;
	}else if(maze[gost.x][gost.y]==-1)
	{
		gost.x-=direction.x;
		gost.y-=direction.y;
		return;
	}else 
	{
		for(int i = 1 ,count = 0; i <= 4 ;i++)
		{
			if((gost.x==gostArr[i].x) && (gost.y==gostArr[i].y))
			{
				count++;
			}
			if(count==2)
			{
				gost.x-=direction.x;
				gost.y-=direction.y;
				direction.x*=-1;
				direction.y*=-1;
				gost.x+=direction.x;
				gost.y+=direction.y;
				break;
			}
		}
	}
	return;
}
void pacmanMove(point &pacman , point direction , int &superpower , int record  , point before)
{
	if(maze[pacman.x][pacman.y]==2)
	{
		record++;
		maze[pacman.x][pacman.y]=0;
	}else if(maze[pacman.x][pacman.y]==3)
	{
		record+=3;
		maze[pacman.x][pacman.y]=0;
		superpower=10;
	}
	pacman.x+=direction.x;
	pacman.y+=direction.y;
	if(maze[pacman.x][pacman.y]==1)
	{
		pacman.x-=direction.x;
		pacman.y-=direction.y;
		pacman.x+=before.x;
		pacman.y+=before.y;
		if(maze[pacman.x][pacman.y]==1)
		{
			pacman.x-=before.x;
			pacman.y-=before.y;
		}
	}else if(maze[pacman.x][pacman.y]==2)
	{
		record++;
		maze[pacman.x][pacman.y]=0;
	}else if(maze[pacman.x][pacman.y]==-1)
	{
		pacman.x-=direction.x;
		pacman.y-=direction.y;
		pacman.x+=before.x;
		pacman.y+=before.y;
		if(maze[pacman.x][pacman.y]==1)
		{
			pacman.x-=before.x;
			pacman.y-=before.y;
		}
	}else if(maze[pacman.x][pacman.y]==3)
	{
		record+=3;
		maze[pacman.x][pacman.y]=0;
		superpower=10;
	}
	return;
}
point input()
{
	point inp;
	char input;
	input = (char)getch();
	if(input == 'a')
	{
		inp.x=0;
		inp.y=-1;
	}else if(input == 'w')
	{
		inp.x=-1;
		inp.y=0;
	}
	else if(input == 's')
	{
		inp.x=1;
		inp.y=0;
	}else if(input == 'd')
	{
		inp.x=0;
		inp.y=1;
	}else if(input=='p')
	{
		input=(char )getch();
		inp.x=0;
		inp.y=0;
	}else
	{
		inp.x=0;
		inp.y=0;
	}
	return inp;
}
void print(int length,int height, point pacman , point gost[])
{

	char temp[length][height];
	for(int i = 1 ; i <= length ; i++)
	{
		for(int j = 1 ; j <= height ; j++)
		{
			if(maze[i][j]==0)
			{
				temp[i][j]=' ';
			}else if(maze[i][j]==1)
			{
				temp[i][j]='|';
			}else if(maze[i][j]==2)
			{
				temp[i][j]='.';
			}else if(maze[i][j]==3)
			{
				temp[i][j]='*';
			}else if(maze[i][j]==-1)
			{
				temp[i][j]='-';
			}
		}
	}
	temp[pacman.x][pacman.y]='p';
	temp[gost[1].x][gost[1].y]='g';
	temp[gost[2].x][gost[2].y]='g';
	temp[gost[3].x][gost[3].y]='g';
	temp[gost[4].x][gost[4].y]='g';
	for(int i = 1 ; i <= length ; i++)
	{
		for(int j = 1 ; j <= height ; j++)
		{
			cout<<temp[i][j];
		}
		cout<<endl;
	}
}
int move(int length,int  height ,point  pacmanRespawnPoint ,point gostRespawnPoint ,int  level )
{
	srand(time(0));
	int hp=3,superpower=0,numJail=0,counterJail=0,record=0;
	point pacman=pacmanRespawnPoint,gost[5]={},direction={0,1};
	gost[1]=gostRespawnPoint;
	gost[2]=gostRespawnPoint;
	gost[3]=gostRespawnPoint;
	gost[4]=gostRespawnPoint;
	cout<<"press w to start.\n";
	sleep(2);
	while(1)
	{
		print(length, height , pacman , gost);
		usleep(500000);
		system("cls");
		point before=direction;
		if(kbhit())
		{
			direction = input();
			if(direction.x==0 && direction.y == 0 )
			{
				cout<<"invalid \n";
				continue;
			}
		}
		pacmanMove(pacman , direction, superpower ,record , before);
		if(superpower>0)
		{
			superpower--;
		}
		counterJail++;
		for(int i = 1 ; i<= -1*numJail ; i++)
		{
			if(checkContradiction(gost[i] , gost , pacman , superpower , hp , record, numJail , pacmanRespawnPoint , gostRespawnPoint))
			{
				continue;
			}
		}
		pacmanMove(pacman , direction, superpower ,record , before);
		if(numJail!=-4)
		{
			if(counterJail==5)
			{	
				counterJail=0;
				numJail--;
				gost[-1*numJail]={gostRespawnPoint.x-2,gostRespawnPoint.y};
			}
		}
		for(int i = 1 ; i <= -1*numJail ; i++ )
		{
			if(i <= level)
			{
				gostINTELEGENCEmove(gost[i] ,  superpower,  pacman ,  gost  , hp ,record , numJail , pacmanRespawnPoint , gostRespawnPoint);
			}else
			{
				gostmove(gost[i] ,superpower, pacman , gost , hp , record ,  numJail ,  pacmanRespawnPoint , gostRespawnPoint);
			}
		}
		if(hp==0)
		{
			sleep(3);
			return;
		}
	}
	return level*record;
}
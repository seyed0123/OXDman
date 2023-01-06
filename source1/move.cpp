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
void pacmanMove(point &pacman , point &direction , int &superpower , int record , point before);
void gostmove(point &gost , int superpower, point &pacman , point gostArr[] , int &hp , int &record , int& , point , point , point*);
void gostINTELEGENCEmove(point &gost , int superpower, point &pacman , point gostArr[] , int &hp , int &record , int& , point , point , point & , point*);
int move(int , int , point , point , int);
void gostRespawn(point &gost , point gostRespawnPoint , int &numJail ,int & record , point);
void pacmanRespawn(point &pacman , point pacmanRespawnPoint , int &hp ,point gost[] ,int & numJail , point gostRespawnPoint , point*);
bool print( int length,int height, point pacman , point gost[]);
bool checkContradiction(point &gost , point &pacman , int superpower , int &hp , int &record ,point pacmanRespawnPoint , point gostRespawnPoint , point*);

int main()
{	
	for(int i=1; i<= 31 ; i++)
	{
		for(int j=1 ; j<=29 ; j++)
		{
			cin>>maze[i][j];
			if(maze[i][j]==-3)
				break;
		}
	}
	cout<<"\n\n\n";
	point pacmanRespawnPoint={18,15},gostRespawnPoint={14,14};

	int finalrecord;
	for(int i=4 ; i <= 4 ; i++ )
	{
		int record=move(31,28,pacmanRespawnPoint , gostRespawnPoint , i);
		if(record==-10)
		{
			break;
		}else
		{
			finalrecord+=record;
		}
	}

	return 0;
}
void pacmanRespawn(point &pacman , point pacmanRespawnPoint , int &hp ,point gost[] ,int & numJail , point gostRespawnPoint )
{
	pacman=pacmanRespawnPoint;
	hp--;
	numJail=0;
	gost[1].x=gostRespawnPoint.x;
	gost[1].y=gostRespawnPoint.y;
	gost[2].x=gostRespawnPoint.x+1;
	gost[2].y=gostRespawnPoint.y;
	gost[3].x=gostRespawnPoint.x;
	gost[3].y=gostRespawnPoint.y+1;
	gost[4].x=gostRespawnPoint.x+1;
	gost[4].y=gostRespawnPoint.y+1;
	system("cls");
	cout<<"you are busted \n";
	sleep(2);
	return;
}
void gostRespawn(point &gost , point gostRespawnPoint , int &numJail ,int & record , point gostbefore)
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
void gostINTELEGENCEmove(point &gost , int superpower, point &pacman , point gostArr[]  , int &hp , int &record , int &numJail , point pacmanRespawnPoint ,point gostRespawnPoint , point &gostbefore)
{
	if(checkContradiction(gost , gostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , gostRespawnPoint))
	{
		return;
	}
	
	point* aim = new point [5];
	point direction=gostbefore;
	int count=0 , defaly = 0 , defalx = 0;
	if(maze[gost.x+1][gost.y]==2 ||maze[gost.x+1][gost.y]==3 ||maze[gost.x+1][gost.y]==0 )
	{
		count++;
		aim[count]={1,0};
		defalx=1;
	}
	if(maze[gost.x][gost.y+1]==2 ||maze[gost.x][gost.y+1]==3 || maze[gost.x][gost.y+1]==0 )
	{
		count++;
		aim[count]={0,1};
		defaly =1;
	}
	if(maze[gost.x-1][gost.y]==2 ||maze[gost.x-1][gost.y]==3 || maze[gost.x-1][gost.y]==0 )
	{
		count++;
		aim[count]={-1,0};
		defalx=1;
	}
	if(maze[gost.x][gost.y-1]==2 ||maze[gost.x][gost.y-1]==3 || maze[gost.x][gost.y-1]==0 )
	{
		count++;
		aim[count]={0,-1};
		defaly=1;
	}

	if(count>2 || (count==2 && defalx==1 && defaly == 1))
	{
		pair<int,int> minimum={100,0};
		for(int i = 1 ; i <= count ; i++)
		{
			//cout<<aim[i].x << ' '<< aim[i].y<< endl;
			if(minimum.first > abs(pacman.x-gost.x-aim[i].x)+abs(pacman.y-gost.y-aim[i].y) )
			{
				minimum={abs(pacman.x-gost.x-aim[i].x)+abs(pacman.y-gost.y-aim[i].y) , i};
			}
		}
		//cout<<direction.x << ','<< direction.y<< endl;
		direction=aim[minimum.second];
		//sleep(2);
	}
	
	if(superpower>0)
	{
		direction.x*=-1;
		direction.y*=-1;
	}
	
	gostbefore=direction;
	gost.x+=direction.x;
	gost.y+=direction.y;
	
	if(checkContradiction(gost , gostArr, pacman , superpower , hp , record , numJail , pacmanRespawnPoint , gostRespawnPoint))
	{
		return;
	}
		
	for(int i = 1 , counter = 0; i <= 4 ;i++)
	{	
		
		if((gost.x==gostArr[i].x) && (gost.y==gostArr[i].y))
		{
			counter++;
		}
		if(counter==2)
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
	if(maze[gost.x][gost.y]==1 || maze[gost.x][gost.y]==-1)
	{
		gost.x-=direction.x;
		gost.y-=direction.y;
	}
	gostbefore=direction;
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
		direction.y=0;
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
void pacmanMove(point &pacman , point &direction , int &superpower , int record  , point before)
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
		direction=before;
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
		direction=before;
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
bool print(int length,int height, point pacman , point gost[])
{
	bool ret=0;
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
				ret=1;
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
	temp[gost[1].x][gost[1].y]='a';
	temp[gost[2].x][gost[2].y]='b';
	temp[gost[3].x][gost[3].y]='c';
	temp[gost[4].x][gost[4].y]='d';
	
	for(int i = 1 ; i <= length ; i++)
	{
		for(int j = 1 ; j <= height ; j++)
		{
			printf("%c",temp[i][j]);
		}
		cout<<endl;
	}
	return ret;
}
int move(int length,int  height ,point  pacmanRespawnPoint ,point gostRespawnPoint ,int  level )
{
	srand(time(0));
	int hp=3,superpower=0,numJail=0,counterJail=0,record=0;
	point* gostbefore=new point [5];
	point* gost =new point [5];
	point pacman=pacmanRespawnPoint,direction={0,1};
	gostbefore[1]={0,-1};
	gostbefore[2]={0,1};
	gostbefore[3]={0,-1};
	gostbefore[4]={0,1};

	gost[1].x=gostRespawnPoint.x;
	gost[1].y=gostRespawnPoint.y;
	gost[2].x=gostRespawnPoint.x+1;
	gost[2].y=gostRespawnPoint.y;
	gost[3].x=gostRespawnPoint.x;
	gost[3].y=gostRespawnPoint.y+1;
	gost[4].x=gostRespawnPoint.x+1;
	gost[4].y=gostRespawnPoint.y+1;

	cout<<"press w to start.\n";
	sleep(2);
	while(1)
	{
		if(!print(length, height , pacman , gost))
		{
			break;
		}
		usleep(16666);
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

		for(int i = 1 ; i<= -1*numJail ; i++)
		{
			if(checkContradiction(gost[i] , gost , pacman , superpower , hp , record, numJail , pacmanRespawnPoint , gostRespawnPoint ,gostbefore))
			{
				continue;
			}
		}

		if(!print(length, height , pacman , gost))
		{
			break;
		}
		usleep(16666);
		system("cls");
		before=direction;
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

		if(numJail!=-4)
		{
			if(counterJail>=10-(2*level)) 
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
				gostINTELEGENCEmove(gost[i] ,  superpower,  pacman ,  gost  , hp ,record , numJail , pacmanRespawnPoint , gostRespawnPoint , gostbefore[i] ,gostbefore);
			}else
			{
				gostmove(gost[i] ,superpower, pacman , gost , hp , record ,  numJail ,  pacmanRespawnPoint , gostRespawnPoint , gostbefore);
			}
		}
		
		if(hp==0)
		{
			cout<<"Shame on you.\nYou have disappointed all the people of Pacman land, prepare yourself for a heavy punishment.\n";
			sleep(3);
			return -10;
		}
	}
	cout<<"congratulations!!!!!\nyou pass this round.\n You will be given a tytab.\n";
	sleep(3);
	return level*record;
}
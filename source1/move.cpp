#include <bits/stdc++.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h> 
#endif

#ifdef linux
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}
#endif

#include <unistd.h>
#include <fstream>

using namespace std;
int maze[50][50]={};

struct point
{
	int x,y;
	bool operator==(point secondPoint) {
		if(x == secondPoint.x && y == secondPoint.y) {
			return true;
		} else {
			return false;
		}
	}
};

point input();
void pacmanMove(point &pacman , point &direction , int &superpower , int record , point before);
void ghostmove(point &ghost , int superpower, point &pacman , point ghostArr[] , int &hp , int &record , int& , point , point , point*);
void ghostINTELEGENCEmove(point &ghost , int superpower, point &pacman , point ghostArr[] , int &hp , int &record , int& , point , point , point & , point*);
int move(int , int , point , point , int);
void ghostRespawn(point &ghost , point ghostRespawnPoint , int &numJail ,int & record , point&);
void pacmanRespawn(point &pacman , point pacmanRespawnPoint , int &hp ,point ghost[] ,int & numJail , point ghostRespawnPoint , point*);
bool print( int length,int height, point pacman , point ghost[]);
bool checkContradiction(point &ghost , point &pacman , int superpower , int &hp , int &record ,point pacmanRespawnPoint , point ghostRespawnPoint , point& , point*);

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
	point pacmanRespawnPoint={18,15},ghostRespawnPoint={14,14};

	int finalrecord;
	for(int i=0 ; i <= 4 ; i++ )
	{
		int record=move(31,28,pacmanRespawnPoint , ghostRespawnPoint , i);
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
void pacmanRespawn(point &pacman , point pacmanRespawnPoint , int &hp ,point ghost[] ,int & numJail , point ghostRespawnPoint  , point ghostBeforeArr[])
{
	pacman=pacmanRespawnPoint;
	hp--;
	numJail=0;
	ghostBeforeArr[1]={0,-1};
	ghostBeforeArr[2]={0,1};
	ghostBeforeArr[3]={0,-1};
	ghostBeforeArr[4]={0,1};

	ghost[1].x=ghostRespawnPoint.x;
	ghost[1].y=ghostRespawnPoint.y;
	ghost[2].x=ghostRespawnPoint.x+1;
	ghost[2].y=ghostRespawnPoint.y;
	ghost[3].x=ghostRespawnPoint.x;
	ghost[3].y=ghostRespawnPoint.y+1;
	ghost[4].x=ghostRespawnPoint.x+1;
	ghost[4].y=ghostRespawnPoint.y+1;
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
	cout<<"you are busted \n";
	sleep(2);
	return;
}
void ghostRespawn(point &ghost , point ghostRespawnPoint , int &numJail ,int & record , point &ghostbefore)
{
	ghost=ghostRespawnPoint;
	ghostbefore.x=0;
	ghostbefore.y=-1;

	numJail++;
	record+=200;
	cout<<"+200 point\n";
	sleep(2);
	return;
}
bool checkContradiction(point &ghost ,point ghostArr[] , point &pacman , int superpower , int &hp , int &record ,int & numJail,point pacmanRespawnPoint , point ghostRespawnPoint , point &ghostbefore ,point ghostBeforeArr[] )
{
	if((ghost.x==pacman.x) && (ghost.y==pacman.y))
	{
		if(superpower>0)
		{
			if((ghostbefore.x==ghostBeforeArr[0].x)&&(ghostbefore.y==ghostBeforeArr[0].y))
			{
				int index;
				for(int i = 1; i <= 4; i++) {
					if((ghost.x==ghostBeforeArr[i].x) && (ghost.y==ghostBeforeArr[i].y)) {
						index = i;
						break;
					}
				}
				ghostRespawn( ghost , ghostRespawnPoint , numJail , record , ghostBeforeArr[index]);
			} else {
				ghostRespawn( ghost , ghostRespawnPoint , numJail , record , ghostbefore);
			}
		}else
		{
			pacmanRespawn( pacman ,  pacmanRespawnPoint ,  hp , ghostArr , numJail , ghostRespawnPoint , ghostBeforeArr);
		}
		return 1;
	}
	return 0;
}
void ghostINTELEGENCEmove(point &ghost , int superpower, point &pacman , point ghostArr[]  , int &hp , int &record , int &numJail , point pacmanRespawnPoint ,point ghostRespawnPoint , point &ghostbefore , point ghostbeforeArr[])
{
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr))
	{
		return;
	}
	
	point* aim = new point [5];
	point direction=ghostbefore;
	int count=0 , defally = 0 , defallx = 0;
	if(maze[ghost.x+1][ghost.y]==2 ||maze[ghost.x+1][ghost.y]==3 ||maze[ghost.x+1][ghost.y]==0 )
	{
		count++;
		aim[count]={1,0};
		defallx=1;
	}
	if(maze[ghost.x][ghost.y+1]==2 ||maze[ghost.x][ghost.y+1]==3 || maze[ghost.x][ghost.y+1]==0 )
	{
		count++;
		aim[count]={0,1};
		defally =1;
	}
	if(maze[ghost.x-1][ghost.y]==2 ||maze[ghost.x-1][ghost.y]==3 || maze[ghost.x-1][ghost.y]==0 )
	{
		count++;
		aim[count]={-1,0};
		defallx=1;
	}
	if(maze[ghost.x][ghost.y-1]==2 ||maze[ghost.x][ghost.y-1]==3 || maze[ghost.x][ghost.y-1]==0 )
	{
		count++;
		aim[count]={0,-1};
		defally=1;
	}

	if(count>2 || (count==2 && defallx==1 && defally == 1))
	{
		pair<int,int> minimum={pacman.x-ghost.x,0};
		int* arr = new int [5];
		for(int i = 1 ; i <= count ; i++)
		{
			//cout<<aim[i].x << ' '<< aim[i].y<< endl;
			if((minimum.first-abs(pacman.x-ghost.x-aim[i].x)-abs(pacman.y-ghost.y-aim[i].y))>=0)
			{
				minimum.second++;
				arr[minimum.second]=i;
			}
		}
		if(minimum.second != 0) {
			int ra=(rand()%minimum.second) +1;
			direction=aim[arr[ra]];
		} else {
			direction=aim[1];
		}

		//cout<<direction.x << ','<< direction.y<< endl;
		//sleep(2);
		delete[] arr;
	}
	
	if(superpower>0)
	{
		direction.x*=-1;
		direction.y*=-1;
	}
	
	ghostbefore=direction;
	ghost.x+=direction.x;
	ghost.y+=direction.y;
	
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record , numJail , pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr))
	{
		return;
	}
		
	for(int i = 1 , counter = 0; i <= 4 ;i++)
	{	
		
		if((ghost.x==ghostArr[i].x) && (ghost.y==ghostArr[i].y))
		{
			counter++;
		}
		if(counter==2)
		{
			ghost.x-=direction.x;
			ghost.y-=direction.y;
			direction.x*=-1;
			direction.y*=-1;
			ghost.x+=direction.x;
			ghost.y+=direction.y;
			
			break;
		}	
	}
	if(maze[ghost.x][ghost.y]==1 || maze[ghost.x][ghost.y]==-1)
	{
		ghost.x-=direction.x;
		ghost.y-=direction.y;
	}
	ghostbefore=direction;
	delete[] aim;
	return;
}
void ghostmove(point &ghost , int superpower, point &pacman , point ghostArr[] , int &hp , int &record , int &numJail , point pacmanRespawnPoint , point ghostRespawnPoint , point &ghostbefore  , point ghostbeforeArr[])
{
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr))
	{
		return;
	}	

	point* aim = new point [5];
	point direction=ghostbefore;
	int count=0 , defally = 0 , defallx = 0;
	if(maze[ghost.x+1][ghost.y]==2 ||maze[ghost.x+1][ghost.y]==3 ||maze[ghost.x+1][ghost.y]==0 )
	{
		count++;
		aim[count]={1,0};
		defallx=1;
	}
	if(maze[ghost.x][ghost.y+1]==2 ||maze[ghost.x][ghost.y+1]==3 || maze[ghost.x][ghost.y+1]==0 )
	{
		count++;
		aim[count]={0,1};
		defally =1;
	}
	if(maze[ghost.x-1][ghost.y]==2 ||maze[ghost.x-1][ghost.y]==3 || maze[ghost.x-1][ghost.y]==0 )
	{
		count++;
		aim[count]={-1,0};
		defallx=1;
	}
	if(maze[ghost.x][ghost.y-1]==2 ||maze[ghost.x][ghost.y-1]==3 || maze[ghost.x][ghost.y-1]==0 )
	{
		count++;
		aim[count]={0,-1};
		defally=1;
	}
	if(count>2 || (count==2 && defallx==1 && defally == 1)) {
		int temp = (rand() % count) + 1;
		direction = aim[temp];
		ghostbefore = direction;
	}
	ghost.x+=direction.x;
	ghost.y+=direction.y;
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr))
	{
		return;
	}
	if(maze[ghost.x][ghost.y]==1)
	{
		ghost.x-=direction.x;
		ghost.y-=direction.y;
		return;
	}else if(maze[ghost.x][ghost.y]==-1)
	{
		ghost.x-=direction.x;
		ghost.y-=direction.y;
		return;
	}else 
	{
		for(int i = 1 ,count = 0; i <= 4 ;i++)
		{
			if((ghost.x==ghostArr[i].x) && (ghost.y==ghostArr[i].y))
			{
				count++;
			}
			if(count==2)
			{
				ghost.x-=direction.x;
				ghost.y-=direction.y;
				direction.x*=-1;
				direction.y*=-1;
				ghost.x+=direction.x;
				ghost.y+=direction.y;
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
bool print(int length,int height, point pacman , point ghost[])
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
	temp[ghost[1].x][ghost[1].y]='a';
	temp[ghost[2].x][ghost[2].y]='b';
	temp[ghost[3].x][ghost[3].y]='c';
	temp[ghost[4].x][ghost[4].y]='d';
	
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
int move(int length,int  height ,point  pacmanRespawnPoint ,point ghostRespawnPoint ,int  level )
{
	srand(time(0));
	int hp=3,superpower=0,numJail=0,counterJail=0,record=0;
	point* ghostbefore=new point [5];
	point* ghost =new point [5];
	point pacman=pacmanRespawnPoint,direction={0,1};
	ghostbefore[1]={0,-1};
	ghostbefore[2]={0,1};
	ghostbefore[3]={0,-1};
	ghostbefore[4]={0,1};

	ghost[1].x=ghostRespawnPoint.x;
	ghost[1].y=ghostRespawnPoint.y;
	ghost[2].x=ghostRespawnPoint.x+1;
	ghost[2].y=ghostRespawnPoint.y;
	ghost[3].x=ghostRespawnPoint.x;
	ghost[3].y=ghostRespawnPoint.y+1;
	ghost[4].x=ghostRespawnPoint.x+1;
	ghost[4].y=ghostRespawnPoint.y+1;

	cout<<"press w to start.\n";
	sleep(2);
	while(1)
	{
		if(!print(length, height , pacman , ghost))
		{
			break;
		}
		usleep(50000);
		#ifdef _WIN32
			system("cls");
		#else
			system("clear");
		#endif
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
			if(checkContradiction(ghost[i] , ghost , pacman , superpower , hp , record, numJail , pacmanRespawnPoint , ghostRespawnPoint ,ghostbefore[0] , ghostbefore))
			{
				continue;
			}
		}

		if(!print(length, height , pacman , ghost))
		{
			break;
		}
		usleep(50000);
		#ifdef _WIN32
			system("cls");
		#else
			system("clear");
		#endif
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
				ghost[-1*numJail]={ghostRespawnPoint.x-2,ghostRespawnPoint.y};
			}
		}
		
		for(int i = 1 ; i <= -1*numJail ; i++ )
		{
			if(i <= level)
			{
				ghostINTELEGENCEmove(ghost[i] ,  superpower,  pacman ,  ghost  , hp ,record , numJail , pacmanRespawnPoint , ghostRespawnPoint , ghostbefore[i] ,ghostbefore);
			}else
			{
				ghostmove(ghost[i] ,superpower, pacman , ghost , hp , record ,  numJail ,  pacmanRespawnPoint , ghostRespawnPoint , ghostbefore[i] , ghostbefore);
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
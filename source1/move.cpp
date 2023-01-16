// standard library
#include <bits/stdc++.h>
#include <unistd.h>
#include <fstream>


#include "Vector.cpp"
#include "Maze.cpp"

// Adding terminal interactivity depending on the OS (Mac currently not supported)
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

using namespace std;
Maze maze;


struct point
{
	int x,y;
};

point input();
void pacmanMove(point &pacman , point &direction , int &superpower , int record , point before );
void ghostmove(point &ghost , int superpower, point &pacman , point ghostArr[] , int &hp , int &record , int& , point , point , point* ,int *);
void ghostINTELEGENCEmove(point &ghost , int superpower, point &pacman , point ghostArr[] , int &hp , int &record , int& , point , point , point & , point*  , int *);
int move(int , int , point , point , int);
void ghostRespawn(point &ghost , point ghostRespawnPoint , int &numJail ,int & record , point&, int* , int );
void pacmanRespawn(point &pacman , point pacmanRespawnPoint , int &hp ,point ghost[] ,int & numJail , point ghostRespawnPoint , point* , int*);
bool print( int length,int height, point pacman , point ghost[], int superpower , int record , int  hp);
bool checkContradiction(point &ghost , point &pacman , int superpower , int &hp , int &record ,point pacmanRespawnPoint , point ghostRespawnPoint , point& , point* , int*);

int main()
{	
	int width = 25;
	width -= width % 2;
	int height = 30;

	ofstream recordsFile;
	recordsFile.open("records.save", ios::app);
	int finalRecord=0;
	for(int i=4 ; i <= 8 ; i+=2 )
	{
		maze = createDesirableMaze(width, height);
		point pacmanRespawnPoint={height-2,width/2},ghostRespawnPoint={(height-3)/2-1,width/2-1};
		int record=move(height,width,pacmanRespawnPoint , ghostRespawnPoint , i);
		if(record==-10)
		{
			break;
		}else
		{
			finalRecord+=record;
		}
	}
	if(recordsFile) {
		recordsFile<<finalRecord<<endl;
		recordsFile.close();
	}

	return 0;
}
void pacmanRespawn(point &pacman , point pacmanRespawnPoint , int &hp ,point ghost[] ,int & numJail , point ghostRespawnPoint  , point ghostBeforeArr[] , int jail [])
{
	pacman=pacmanRespawnPoint;
	hp--;
	numJail=0;
	ghostBeforeArr[1]={0,-1};
	ghostBeforeArr[2]={0,1};
	ghostBeforeArr[3]={0,-1};
	ghostBeforeArr[4]={0,1};
	
	jail[1]=1;
	jail[2]=2;
	jail[3]=3;
	jail[4]=4;

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
void ghostRespawn(point &ghost , point ghostRespawnPoint , int &numJail ,int & record , point &ghostbefore , int jail[] , int index)
{
	ghost=ghostRespawnPoint;
	ghostbefore.x=0;
	ghostbefore.y=-1;
	for(int i = 1 ; i<= 4 ; i++ )
	{
		if(jail[i]==0)
		{
			jail[i]=index;
			break;
		}
	}
	numJail++;
	record+=200;
	cout<<"+200 point\n";
	sleep(2);	return;
}
bool checkContradiction(point &ghost ,point ghostArr[] , point &pacman , int superpower , int &hp , int &record ,int & numJail,point pacmanRespawnPoint , point ghostRespawnPoint , point &ghostbefore ,point ghostBeforeArr[] ,int  jail[] )
{
	if((ghost.x==pacman.x) && (ghost.y==pacman.y))
	{
		if(superpower>0)
		{
			int index=0;
			for(int i = 1; i <= 4; i++) 
			{
				if((ghost.x==ghostArr[i].x) && (ghost.y==ghostArr[i].y)) {
					index = i;
					break;
				}
			}
			if((ghostbefore.x==ghostBeforeArr[0].x)&&(ghostbefore.y==ghostBeforeArr[0].y))
			{
				ghostRespawn( ghost , ghostRespawnPoint , numJail , record , ghostBeforeArr[index] , jail , index);
			} else 
			{
				ghostRespawn( ghost , ghostRespawnPoint , numJail , record , ghostbefore , jail , index );
			}
		}else
		{
			pacmanRespawn( pacman ,  pacmanRespawnPoint ,  hp , ghostArr , numJail , ghostRespawnPoint , ghostBeforeArr , jail );
		}
		return 1;
	}
	return 0;
}
void superghost(point &ghost , int superpower, point &pacman , point ghostArr[] , int &hp , int &record , int &numJail , point pacmanRespawnPoint , point ghostRespawnPoint , point &ghostbefore  , point ghostbeforeArr[] , int jail[])
{
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr , jail ))
	{
		return;
	}	
	point direction={0,0};
	if(pacman.x>ghost.x)
	{
		direction.x=1;
	}else if( pacman.x == ghost.x )
	{
		direction.x=0;
	}else 
	{
		direction.x=-1;
	}
	
	if(pacman.y>ghost.y)
	{
		direction.y=1;
	}else if( pacman.y == ghost.y)
	{
		direction.y=0;
	}else 
	{
		direction.y=-1;
	}
	ghost.x+=direction.x;
	ghost.y+=direction.y;
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr , jail))
	{
		return;
	} 
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
	if(maze[ghost.x][ghost.y]==1 || maze[ghost.x][ghost.y]==-1)
	{
		ghost.x-=direction.x;
		ghost.y-=direction.y;
	}
	return;
}
void ghostINTELEGENCEmove(point &ghost , int superpower, point &pacman , point ghostArr[]  , int &hp , int &record , int &numJail , point pacmanRespawnPoint ,point ghostRespawnPoint , point &ghostbefore , point ghostbeforeArr[] , int jail[])
{
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr , jail ))
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
		int numOfMin = 0;
		int* arr = new int [5];
		pair <int,int> minimum ={pacman.x-ghost.x,pacman.y-ghost.y};
		for(int i = 1 ; i <= count ; i++)
		{
			//cout<<aim[i].x << ' '<< aim[i].y<< endl;
			if(aim[i].x*minimum.first > 0 || aim[i].y * minimum.second > 0 )
			{
				numOfMin++;
				arr[numOfMin]=i;
			}
		}
		if(numOfMin != 0) {
			int ra=(rand()%numOfMin) +1;
			direction=aim[arr[ra]];
		} else {
			int ra=(rand()% count)+1;
			direction=aim[ra];
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
	
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record , numJail , pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr , jail ))
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
void ghostmove(point &ghost , int superpower, point &pacman , point ghostArr[] , int &hp , int &record , int &numJail , point pacmanRespawnPoint , point ghostRespawnPoint , point &ghostbefore  , point ghostbeforeArr[] , int jail[])
{
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr , jail ))
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
	if(checkContradiction(ghost , ghostArr, pacman , superpower , hp , record ,numJail, pacmanRespawnPoint , ghostRespawnPoint , ghostbefore , ghostbeforeArr , jail))
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
	if(maze[ghost.x][ghost.y]==1 || maze[ghost.x][ghost.y]==-1)
	{
		ghost.x-=direction.x;
		ghost.y-=direction.y;
	}
	delete [] aim;
	return;
}
void pacmanMove(point &pacman , point &direction , int &superpower , int record  , point before)
{
	if(maze[pacman.x][pacman.y]==2)
	{
		record++;
		maze.change(pacman.x, pacman.y, 0);
	}else if(maze[pacman.x][pacman.y]==3)
	{
		record+=3;
		maze.change(pacman.x, pacman.y, 0);
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
		maze.change(pacman.x, pacman.y, 0);
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
		maze.change(pacman.x, pacman.y, 0);
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
bool print(int length,int height, point pacman , point ghost[] , int superpower , int record , int  hp)
{
	bool ret=0;
	char temp[length][height];
	
	for(int i = 0 ; i < length ; i++)
	{
		for(int j = 0 ; j < height ; j++)
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
				ret=1;
				temp[i][j]='*';
			}else if(maze[i][j]==-1)
			{
				temp[i][j]='-';
			}
		}
	}
	
	if(superpower)
	{
		temp[pacman.x][pacman.y]='S';
		temp[ghost[1].x][ghost[1].y]='g';
		temp[ghost[2].x][ghost[2].y]='g';
		temp[ghost[3].x][ghost[3].y]='g';
		temp[ghost[4].x][ghost[4].y]='g';	
	}else{
		temp[pacman.x][pacman.y]='p';
		temp[ghost[1].x][ghost[1].y]='g';
		temp[ghost[2].x][ghost[2].y]='g';
		temp[ghost[3].x][ghost[3].y]='g';
		temp[ghost[4].x][ghost[4].y]='g';
	}
	
	for(int i = 0 ; i < length ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			printf("%c",temp[i][j]);
		}
		cout<<endl;
	}
	cout<<"record: " << record << " HP: " << hp <<" superpower: " << superpower<<endl;
	return ret;
}
int move(int length,int  height ,point  pacmanRespawnPoint ,point ghostRespawnPoint ,int  level )
{
	srand(time(0));
	int hp=3,superpower=0,numJail=0,counterJail=0,record=0;
	int* jail = new int [5];
	point* ghostbefore=new point [5];
	point* ghost =new point [5];
	point pacman=pacmanRespawnPoint,direction={0,1};
	ghostbefore[1]={0,-1};
	ghostbefore[2]={0,1};
	ghostbefore[3]={0,-1};
	ghostbefore[4]={0,1};

	jail[1]=1;
	jail[2]=2;
	jail[3]=3;
	jail[4]=4;
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
		if(!print(length, height , pacman , ghost , superpower , record , hp))
		{
			break;
		}
		usleep(180000);
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
			if(checkContradiction(ghost[i] , ghost , pacman , superpower , hp , record, numJail , pacmanRespawnPoint , ghostRespawnPoint ,ghostbefore[0] , ghostbefore , jail))
			{
				continue;
			}
		}

		if(!print(length, height , pacman , ghost ,superpower , record ,hp))
		{
			break;
		}
		usleep(180000);
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
			if(counterJail>=10-sqrt(level)) 
			{		
				counterJail=0;
				numJail--;
				int index;
				for ( int i = 4 ; i >=1 ; i-- )
				{
					if(jail[i]!= 0 )
					{
						index=jail[i];
						jail[i]=0;
						break;
					}
				}
				ghost[index]={ghostRespawnPoint.x-2,ghostRespawnPoint.y};
			}
		}
		
		for(int i = 1 ; i <= 4 ; i++ )
		{
			bool free=1;
			for( int j = 1 ; j <= 4 ; j++)
			{
				if( i == jail[j])
				{
					free=0;
					break;
				}
			}
			if(free)
			{
				if( level<=4)
				{
					if(i <= level)
					{
						ghostINTELEGENCEmove(ghost[i] ,  superpower,  pacman ,  ghost  , hp ,record , numJail , pacmanRespawnPoint , ghostRespawnPoint , ghostbefore[i] ,ghostbefore , jail );
					}else
					{
						ghostmove(ghost[i] ,superpower, pacman , ghost , hp , record ,  numJail ,  pacmanRespawnPoint , ghostRespawnPoint , ghostbefore[i] , ghostbefore , jail );
					}
				}else
				{
					if(i <= level-4)
					{
						superghost(ghost[i] ,superpower, pacman , ghost , hp , record ,  numJail ,  pacmanRespawnPoint , ghostRespawnPoint , ghostbefore[i] , ghostbefore , jail );
					}else
					{
						ghostINTELEGENCEmove(ghost[i] ,  superpower,  pacman ,  ghost  , hp ,record , numJail , pacmanRespawnPoint , ghostRespawnPoint , ghostbefore[i] ,ghostbefore , jail );
					}
				}
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
	if(level == 0) {
		return record;
	} else {
		return level*record;
	}
}
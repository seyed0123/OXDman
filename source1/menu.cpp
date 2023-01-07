#include<bits/stdc++.h>

using namespace std;

void showMenu();
void printpacman();
void ranks();

int main()
{

	return 0;
}
void ranks()
{
	system("cls");
	string line;
	ifstream file("ranks.pacman");
	if(file.is_open())
	{

		return ;
	}else
	{
		cout<<"sorry something went wrong so your load failed :(((\n";
		return ;
	}
}
void printpacman()
{
	cout<<"       *********           **              *********             **            **                    **              **      **\n";
	cout<<"       **     **         **  **            **                  **  **        **  **                **  **            ****    **\n";
	cout<<"       **     **        **    **           **                 **    **      **     **             **    **           ** **   **\n";
	cout<<"       **     **       **********          **                **      **    **       **           **********          **  **  **\n";
	cout<<"       *********      **        **         **               **        **  **         **         **        **         **   ** **\n";
	cout<<"       **            **          **        **              **          ****          **        **          **        **    ****\n";
	cout<<"       **           **            **       **********     **            **            **      **            **       **      **\n";

}
void showMenu()
{
	printpacman();
	cout<<" 1- New game\n 2- Load a game:\n 3- Ranking\n 4- Exit\n";
	string input;
	cin>>input;
	if(input=='1')
	{
		newGame();
		return;
	}else if(input =='2')
	{
		load();
		return;
	}else if(input=='3')
	{
		ranks();
		showMenu();
	}else if(input == '4')
	{
		exit(0);
	}else
	{
		system("cls");
		cout<<"your input isn't valid.\n";
		showMenu();
	}
}
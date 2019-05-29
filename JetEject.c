#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#define EnemyNumber 16
typedef enum {false, true} bool;
struct Plane
{
	int x ;
	int y ;
	bool isalive;
};
//声明
const int MapHigh = 25,MapLong = 25,RefreshLine = 3,SpeedUpCount = 15;
int PlayerScore = 0,DelayTime = 250;
struct Plane Enemy[EnemyNumber],Player;

void ChangeObject(int s,int e);
void Operate();
void InitEnemyPlane(struct Plane* PlaneObject);
void MoveEnemyPlane(struct Plane* PlaneObject);
void PrintPlayerPlane(int x,int y);
void Gaming();
void JudgeCrash();
void Init();
void OutPutMap();
//函数定义
void gotoxy(int x, int y)
{
   COORD coord = {x, y};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
   return;
}
void Delay(int xms)
{
	int s_clock = clock(),e_clock;
	do
	{
		e_clock = clock();
	}while((e_clock - s_clock)<=xms);
	return;
}
void Operate()
{
	if(!kbhit())                //若键盘无操作则返回
	      return;
	char key =getch();          //获取按键
	int tx = 0,ty = 0;          //方向的临时储存变量
	switch(key)
	{
		case 'W':
		case 'w':ty = -1;break;
		case 'S':
		case 's':ty = 1;break;
		case 'A':
		case 'a':tx = -1;break;
		case 'D':
		case 'd':tx = 1;break;
	}
	PrintPlayerPlane(tx,ty);
}
void ChangeObject(int s,int e)      //交换两结构
{
	struct Plane temp;
	temp = Enemy[s];
	Enemy[s] = Enemy[e];
	Enemy[e] = temp;
	return;
}
void InitEnemyPlane(struct Plane* PlaneObject)
{
	PlaneObject->x = rand()%(MapLong + 1);
	PlaneObject->y = 0;
}
void MoveEnemyPlane(struct Plane* PlaneObject)
{
        gotoxy(PlaneObject->x,PlaneObject->y);
	printf(" ");                    //清除之前的位置
	PlaneObject->y++;		//下移敌机
	gotoxy(PlaneObject->x,PlaneObject->y);
	printf("X");                    //输出新的位置              
}
void PrintPlayerPlane(int x,int y)
{
	int tpx = Player.x + x,tpy = Player.y + y;
	gotoxy(Player.x,Player.y);
	printf(" ");                   //清除之前的位置
	if(tpx<0)
	      Player.x = MapLong;
	else if(tpx>MapLong)
	      Player.x = 0;
	else Player.x = tpx;

	if(tpy>0 && tpy <= MapHigh)
		Player.y = tpy;          //设置玩家位置
	
	gotoxy(Player.x,Player.y);
	printf("H");			//输出玩家位置
}
void Gaming()
{
	Delay(DelayTime);
	for(int i = 0;i< EnemyNumber;i++)
	{
		if(!Enemy[i].isalive)
		      continue;
		MoveEnemyPlane(&Enemy[i]);   //循环下移每个敌机
		if(Enemy[i].y == RefreshLine )//当有敌机越过刷新线时
		     Enemy[(i+1)%EnemyNumber].isalive = true;  //将下一敌机设为可视，若i为EnemyNumber - 1，则设为第一个敌机可视
		if(Enemy[i].y > MapHigh)   //若敌机过界
		{
			PlayerScore++;
			gotoxy(Enemy[i].x,Enemy[i].y);
			printf(" ");  
			InitEnemyPlane(&Enemy[i]);//重置该机位置
			Enemy[i].isalive = true;
			gotoxy(Enemy[i].x,Enemy[i].y);
			printf("X");  
			ChangeObject(i,(i+1)%EnemyNumber);
			//因为是按数组下标顺序进行循环输出，所以触底后
			//将触底的飞机与其数组上后一个飞机结构交换
			//实现触底后立即在上方重新输出
		}
	
	} 
}
void JudgeCrash()
{
	for(int i = 0;i<EnemyNumber;i++)
	{
		if(Player.x == Enemy[i].x)
		      if(Player.y == Enemy[i].y)
		      {
			      Player.isalive = false;
			      break;
		      }
	}
}
void Init()
{
	system("cls");
	system("color 0a");
	system("mode con cols=27 lines=27");	//设置好运行时的控制台
	for(int i = 0;i<=EnemyNumber;i++)
	      InitEnemyPlane(&Enemy[i]);	//循环初始化每个敌机
	Enemy[0].isalive = true;		//设置第一个敌机
	gotoxy(Enemy[0].x,Enemy[0].y);
	printf("X"); 
	Player.isalive = true;
	srand(time(NULL));

	Player.x = (MapLong + 1)/2;		//初始化玩家飞机位置
	Player.y = MapHigh;
	PrintPlayerPlane(0,0);			//画玩家飞机
}
void OutPutMap()
{
	gotoxy(0,MapHigh+1);
	printf("Score:%d",PlayerScore);
}
int main()
{
	int count = SpeedUpCount;
	printf("Press any key to start");
	getch();
	Init();
	while(Player.isalive)
	{
		OutPutMap();
		Operate();
		JudgeCrash();
		Gaming();
		JudgeCrash();
		if(!(count--))//每当计数次到，则加速
		{
			count = SpeedUpCount;
			if(DelayTime>=0)
				DelayTime = DelayTime-5;
		}
		
	}
	gotoxy((MapLong/2)-3,MapLong/2);
	printf("GameOver");
	gotoxy((MapLong/2)-5,(MapLong/2)+5);
	printf("Your Score:%d",PlayerScore);
	getch();
	return 0;

}

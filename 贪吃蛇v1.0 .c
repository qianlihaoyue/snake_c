#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>


//全局变量
int len=3,apple[2],snake[100][2],path=0,wall[23][23],i,j;

//函数声明
void color(int a);         //颜色
void gotoxy(int x,int y);  //位置函数
void wall_info();          //围墙信息
void snake_apple();        //初始化显示蛇 苹果
void show_snake();         //显示蛇身
void show_score();         //显示分数
void if_wall();            //撞到墙
void eat_apple();          //吃到苹果
void eat_own();            //吃到自己
void show_apple();         //显示苹果
void movesnake();          //移动
void game_over();


void main()
{
    srand(time(NULL));                      //随机数种子
    system("title 贪吃蛇 BY LD");           //标题
    system("mode con lines=23 cols=46");    //界面大小
    wall_info();
    snake_apple();
    while(1)
    {
        show_score();
        movesnake();
        eat_apple();
        if_wall();
        eat_own();
    }
}

void color(int a)                 //颜色函数
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

void gotoxy(int x,int y)          //位置函数,光标移动到该点
{
	 COORD pos;
	 pos.X=2*x;
	 pos.Y=y;
	 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void wall_info()
{
    color(9);                      //blue
    for(i=1;i<=21;i++)             //打印墙内
    {
        for(j=1;j<=21;j++)
        {
            wall[i][j]=1;
            gotoxy(i,j);
            printf("■");
        }
    }
    color(7);                       //默认颜色
    for(i=0;i<23;i++)               //打印墙体
    {
        gotoxy(0,i);  printf("□");  //四道墙
        gotoxy(22,i); printf("□");
        gotoxy(i,0);  printf("□");
        gotoxy(i,22); printf("□");
    }
}

void snake_apple()                 //初始化蛇 苹果
{
    //随机数生成苹果
    apple[0]=rand()%21+1;
    apple[1]=rand()%21+1;
    show_apple();

    //
    //snake[0][0]=rand()%21+1;
    //snake[0][1]=rand()%21+1;
    /*
    for(i=0;i<10;i++)
    {
        snake[i][0]==10;
        snake[i][1]==10+i;
    }
    */
    snake[0][0]=10;                //蛇头
    snake[0][1]=10;

    snake[1][0]=10;                //蛇
    snake[1][1]=11;

    snake[2][0]=9;                 //蛇尾
    snake[2][1]=11;
    gotoxy(snake[0][0],snake[0][1]);
    color(14);
    printf("¤");

    show_snake();
}

void show_snake()
{
    color(14);
    for(i=1;i<len;i++)
    {
        gotoxy(snake[i][0],snake[i][1]);
        printf("★");
    }
}

void show_score()
{
    gotoxy(9,0);
    color(10);
    printf("score  %d",len-3);
}

void if_wall()
{
    if(snake[0][0]==0||snake[0][1]==0||snake[0][0]==22||snake[0][1]==22)
        game_over();
}



void eat_apple()
{
    if(snake[0][0]==apple[0]&&snake[0][1]==apple[1])
    {
        len++;        //如果蛇头与苹果坐标相同，长度++

        //再次生成苹果
        apple[0]=rand()%21+1;
        apple[1]=rand()%21+1;
        for(i=0;i<100;i++)              //遍历snake数组
        {                               //防止苹果出现在蛇身上
            if(apple[0]==snake[i][0]&&apple[1]==snake[i][1])
            {
                apple[0]=rand()%21+1;   //如果存在相同，重新生成
                apple[1]=rand()%21+1;
            }
        }
        show_apple();                   //显示苹果
    }
}

void show_apple()                 //显示苹果
{
    gotoxy(apple[0],apple[1]);
    color(12);                    //红色
    printf("●");
}

void eat_own()
{
    for(i=1;i<100;i++)            //遍历蛇身数组
    {                             //如果和蛇头相同 game over
        if(snake[i][0]==snake[0][0]&&snake[i][1]==snake[0][1])
            game_over();
    }
}


void movesnake()
{
    int temp[100][2];
    char ch;
    if(_kbhit())                 //如果有按键按下
    {
        ch=_getch();             //用ch储存键值
        switch (ch)              //if(path!=3)防止蛇往回走
        {                        //wasd  WASD 方向键都可控制，不建议方向键
            case 'w':case 'W':case 72:  if(path!=3) path=1;  break;
            case 'a':case 'A':case 75:  if(path!=4) path=2;  break;
            case 's':case 'S':case 80:  if(path!=1) path=3;  break;
            case 'd':case 'D':case 77:  if(path!=2) path=4;  break;
            default:break;
        }
    }

    if(path)
    {

        for(i=0;i<len;i++)
        {
            for(j=0;j<2;j++)                 //将snake[][]备份到temp[][]中
                temp[i][j]=snake[i][j];
        }
        for(i=0;i<len-1;i++)                            //蛇移动
        {
            for(j=0;j<2;j++)
                snake[i+1][j]=temp[i][j];
        }
        if(temp[len-1][0]!=0)
        {
            gotoxy(temp[len-1][0],temp[len-1][1]);      //消除蛇尾
            color(9);
            printf("■");
        }
        show_snake();                       //显示移动后的蛇身
    }

    if(path==1) snake[0][1]--;              //w
    if(path==2) snake[0][0]--;              //a
    if(path==3) snake[0][1]++;              //s
    if(path==4) snake[0][0]++;              //d

    color(14);
    gotoxy(snake[0][0],snake[0][1]);      //显示新蛇头
        printf("¤");

    switch(len/10)                        //随着长度增加，难度增加
    {
        case 0: Sleep(200);break;         //通过延迟调节难度
        case 1: Sleep(175);break;
        case 2: Sleep(150);break;
        case 3: Sleep(125);break;
        case 4: Sleep(100);break;
        default:Sleep(75);
    }

}


void game_over()
{
    gotoxy(9,10);                         //移动到中间
    color(12);
    printf("Game over!");
    while(1);                             //死循环
}

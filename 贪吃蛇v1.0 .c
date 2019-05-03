#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>


//ȫ�ֱ���
int len=3,apple[2],snake[100][2],path=0,wall[23][23],i,j;

//��������
void color(int a);         //��ɫ
void gotoxy(int x,int y);  //λ�ú���
void wall_info();          //Χǽ��Ϣ
void snake_apple();        //��ʼ����ʾ�� ƻ��
void show_snake();         //��ʾ����
void show_score();         //��ʾ����
void if_wall();            //ײ��ǽ
void eat_apple();          //�Ե�ƻ��
void eat_own();            //�Ե��Լ�
void show_apple();         //��ʾƻ��
void movesnake();          //�ƶ�
void game_over();


void main()
{
    srand(time(NULL));                      //���������
    system("title ̰���� BY LD");           //����
    system("mode con lines=23 cols=46");    //�����С
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

void color(int a)                 //��ɫ����
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

void gotoxy(int x,int y)          //λ�ú���,����ƶ����õ�
{
	 COORD pos;
	 pos.X=2*x;
	 pos.Y=y;
	 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void wall_info()
{
    color(9);                      //blue
    for(i=1;i<=21;i++)             //��ӡǽ��
    {
        for(j=1;j<=21;j++)
        {
            wall[i][j]=1;
            gotoxy(i,j);
            printf("��");
        }
    }
    color(7);                       //Ĭ����ɫ
    for(i=0;i<23;i++)               //��ӡǽ��
    {
        gotoxy(0,i);  printf("��");  //�ĵ�ǽ
        gotoxy(22,i); printf("��");
        gotoxy(i,0);  printf("��");
        gotoxy(i,22); printf("��");
    }
}

void snake_apple()                 //��ʼ���� ƻ��
{
    //���������ƻ��
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
    snake[0][0]=10;                //��ͷ
    snake[0][1]=10;

    snake[1][0]=10;                //��
    snake[1][1]=11;

    snake[2][0]=9;                 //��β
    snake[2][1]=11;
    gotoxy(snake[0][0],snake[0][1]);
    color(14);
    printf("��");

    show_snake();
}

void show_snake()
{
    color(14);
    for(i=1;i<len;i++)
    {
        gotoxy(snake[i][0],snake[i][1]);
        printf("��");
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
        len++;        //�����ͷ��ƻ��������ͬ������++

        //�ٴ�����ƻ��
        apple[0]=rand()%21+1;
        apple[1]=rand()%21+1;
        for(i=0;i<100;i++)              //����snake����
        {                               //��ֹƻ��������������
            if(apple[0]==snake[i][0]&&apple[1]==snake[i][1])
            {
                apple[0]=rand()%21+1;   //���������ͬ����������
                apple[1]=rand()%21+1;
            }
        }
        show_apple();                   //��ʾƻ��
    }
}

void show_apple()                 //��ʾƻ��
{
    gotoxy(apple[0],apple[1]);
    color(12);                    //��ɫ
    printf("��");
}

void eat_own()
{
    for(i=1;i<100;i++)            //������������
    {                             //�������ͷ��ͬ game over
        if(snake[i][0]==snake[0][0]&&snake[i][1]==snake[0][1])
            game_over();
    }
}


void movesnake()
{
    int temp[100][2];
    char ch;
    if(_kbhit())                 //����а�������
    {
        ch=_getch();             //��ch�����ֵ
        switch (ch)              //if(path!=3)��ֹ��������
        {                        //wasd  WASD ��������ɿ��ƣ������鷽���
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
            for(j=0;j<2;j++)                 //��snake[][]���ݵ�temp[][]��
                temp[i][j]=snake[i][j];
        }
        for(i=0;i<len-1;i++)                            //���ƶ�
        {
            for(j=0;j<2;j++)
                snake[i+1][j]=temp[i][j];
        }
        if(temp[len-1][0]!=0)
        {
            gotoxy(temp[len-1][0],temp[len-1][1]);      //������β
            color(9);
            printf("��");
        }
        show_snake();                       //��ʾ�ƶ��������
    }

    if(path==1) snake[0][1]--;              //w
    if(path==2) snake[0][0]--;              //a
    if(path==3) snake[0][1]++;              //s
    if(path==4) snake[0][0]++;              //d

    color(14);
    gotoxy(snake[0][0],snake[0][1]);      //��ʾ����ͷ
        printf("��");

    switch(len/10)                        //���ų������ӣ��Ѷ�����
    {
        case 0: Sleep(200);break;         //ͨ���ӳٵ����Ѷ�
        case 1: Sleep(175);break;
        case 2: Sleep(150);break;
        case 3: Sleep(125);break;
        case 4: Sleep(100);break;
        default:Sleep(75);
    }

}


void game_over()
{
    gotoxy(9,10);                         //�ƶ����м�
    color(12);
    printf("Game over!");
    while(1);                             //��ѭ��
}

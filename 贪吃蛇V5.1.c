#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
#include<math.h>

#define MAP 40   // 20~40       //标准大小40   

//全局变量
int ai_white[5],ai_black[5],white;
int apple[2],wall[MAP][MAP],i,j,page=0,end=0,mode=0,pause=0;
int len=3,snake[600][2]={MAP/2-6,MAP/2},path=0;
int len_1=3,snake_1[600][2]={MAP/2+6,MAP/2},path_1=0;

int theme_head=207,theme_back=219,theme_wall=219,theme_snake=158;
int color_back=1,color_wall=7,color_snake=14,color_snake_1=10;
int wall[MAP][MAP]={0};       ///1 有墙


//游戏函数
void wall_init();          //围墙信息         
void snake_apple();        //初始化显示蛇 苹果
void show_snake();         //显示蛇身
void show_score();         //显示分数
void if_wall();
void eat_apple();          //吃到苹果
void eat_own();            //吃到自己
void creat_apple();         //显示苹果
void movesnake();          //移动
void game_over();
void clear();
void knock();              //撞到墙k
void make_map();

//系统函数
void system_init();
void Cursor(int mode);
void color(int a);         
void gotoxy(int x,int y); 
void half_screen();
void homepage(); 

//人工智障
void AI();
void AI_apple();      
void AI_wall();
void AI_return();
void AI_enemy();
void AI_attack();        //普通攻击  超车杀
void AI_behead();        //对头杀
void AI_allure();        //诱杀
void AI_dagger();        //断头杀

void homepage()
{
    char ch;
    int k; 
  pagebegin:  
    k=0;
    SetConsoleOutputCP(936);
    system("cls");
    color(13);   

    gotoxy(MAP/2-2,MAP/2-3);   //大标题            
    printf("<Home page>");

    color(10); 
    gotoxy(MAP/2-2,MAP/2);        //第1行
    printf("<经典模式>");
    gotoxy(MAP/2-2,MAP/2+2);        //第2行   
    printf("<双人模式>");  
    gotoxy(MAP/2-2,MAP/2+4);      //第3行
    printf("<人机对战>");
    gotoxy(MAP/2-2,MAP/2+6);
    printf("<制作地图>");       

    color(13);      
    gotoxy(MAP/2-4,MAP/2);    
    printf("->");  
    while(1)
    {
        if(_kbhit())                
        {
            gotoxy(MAP/2-4,MAP/2+k);          //消除之前箭头
            printf("  ");
            ch=_getch();  
            switch(ch)             
            {                      
                case 'w':case 'W':case 72:k-=2; break;
                case 's':case 'S':case 80:k+=2; break;
            }
                
            if(k<0) k+=8; k%=8; 
                       
            if((int)ch==13) break;
            if((int)ch==27) {k=404; break;}
            gotoxy(MAP/2-4,MAP/2+k);                              //显示箭头
            printf("->"); 
        }
    }
   
    if(k==0) mode=0;                         //single
    if(k==2) mode=1;                         //double
    if(k==4) mode=2;                         //p v e
    if(k==6) mode=3;                         //diy map
    if(k==8) {page=1; goto pagebegin; }      //setting
    SetConsoleOutputCP(850);  
}

void main()
{
    system_init();
 begin:
    Cursor(0);
    homepage();       
    wall_init();
    if(mode==3) 
    {             
        make_map();
        goto begin;
    }   

    if(mode<3)
    {
        snake_apple();
        while(1)
        { 
            clear();
            show_score();
            movesnake();
            knock();  
            eat_apple();    
            eat_own();
            if_wall();
        }
    }
}


void AI()               ///黑白名单 优先级   <black=1>   <black=0 white=0>   <black=0 white=1>                                                                    
{
    int real;
    ai_white[1]=ai_white[2]=ai_white[3]=ai_white[4]=0;   //白名单初始化  
    ai_black[1]=ai_black[2]=ai_black[3]=ai_black[4]=0;   //黑名单初始化 
    real=white=0;                                        //最优解初始化   白名单优先级初始化
    i=snake_1[0][0];
    j=snake_1[0][1];

    AI_wall();      //黑名单
    AI_return();    //黑名单
    AI_enemy();     //黑名单

    AI_attack();  
    AI_behead(); 
    AI_allure();  
    AI_dagger();
//    AI_dead();   

    if(white==0)
    {
            AI_apple();     //白名单
    }
        

    for(i=1;i<5;i++)
    {
         if(ai_black[i]==0&&ai_white[i]) 
        {
            path_1=i; 
            real=1;                         //最优解
        }      
     }
    if(real==0)
    {
         for(i=1;i<5;i++)
        {
          if(ai_black[i]==0)
             path_1=i;
        }
    }
}


void AI_apple()
{
    if(apple[0]>i)  ai_white[4]=1;
    if(apple[0]<i)  ai_white[2]=1;
    if(apple[1]>j)  ai_white[3]=1;
    if(apple[1]<j)  ai_white[1]=1;
}

void AI_wall()
{
    if(wall[j-1][i]==1) ai_black[1]=1;
    if(wall[j+1][i]==1) ai_black[3]=1;
    if(wall[j][i-1]==1) ai_black[2]=1;                
    if(wall[j][i+1]==1) ai_black[4]=1;               
}

void AI_return()
{
    if(path_1==1) ai_black[3]=1;
    if(path_1==2) ai_black[4]=1;
    if(path_1==3) ai_black[1]=1;
    if(path_1==4) ai_black[2]=1;
}

void AI_enemy()
{
    int a;
    for(a=0;a<len;a++)
    {
         if(snake[a][1]==j)      //y 轴相同
        {
             if(snake[a][0]==i+1)   ai_black[4]=1;
             if(snake[a][0]==i-1)   ai_black[2]=1;
        }

         if(snake[a][0]==i)      //x 轴相同
         {
              if(snake[a][1]==j+1)   ai_black[3]=1;
              if(snake[a][1]==j-1)   ai_black[1]=1;
         }
    }
}

void AI_attack()                  //邻近攻击
{
    if(path==path_1)
    {
        switch(path)
        {
            case 1:          //同时向上    //p2 在 上
            if(snake[0][1]-j==2||snake[0][1]-j==3)       
            {
                if(i-snake[0][0]==1||i-snake[0][0]==2) {ai_white[2]=1;white=1;}   
                if(snake[0][0]-i==1||snake[0][0]-i==2) {ai_white[4]=1;white=1;}  
            }break;

            case 3:          //同时向下   //p2 在 下
            if(j-snake[0][1]==2||j-snake[0][1]==3)
            {
                if(i-snake[0][0]==1||i-snake[0][0]==2) {ai_white[2]=1;white=1;}  
                if(snake[0][0]-i==1||snake[0][0]-i==2) {ai_white[4]=1;white=1;}  
            }break;

            case 2:          //同时向左  //p2 在 左
            if(snake[0][0]-i==2||snake[0][0]-i==3)
            {
                if(j-snake[0][1]==1||j-snake[0][1]==2) {ai_white[1]=1;white=1;}     //p2 在 下
                if(snake[0][1]-i==1||snake[0][1]-i==2) {ai_white[3]=1;white=1;}     //p2 在 上
            }break;

            case 4:          //同时向右  //p2 在 右
            if(i-snake[0][0]==2||i-snake[0][0]==3)
            {
                if(j-snake[0][1]==1||j-snake[0][1]==2) {ai_white[1]=1;white=1;}  
                if(snake[0][1]-j==1||snake[0][1]-j==2) {ai_white[3]=1;white=1;}  
            }break;                                    
        }
    }
}

void AI_behead()   //对头杀
{
    if(abs(path-path_1)==2&&abs(snake[0][0]-i)<2&&abs(snake[0][1]-j)<2)       //对头 且临近
    {
        if(path%2)               //奇数  13  31
        {
            if(snake[0][0]>i)  {ai_white[4]=1;white=2;} 
            if(snake[0][0]<i)  {ai_white[2]=1;white=2;} 
        }
        else                     //偶数  24  42
        {            
            if(snake[0][1]>j)  {ai_white[3]=1;white=2;} 
            if(snake[0][1]<j)  {ai_white[1]=1;white=2;} 
        }

    }
}

void AI_allure()                //诱杀
{
    if(path_1%2&&apple[0]==i)       
    {
        if(apple[1]+1==j||apple[1]+1==j)   
        {
            if(path==4&&apple[0]-snake[0][0]<=6&&apple[0]-snake[0][0]>=4)
            {
                ai_white[2]=1;white=3;
            }
            if(path==2&&snake[0][0]-apple[0]<=6&&snake[0][0]-apple[0]>=4)
            {
                ai_white[4]=1;white=3;
            } 
        }
    }
    if(path_1%2==0&&apple[1]==j)       
    {
        if(apple[0]+1==i||apple[0]+1==i)   
        {
            if(path==3&&apple[1]-snake[0][1]<=6&&apple[1]-snake[0][1]>=4)
            {
                ai_white[1]=1;white=3;
            }
            if(path==1&&snake[0][1]-apple[1]<=6&&snake[0][1]-apple[1]>=4)
            {
                ai_white[3]=1;white=3;
            } 
        }
    }

}

void AI_dagger()        //断头杀
{
    if(path%2==0&&abs(i-snake[0][0])<4)
    {
        if(path_1==1&&snake[0][1]-j>2&&snake[0][1]-j<6)
        {
            ai_white[6-path]=1;
            white=1;            
        }
        if(path_1==3&&j-snake[0][1]>2&&j-snake[0][1]<6)
        {
            ai_white[6-path]=1;
            white=1;            
        }  
    }
    
    if(path%2&&abs(j-snake[0][1])<4)
    {
        if(path_1==4&&j-snake[0][0]>2&&j-snake[0][0]<6)
        {
            ai_white[4-path]=1;
            white=1;            
        }
        
        if(path_1==2&&snake[0][0]-j>2&&snake[0][0]-j<6)
        {
            ai_white[4-path]=1;
            white=1;            
        }  
    }
}

void system_init()
{    
    if(MAP<30) system("mode con lines=30 cols=60");    //界面大小
    else       system("mode con lines=50 cols=100");    //界面大小
    system("title 贪吃蛇 BY Haoyue");  
    srand(time(NULL));  
}

void movesnake()
{
    int temp[600][2];
    char ch;
    Cursor(0);
    
    if(_kbhit())                 //如果有按键按下
    {
        ch=_getch();             
        switch (ch)              //P1
        {                        //if(path!=3)防止蛇往回走
            case 'w':case 'W':  if(path!=3) path=1;  break;
            case 'a':case 'A':  if(path!=4) path=2;  break;
            case 's':case 'S':  if(path!=1) path=3;  break;
            case 'd':case 'D':  if(path!=2) path=4;  break;
            default:break;
        }
        if(mode==1)              //P2
        {
            switch (ch)             
            {                      
                case 'i':case 'I':  if(path_1!=3) path_1=1;  break;
                case 'j':case 'J':  if(path_1!=4) path_1=2;  break;
                case 'k':case 'K':  if(path_1!=1) path_1=3;  break;
                case 'l':case 'L':  if(path_1!=2) path_1=4;  break;            
                default:break;
            }

        }

        if((int)ch==27) pause=1;
        while(pause)
        {
            if((int)_getch()==27) pause=0;
        } 
    }
    
    if(mode==2) AI();

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

            gotoxy(temp[len-1][0],temp[len-1][1]);      //消除蛇尾
            color(color_back);
            printf("%c",theme_back);
        show_snake();                       //显示移动后的蛇身
    }

    if(path==1) snake[0][1]--;              //w
    if(path==2) snake[0][0]--;              //a
    if(path==3) snake[0][1]++;              //s
    if(path==4) snake[0][0]++;              //d

    if(mode)
    {
        if(path_1)
        {
            for(i=0;i<len_1;i++)
            {
                for(j=0;j<2;j++)                 
                    temp[i][j]=snake_1[i][j];
            }
            for(i=0;i<len_1-1;i++)                          
            {
                for(j=0;j<2;j++)
                    snake_1[i+1][j]=temp[i][j];
            }

            gotoxy(temp[len_1-1][0],temp[len_1-1][1]);      //消除蛇尾
            color(color_back);
            printf("%c",theme_back);
            show_snake();                       //显示移动后的蛇身
        }

        if(path_1==1) snake_1[0][1]--;              //w
        if(path_1==2) snake_1[0][0]--;              //a
        if(path_1==3) snake_1[0][1]++;              //s
        if(path_1==4) snake_1[0][0]++;              //d

        color(color_snake_1);
        gotoxy(snake_1[0][0],snake_1[0][1]);      //显示新蛇头
            printf("%c",theme_head); 
    }
    color(color_snake);
    gotoxy(snake[0][0],snake[0][1]);      //显示新蛇头
    printf("%c",theme_head);

    if(mode) 
        Sleep(125);                           //双人游戏 &  人机对战    速度恒定
    else
    {
        if((len-3)<40)                        //单人模式  随着长度增加，难度增加
            Sleep(200-(len-3)*3);
        else if((len-3)<80)
            Sleep(120-(len-3));
        else
         Sleep(40);
    }
} 

void wall_init()
{
    color(color_back);                      //blue
    for(i=1;i<=MAP-2;i++)             //打印墙内
    {
        for(j=1;j<=MAP-2;j++)
        {
            gotoxy(i,j);
            printf("%c",theme_back);          
        }
    }
    color(color_wall);                       //默认颜色
    for(i=1;i<=MAP-2;i++)             //打印墙
    {
        for(j=1;j<=MAP-2;j++)
        {
            if(wall[i][j])
            {
               gotoxy(j,i);  printf("%c",theme_wall);
            }
        }
    }

}

void knock()
{
    for(i=0;i<MAP;i++)
    {
        for(j=0;j<MAP;j++)
        {
            if(wall[i][j]==1&&snake[0][0]==j&&snake[0][1]==i)
                end=1;
            if(wall[i][j]==1&&snake_1[0][0]==j&&snake_1[0][1]==i)
            {
                if(mode==1) end=2;
                else end=3;
            }
            if(end)
                game_over();

        }
    }
}
void snake_apple()                 //初始化蛇 苹果
{
    color(10);
    gotoxy(MAP,3);
    printf("Ese pause");
    gotoxy(MAP,5);
    printf("P1 WASD");
    if(mode)
    {
        gotoxy(MAP,7);
        printf("P2 IJKL");
    }

    creat_apple();
   
    for(i=0;i<MAP;i++)
    {
        for(j=0;j<MAP;j++)
        {

            if(wall[i][j]&&snake[0][0]==j&&snake[0][1]==i)                       //P1在墙上
            {
                snake[0][0]=rand()%(MAP-2)+1;                                    //重新定位
                snake[0][1]=rand()%(MAP-2)+1;
            }
            if(mode)
            {            
                if(snake[0][0]==snake_1[0][0]&&snake[0][1]==snake_1[0][1])          //初始位置相同
                {
                    snake[0][0]=rand()%(MAP-2)+1;   
                    snake[0][1]=rand()%(MAP-2)+1;
                }
                if(wall[i][j]&&snake_1[0][0]==j&&snake_1[0][1]==i)                  //P2在墙上
                {
                    snake_1[0][0]=rand()%(MAP-2)+1;   
                    snake_1[0][1]=rand()%(MAP-2)+1;
                }               
            }
        }
    }

    gotoxy(snake[0][0],snake[0][1]);
    color(14); 
    printf("%c",theme_head);
    if(mode)
    {  
        gotoxy(snake_1[0][0],snake_1[0][1]);
        printf("%c",theme_head);
    }
}

void show_snake()
{
    for(i=1;i<len;i++)
    {
        color(color_snake);
        gotoxy(snake[i][0],snake[i][1]); 
        printf("%c",theme_snake);
    }
    if(mode)
    {
        color(color_snake_1);
        for(i=1;i<len_1;i++)
        {
            gotoxy(snake_1[i][0],snake_1[i][1]);
            printf("%c",theme_snake);
        }
    }
    SetConsoleOutputCP(936);
    gotoxy(apple[0],apple[1]);
    color(12);                    //红色
    printf("●");
    SetConsoleOutputCP(850);
}

void show_score()
{
    color(color_snake);
    gotoxy(MAP,1);
    if(mode) 
        printf("P1 score  %d",(len-3)/2);
    else
        printf("P1 score  %d",len-3);

    if(mode)
    {
        color(color_snake_1);
        gotoxy(MAP,3);  
        printf("P2 score  %d",(len_1-3)/2);  
    }
}

void if_wall()
{
    for(i=0;i<2;i++)
    {
        if(snake[0][i]==MAP-1)
            snake[0][i]=1;
        if(snake[0][i]==0)
            snake[0][i]=MAP-1;
    }
    if(mode)
    {
        for(i=0;i<2;i++)
        {
        if(snake_1[0][i]==MAP-1)
            snake_1[0][i]=1;
        if(snake_1[0][i]==0)
            snake_1[0][i]=MAP-1;
        }   
    }

}

void eat_apple()
{
    if(snake[0][0]==apple[0]&&snake[0][1]==apple[1])
    {
        if(mode) len+=2;
        else len++;        //如果蛇头与苹果坐标相同，长度++
        creat_apple();
    }
    if(mode)
    {
        if(snake_1[0][0]==apple[0]&&snake_1[0][1]==apple[1])
        {
            len_1+=2;        //如果蛇头与苹果坐标相同，长度++
            creat_apple();
        }   
    }
}

void creat_apple()                 //显示苹果
{
    SetConsoleOutputCP(936);
    apple[0]=rand()%(MAP-2)+1;
    apple[1]=rand()%(MAP-2)+1;
    for(i=0;i<100;i++)              //遍历snake数组
    {                               //防止苹果出现在蛇身上
        if((apple[0]==snake[i][0]&&apple[1]==snake[i][1])||(apple[0]==snake_1[i][0]&&apple[1]==snake_1[i][1]))
        {
            apple[0]=rand()%(MAP-2)+1;   //如果存在相同，重新生成
            apple[1]=rand()%(MAP-2)+1;
        }
    }
    for(i=0;i<MAP;i++)
    {
        for(j=0;j<MAP;j++)
        {
            if(wall[i][j]&&apple[0]==j&&apple[1]==i)
            {
                apple[0]=rand()%(MAP-2)+1;   //如果存在相同，重新生成
                apple[1]=rand()%(MAP-2)+1;
            }
        }
    }
    gotoxy(apple[0],apple[1]);
    color(12);                    //红色
    printf("●");
    SetConsoleOutputCP(850);
}

void eat_own()
{
    for(i=1;i<600;i++)            //遍历蛇身数组
    {                             //如果和蛇头相同 game over
        
        if(mode)
        {
            if(snake[0][0]==snake_1[0][0]&&snake[0][1]==snake_1[0][1]) end=4;
            if(snake_1[i][0]==snake[0][0]&&snake_1[i][1]==snake[0][1]) end=1;      //P1 eat p2
            if(snake[i][0]==snake_1[0][0]&&snake[i][1]==snake_1[0][1])
            {
                if(mode==1) end=2;      //p2 eat p1
                else end=3;
            } 
        }
        else
            if(snake[i][0]==snake[0][0]&&snake[i][1]==snake[0][1]) end=1;            //P1 eat P1
        if(end)
         game_over();
    }
}

void game_over()
{
    gotoxy(MAP/2-6,MAP/2);                         //移动到中间
    color(12);
    switch(end)                                    
    {
        case 1:printf("P1  Game over!"); break;
        case 2:printf("P2  Game over!"); break;
        case 3: color(14); printf("You win!");       break;
        case 4:printf("All dead!!!");
    }
    Sleep(1000);       
    while(1)
    {
        gotoxy(MAP,MAP); 
        if(_kbhit())   
            exit(0);                 //如果有按键按下                       
    }
}

void make_map()
{
    int x=1,y=1,w=0;
    char ch;
    Cursor(1);                            //显示光标
    SetConsoleOutputCP(936);              //简体中文
    gotoxy(MAP,3);
    printf("空格切换  enter确认");
    SetConsoleOutputCP(850);  

    while(1)
    {
        gotoxy(x,y);                 //显示光标

        if(_kbhit())                 //如果有按键按下
        {
            ch=_getch();             //用ch储存键值 
            switch (ch)              //if(path!=3)防止蛇往回走
            {                        //wasd  WASD 方向键都可控制，不建议方向键
                case 'w':case 'W':case 72: y--;   break;
                case 'a':case 'A':case 75: x--;   break;
                case 's':case 'S':case 80: y++;   break;
                case 'd':case 'D':case 77: x++;   break;
                case 32 :w++; break;
            }
            w%=2;
            if((int)ch==13)  break;

            if(x==0) x=MAP-2;              //从另一边出来
            if(x==MAP-1) x=1;
            if(y==0) y=MAP-2;
            if(y==MAP-1) y=1;

            gotoxy(x,y);
            if(w)
            {
                color(color_wall);
                printf("%c",theme_wall);
                wall[y][x]=1;
            }
            else
            {
                color(color_back);
                printf("%c",theme_back);
                wall[y][x]=0;
            }         
        }      
    }
}

void clear()
{
    for(i=0;i<MAP;i++)               
    {
        gotoxy(0,i);  printf(" "); 
        gotoxy(MAP-1,i); printf(" ");
        gotoxy(i,0);  printf(" ");
        gotoxy(i,MAP-1); printf(" ");
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

void Cursor(int mode)                       //mode=0 消除光标 
{ 
    CONSOLE_CURSOR_INFO cursor_info = {1, mode};  
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); 
} 

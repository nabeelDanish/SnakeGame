#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <cmath>     
#include <fstream>
using namespace std;

//***************************************LETS STARTS BY DECLARING ALL THE VARIABLES


//File Handling variable
fstream file;
//Drawing Board
int board_x[80];
int board_y[80];
int boundry[4][80][2];
//Snake Body and Head
int snake_size=5;
int head_x;
int head_y;
int *snake_body_x;
int *snake_body_y;
int temp_body_x;
int temp_body_y;
int direction=1;
//Fruits
int fruit_x[5];
int fruit_y[5];
int fruit_time[5]={0,0,0,0,0};
//Bonus Food
int bonus_food_coor[2];
int bonus_food_time=0;
//Hurdles
int hurdle_x[3];
int hurdle_y[3];
int hurdle_type[3];
int hurdle_time=0;
//Scoring and Highscoring
int score=0;
char score_string[64];
int highscore;
char highscore_string[64];
int perv_score[5];
char prevscore_string[5][64];
//Switches, Menus, Miscs
int diffculty=3;
int color=5;
bool pause_game=0;
int menu=1;
int progress_bar;
bool space_press=0;
float game_speed;
bool game_resumed=0;
bool game_started=0;
int game_mode=0;
bool escape_press=0;
bool perv_game_status=1;
bool old_game;
bool has_game_played_once=0;
int menu_counter=0;

//******************************************PROTOTYPING-FUNCTIONS

//--------------------GLUT Functions provided by NUCES
void SetCanvasSize(int,int);
void Display();
void NonPrintableKeys(int, int, int);
void PrintableKeys(unsigned char, int, int);
void Timer(int);

//---------------------Functions for Board and Boundries
void intialize_board(int[],int[]);
void initiliaze_boundries();
void Drawboundry();

//---------------------Functions for Snake Movement and Positions
void movement(int&,int&,int&);
void snake_body_coords_init(int*,int*,int[],int[],int&);
void snake_body_update();
void snake_body_coords(int*,int*,int[],int[],int&,int&);
void snake_head_draw();
void snake_draw(int*,int*,int);

//--------------------Functions for Fruit placement and thier collisions
void fruit_place(int);
void fruit_scatter();
void fruit_time_update();
void draw_food();
void food_pick();

//--------------------Functions for Bonus Food Placement and thier collisions
void bonus_food_place();
void bonus_food();

//--------------------Functions for Hurdle placement and Drawing
void spawn_hurdle(int);
void scatter_hurdles();
void hurdle_timer();
void draw_hurdle();

//--------------------Function for FATAL Collisions
void collision();

//--------------------Functions for MAIN MENU functionality
void game();
void menu_screen();
void menu_diffculty();
void game_history();
void resume_game();
void help_screen();
void start_game();
void level_choose();
void game_end();

//--------------------Utility Function
void score_to_string(int,char[]);


//*********************************************THE MAIN FUNCTION

int main(int argc, char*argv[]) 
{
	int width = 800, height = 900; 	
	file.open("save-file.txt",ios::in);
	file>>old_game;
	perv_game_status=1;
	file>>highscore;
	for (int i=0;i<5;++i)
	{
		file>>perv_score[i];
	}
	file.close();
	intialize_board(board_x,board_y);
	snake_body_x=new int[snake_size];
	snake_body_y=new int[snake_size];
	snake_body_coords_init(snake_body_x,snake_body_y,board_x,board_y,snake_size);
	snake_body_coords(snake_body_x,snake_body_y,board_x,board_y,snake_size,direction);
	InitRandomizer();
	fruit_scatter();
	scatter_hurdles();
	
	//GLUT FUNCTIONS WHICH ARE NOT TO BE MEDDELED WITH
	
	glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50); 
	glutInitWindowSize(width, height);
	glutCreateWindow("SNAKE GAME BY NABEEL DANISH");
	SetCanvasSize(width, height);
	glutDisplayFunc(Display);
	glutSpecialFunc(NonPrintableKeys);
	glutKeyboardFunc(PrintableKeys); 
	glutTimerFunc(250 / FPS, Timer, 0);
	glutMainLoop();
	return 1;
}

//**************************************FUNCTION DEFINATIONS

void SetCanvasSize(int width, int height) 
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); 
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
}
void Display()
{	
	glClearColor(0,0.0,0.0,0);
	glClear(GL_COLOR_BUFFER_BIT);  
	switch(menu)
	{
		case 0:
		if(game_started)
		{
			game();
		}
		else
		{
		start_game();
		}
		break;
		case 1:
			menu_screen();
		break;
		case 2:
			menu_diffculty();
		break;
		case 3:
			game_history();
		break;
		case 4:
			resume_game();
		break;
		case 5:
			help_screen();
		break;
		case 6:
			game_end();
		break;
		case 7:
			level_choose();
		break;
	}
	glutSwapBuffers(); // do not modify this line..
}
void NonPrintableKeys(int key, int x, int y) 
{
	if (key == GLUT_KEY_LEFT) 
	{
		if (direction!=1)
		{		
			if (menu==0)
			{
				direction=0;
			}
		}											 
	} 
	else if (key == GLUT_KEY_RIGHT) 
	{
		if (direction!=0)
		{		
			direction=1;
		}		
	} 
	else if (key == GLUT_KEY_UP)
	{
		switch(menu)
		{
			case 0:
			if (direction!=3)
			{		
				direction=2;
			}
			break;
			case 1:
			case 2:
			case 7:
			if (menu_counter!=0)
			{
				--menu_counter;
			}
			break;	 
		}
		       
	}
	else if (key == GLUT_KEY_DOWN)
	{
		switch(menu)
		{
			case 0:
			if (direction!=2)
			{		
				direction=3;
			}
			break;
			case 1:
			if (menu_counter!=4)
			{
				++menu_counter;
			}
			break;
			case 2:
			if (menu_counter!=2)
			{
				++menu_counter;
			}
			break;
			case 7:
			if (menu_counter!=1)
			{
				++menu_counter;
			}
		}		
	}
glutPostRedisplay();
}
void PrintableKeys(unsigned char key, int x, int y) 
{
    if (key == KEY_ESC) 
    {
        escape_press=1;
        if (game_started)
        {
        	perv_game_status=1;
        	menu=1;
        }
        else
        {
        	game_end();
        }
    }
    if (key == 'R' || key=='r')
    {
    }
    else if (int(key) == 13)
    {  
    }
	else if(key == ' ')
	{
		if (space_press==1)
		{
			space_press=0;
		}
		else
		{
			space_press=1;
		}
	}
	if (key =='c')
	{
		if (color!=7)
		{
			++color;
		}
		else
		{
			color=0;
		}
	}
	if ((key == 'a')||(key == 'A')) 
	{
		if (direction!=1)
		{		
			if (menu==0)
			{
				direction=0;
			}
		}											 
	} 
	else if ((key == 'd')||(key == 'D')) 
	{
		if (direction!=0)
		{		
			direction=1;
		}		
	} 
	else if ((key == 'w')||(key == 'W')) 
	{
		switch(menu)
		{
			case 0:
			if (direction!=3)
			{		
				direction=2;
			}
			break;
			case 1:
			case 2:
			case 7:
			if (menu_counter!=0)
			{
				--menu_counter;
			}
			break;	 
		}
		       
	}
	else if ((key == 's')||(key == 'S')) 
	{
		switch(menu)
		{
			case 0:
			if (direction!=2)
			{		
				direction=3;
			}
			break;
			case 1:
			if (menu_counter!=4)
			{
				++menu_counter;
			}
			break;
			case 2:
			if (menu_counter!=2)
			{
				++menu_counter;
			}
			break;
			case 7:
			if (menu_counter!=1)
			{
				++menu_counter;
			}
		}		
	}
    glutPostRedisplay();
}
void Timer(int m) 
{
	game_speed=600.0/diffculty;
	glutPostRedisplay();
	glutTimerFunc(game_speed / FPS, Timer, 0);
}
void intialize_board(int b_x[],int b_y[])
{
	for (int i=0;i<80;++i)
	{
		b_x[i]=i*10;
		b_y[i]=i*10;
	}
}
void game_end()
{
	menu=6;
	game_started=0;
	file.open("save-file.txt",ios::out);
	if(has_game_played_once)
	{
		file<<perv_game_status<<endl;
	}
	else
	{
		file<<old_game<<endl;
	}
	if (score>highscore)
	{
		file<<score<<endl;
	}
	else
	{
		file<<highscore<<endl;
	}
	for (int i=4;i>=1;--i)
	{
		perv_score[i]=perv_score[i-1];
	}
	perv_score[0]=score;
	for (int i=0;i<5;++i)
	{
		file<<perv_score[i]<<endl;
	}
	file<<head_x<<endl;
	file<<head_y<<endl;
	file<<snake_size<<endl;
	for (int i=0;i<snake_size;++i)
	{
		file<<snake_body_x[i]<<"\n"<<snake_body_y[i]<<endl;
	}
	for (int i=0;i<5;++i)
	{
		file<<fruit_x[i]<<"\n"<<fruit_y[i]<<endl;
	}
	file<<diffculty<<"\n"<<color<<"\n"<<direction<<endl;
	for (int i=0;i<3;++i)
	{
		file<<hurdle_type[i]<<endl;
	}
	for (int i=0;i<3;++i)
	{
		file<<hurdle_x[i]<<"\n"<<hurdle_y[i]<<endl;
	}
	file<<game_mode<<endl;
	file.close();
	DrawString(280,850,"OUCH!!, SORRY YOU LOST",colors[5]);
	DrawString(220,800,"YOUR SCORE: ",colors[5]);
	DrawString(450,800,score_string,colors[5]);
	if (score>highscore)
	{
		DrawString(320,550,"WOW! YOU HAVE SET A NEW HIGHSCORE",colors[5]);
	}
	DrawString(200,450,"PRESS SPACE TO GO BACK TO MAIN MENU",colors[5]);
	DrawString(200,410,"PRESS ESC TO QUIT",colors[5]);
	if (escape_press)
	{
		exit(1);
	}
	if (space_press)
	{
		space_press=0;
		menu=1;
	}
}
void score_to_string(int x,char a[])
{
	if (x!=0)
	{
		int size=0;
		int temp=x;
		while(temp>0)
		{
			temp/=10;
			++size;
		}
		int k=0;
		for (int i=size-1;i>=0;--i,++k)
		{
			int temp2=pow(10,i);
			a[k]=48+(x/temp2);
			x=x-(x/temp2)*temp2;
		}
		a[k]='\0';
	}
	else
	{
		a[0]=48;
		a[1]='\0';
	}
}
void spawn_hurdle(int n)
{
	bool proper_place=1;
	do
	{
		proper_place=1;
		hurdle_type[n]=rand()%5;
		hurdle_x[n]=GetRandInRange(10,70)*10;
		hurdle_y[n]=GetRandInRange(10,70)*10;
		if ((hurdle_x[n]==board_x[head_x])&&(hurdle_y[n]==board_y[head_y]))
		{
			proper_place=0;
		}
		for (int i=0;i<snake_size;++i)
		{
			if((hurdle_x[n]==snake_body_x[i])&&(hurdle_y[n]==snake_body_y[i]))
			{
				proper_place=0;
			}
		}
		for (int i=0;i<5;++i)
		{
			if((hurdle_x[n]==fruit_x[i])&&(hurdle_y[n]==fruit_y[i]))
			{
				proper_place=0;
			}
		}
	}while(proper_place==0);
}
void scatter_hurdles()
{
	for (int i=0;i<diffculty;++i)
	{
		spawn_hurdle(i);
	}
	hurdle_time=0;
}
void hurdle_timer()
{
	++hurdle_time;
	if (hurdle_time>=300)
	{
		scatter_hurdles();
	}
}
void draw_hurdle()
{
	for (int i=0;i<diffculty;++i)
	{
		switch(hurdle_type[i])
		{
			case 0:
			DrawSquare(hurdle_x[i],hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+10,hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+20,hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+30,hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+40,hurdle_y[i],10,colors[25]);			
			break;
			case 1:
			DrawSquare(hurdle_x[i],hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i],hurdle_y[i]+10,10,colors[25]);
			DrawSquare(hurdle_x[i],hurdle_y[i]+20,10,colors[25]);
			DrawSquare(hurdle_x[i],hurdle_y[i]+30,10,colors[25]);
			DrawSquare(hurdle_x[i],hurdle_y[i]+40,10,colors[25]);	
			break;		
			case 2:
			DrawSquare(hurdle_x[i],hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+10,hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+20,hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+20,hurdle_y[i]+10,10,colors[25]);
			DrawSquare(hurdle_x[i]+20,hurdle_y[i]+20,10,colors[25]);
			break;		
			case 3:
			DrawSquare(hurdle_x[i],hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i],hurdle_y[i]-10,10,colors[25]);
			DrawSquare(hurdle_x[i],hurdle_y[i]-20,10,colors[25]);
			DrawSquare(hurdle_x[i]+10,hurdle_y[i]-20,10,colors[25]);
			DrawSquare(hurdle_x[i]+20,hurdle_y[i]-20,10,colors[25]);
			DrawSquare(hurdle_x[i]+30,hurdle_y[i]-20,10,colors[25]);	
			DrawSquare(hurdle_x[i]+30,hurdle_y[i]-10,10,colors[25]);
			DrawSquare(hurdle_x[i]+30,hurdle_y[i],10,colors[25]);	
			break;
			case 4:
			DrawSquare(hurdle_x[i],hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+10,hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+20,hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+30,hurdle_y[i],10,colors[25]);
			DrawSquare(hurdle_x[i]+30,hurdle_y[i]-10,10,colors[25]);
			DrawSquare(hurdle_x[i]+30,hurdle_y[i]-20,10,colors[25]);
			DrawSquare(hurdle_x[i]+30,hurdle_y[i]-30,10,colors[25]);
			DrawSquare(hurdle_x[i]+40,hurdle_y[i]-30,10,colors[25]);
			DrawSquare(hurdle_x[i]+50,hurdle_y[i]-30,10,colors[25]);
			DrawSquare(hurdle_x[i]+60,hurdle_y[i]-30,10,colors[25]);
			break;
		}
	}
}
void movement(int &x,int &y,int &direction)
{
	temp_body_x=board_x[head_x];
	temp_body_y=board_y[head_y];
	if (head_y==1)
	{
		if (game_mode!=1)
		{
			head_y=79;
		}
		else
		{
			perv_game_status=0;
			game_end();
		}
	}
	else if(head_y==79)
	{
		if (game_mode!=1)
		{
			head_y=1;
		}
		else
		{
			perv_game_status=0;
			game_end();
		}
	}
	if (head_x==1)
	{
		if (game_mode!=1)
		{
			head_x=79;
		}
		else
		{
			perv_game_status=0;
			game_end();
		}
	}
	else if(head_x==79)
	{
		if (game_mode!=1)
		{
			head_x=1;
		}
		else
		{
			perv_game_status=0;
			game_end();
		}
	}
	switch(direction)
	{
		case 0:
			--head_x;
			break;
		case 1:
			++head_x;
			break;
		case 2:
			++head_y;
			break;
		case 3:
			--head_y;
			break;
	}
}
void fruit_place(int n)
{
	bool proper_place=1;
	do
	{
		proper_place=1;
		int rand1=GetRandInRange(1,79)*10;
		int rand2=GetRandInRange(1,79)*10;
		fruit_x[n]=rand1;
		fruit_y[n]=rand2;
		if((fruit_x[n]==board_x[head_x])&&(fruit_y[n]==board_y[head_y]))
		{
			proper_place=0;
		}
		for (int i=0;i<5;++i)
		{
			if(i!=n)
			{
				if((fruit_x[n]==fruit_x[i])||(fruit_y[n]==fruit_y[i]))
				{
					proper_place=0;
				}
				if(abs(fruit_x[n]-fruit_x[i])==abs(fruit_y[n]-fruit_y[i]))
				{
					proper_place=0;
				}
				if((fruit_x[n]==fruit_x[i])&&(fruit_y[n]==fruit_y[i]))
				{
					proper_place=0;
				}
			}
		}
		for (int i=0;i<snake_size;++i)
		{
			if ((fruit_x[n]==snake_body_x[i])&&(fruit_y[n]==snake_body_y[i]))
			{
				proper_place=0;
			}
		}
		for (int i=0;i<diffculty;++i)
		{
			if((fruit_x[n]==hurdle_x[i])&&(fruit_y[n]==hurdle_y[i]))
			{
				proper_place=0;
			}
		}
	}while(proper_place==0);
	fruit_time[n]=0;
}
void fruit_scatter()
{
	for(int i=0;i<5;++i)
	{
		fruit_place(i);
	}
}
void fruit_time_update()
{
	for(int i=0;i<5;++i)
	{
		++fruit_time[i];
		if (fruit_time[i]>=300)
		{
			fruit_place(i);
		}
	}
}
void bonus_food_place()
{
	if(bonus_food_time==300)
	{
		bool proper_place=0;
		do
		{
			proper_place=0;
			bonus_food_coor[0]=GetRandInRange(10,70)*10;
			bonus_food_coor[1]=GetRandInRange(10,70)*10;
			if ((bonus_food_coor[0]==board_x[head_x])&&(bonus_food_coor[1]==board_y[head_y]))
			{
				proper_place=1;
			}
			for (int i=0;i<snake_size;++i)
			{
				if((bonus_food_coor[0]==snake_body_x[i])&&(bonus_food_coor[1]==snake_body_y[i]))
				{
					proper_place=1;
				}
			}
			for(int i=0;i<diffculty;++i)
			{
				if((bonus_food_coor[0]==hurdle_x[i])&&(bonus_food_coor[1]==hurdle_y[i]))
				{
					proper_place=1;
				}
			}
			for (int i=0;i<5;++i)
			{
				if((bonus_food_coor[0]==fruit_x[i])&&(bonus_food_coor[1]==fruit_y[i]))
				{
					proper_place=1;
				}
			}
		}while(proper_place==1);
	
	}
}
void bonus_food()
{
	++bonus_food_time;
	if (bonus_food_time>=1200)
	{
		DrawTriangle(bonus_food_coor[0],bonus_food_coor[1],bonus_food_coor[0]+20,
		bonus_food_coor[1],bonus_food_coor[0]+10,bonus_food_coor[1]+20,colors[0+5*((bonus_food_time-1200)/20)]);
		if (((board_x[head_x]==bonus_food_coor[0])||(board_x[head_x]==bonus_food_coor[0]+10))&&((board_y[head_y]==bonus_food_coor[1])||(board_y[head_y]==bonus_food_coor[1]+20)))
		{
			score+=100-5*((bonus_food_time-1200)/20);
			bonus_food_time=0;
			if(color!=7)
			{
				++color;
			}
			else
			{
				color=0;
			}
		}
	}
	if (bonus_food_time>=1500)
	{
		bonus_food_time=0;
	}
}
void draw_food()
{
	for (int i=0;i<5;++i)
	{
		DrawSquare(fruit_x[i],fruit_y[i],10,colors[36]);
	}
}
void snake_body_coords_init(int *a_x,int *a_y ,int b_x[],int b_y[],int &n)
{
	for (int i=0;i<n;++i)
	{
		a_x[i]=b_x[head_x-i-1];
		a_y[i]=b_y[head_y];
	}
}
void snake_body_update()
{
	int *temp_x=new int[snake_size];
	int *temp_y=new int[snake_size];
	for (int i=0;i<snake_size-1;++i)
	{
		temp_x[i]=snake_body_x[i];
		temp_y[i]=snake_body_y[i];
	}
	switch(direction)
	{
		case 0:
		temp_x[snake_size-1]=temp_x[snake_size-2]+10;
		temp_y[snake_size-1]=temp_y[snake_size-2];
		break;
		case 1:
		temp_x[snake_size-1]=temp_x[snake_size-2]-10;
		temp_y[snake_size-1]=temp_y[snake_size-2];
		break;
		case 2:
		temp_x[snake_size-1]=temp_x[snake_size-2];
		temp_y[snake_size-1]=temp_y[snake_size-2]-10;
		break;
		case 3:
		temp_x[snake_size-1]=temp_x[snake_size-2];
		temp_y[snake_size-1]=temp_y[snake_size-2]+10;
		break;
		
	}
	delete [] snake_body_x;
	delete [] snake_body_y;
	snake_body_x=temp_x;
	snake_body_y=temp_y;
}
void food_pick()
{
	for (int i=0;i<5;++i)
	{
		if ((board_x[head_x]==fruit_x[i])&&(board_y[head_y]==fruit_y[i]))
		{
			++snake_size;
			fruit_place(i);
			snake_body_update();
			score+=5;
		}
	}
}
void snake_body_coords(int *a_x,int *a_y ,int b_x[],int b_y[],int &n,int &dir)
{
	int prev_x=a_x[0];
	int prev_y=a_y[0];
	a_x[0]=temp_body_x;
	a_y[0]=temp_body_y;
	int temp1,temp2;	
	for (int i=1;i<n;++i)
	{	
		temp1=a_x[i];
		temp2=a_y[i];
		a_x[i]=prev_x;
		a_y[i]=prev_y;
		prev_x=temp1;
		prev_y=temp2;
	}
}
void snake_head_draw()
{
	int color_index;
	switch(color)
	{
		case 0:
		color_index=5;
		break;
		case 1:
		color_index=6;
		break;
		case 2:
		color_index=13;
		break;
		case 3:
		color_index=24;
		break;
		case 4:
		color_index=35;
		break;
		case 5:
		color_index=65;
		break;
		case 6:
		color_index=71;
		break;
		case 7:
		color_index=0;
		break;
	}
	switch(direction)
	{
		case 0:
		{
			DrawCircle(board_x[head_x]+3,board_y[head_y]+4,6,colors[color_index]);
		}
		break;		
		case 1:
		{
			DrawCircle(board_x[head_x]-3,board_y[head_y]+4,6,colors[color_index]);
		}
		break;
		case 2:
		{
			DrawCircle(board_x[head_x]+5,board_y[head_y]-2,6,colors[color_index]);
		}
		break;
		case 3:
		{
			DrawCircle(board_x[head_x]+5,board_y[head_y]+2,6,colors[color_index]);
		}
		break;
	}
}
void snake_draw(int *a_x,int *a_y,int n)
{
	snake_head_draw();	
	switch(color)
	{
		case 0:
		for (int i=0;i<n;++i)
		{
			DrawRoundRect(snake_body_x[i],snake_body_y[i],10,10,colors[i-5*(i/5)],5);
		}
		break;
		case 1:
		for (int i=0;i<n;++i)
		{
			DrawRoundRect(snake_body_x[i],snake_body_y[i],10,10,colors[6+i-6*(i/6)],5);
		}
		break;
		case 2:
		for (int i=0;i<n;++i)
		{
			DrawRoundRect(snake_body_x[i],snake_body_y[i],10,10,colors[13+i-11*(i/11)],5);
		}
		break;
		case 3:
		for (int i=0;i<n;++i)
		{
			DrawRoundRect(snake_body_x[i],snake_body_y[i],10,10,colors[24+i-11*(i/11)],5);
		}
		break;
		case 4:
		for (int i=0;i<n;++i)
		{
			DrawRoundRect(snake_body_x[i],snake_body_y[i],10,10,colors[35+i-30*(i/30)],5);
		}
		break;
		case 5:
		for (int i=0;i<n;++i)
		{
			DrawRoundRect(snake_body_x[i],snake_body_y[i],10,10,colors[65+i-6*(i/6)],5);
		}
		break;
		case 6:
		for (int i=0;i<n;++i)
		{
			DrawRoundRect(snake_body_x[i],snake_body_y[i],10,10,colors[71+i-20*(i/20)],5);
		}
		break;
		case 7:
		for (int i=0;i<n;++i)
		{
			DrawRoundRect(snake_body_x[i],snake_body_y[i],10,10,colors[i*3-30*(i/30)],5);
		}
		break;
	}
	
}
void collision()
{
	for (int i=0;i<snake_size;++i)
	{
		if((board_x[head_x]==snake_body_x[i])&&(board_y[head_y]==snake_body_y[i]))
		{
			perv_game_status=0;
			game_end();
		}
	}
	for (int i=0;i<diffculty;++i)
	{
		if (hurdle_type[i]==0)
		{
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+10)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+20)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+30)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+40)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
		}
		else if(hurdle_type[i]==1)
		{
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]+10))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]+20))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]+30))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]+40))
			{
				perv_game_status=0;
				game_end();
			}
		}
		else if(hurdle_type[i]==2)
		{
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+10)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+20)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+20)&&(board_y[head_y]==hurdle_y[i]+10))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+20)&&(board_y[head_y]==hurdle_y[i]+20))
			{
				perv_game_status=0;
				game_end();
			}			
		}
		else if(hurdle_type[i]==3)
		{
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]-10))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]-20))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+10)&&(board_y[head_y]==hurdle_y[i]-20))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+20)&&(board_y[head_y]==hurdle_y[i]-20))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+30)&&(board_y[head_y]==hurdle_y[i]-20))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+30)&&(board_y[head_y]==hurdle_y[i]-10))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+30)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}				
		}
		else if(hurdle_type[i]==4)
		{
			if((board_x[head_x]==hurdle_x[i])&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+10)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+20)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+30)&&(board_y[head_y]==hurdle_y[i]))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+30)&&(board_y[head_y]==hurdle_y[i]-10))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+30)&&(board_y[head_y]==hurdle_y[i]-20))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+30)&&(board_y[head_y]==hurdle_y[i]-30))
			{
				perv_game_status=0;
				game_end();
			}
			if((board_x[head_x]==hurdle_x[i]+40)&&(board_y[head_y]==hurdle_y[i]-30))
			{
				perv_game_status=0;
				game_end();
			}	
			if((board_x[head_x]==hurdle_x[i]+50)&&(board_y[head_y]==hurdle_y[i]-30))
			{
				perv_game_status=0;
				game_end();
			}	
			if((board_x[head_x]==hurdle_x[i]+60)&&(board_y[head_y]==hurdle_y[i]-30))
			{
				perv_game_status=0;
				game_end();
			}
		}	
	}
}
void initiliaze_boundries()
{
	for (int i=0;i<80;++i)
	{
		boundry[0][i][0]=i*10;
		boundry[0][i][1]=0;
		boundry[1][i][0]=i*10;
		boundry[1][i][1]=790;
		boundry[2][i][0]=0;
		boundry[3][i][0]=790;
		boundry[2][i][1]=i*10;
		boundry[3][i][1]=i*10;
	}	
}
void Drawboundry()
{
	for (int i=0;i<80;++i)
	{
		DrawSquare(boundry[0][i][0],boundry[0][i][1],10,colors[65]);
		DrawSquare(boundry[1][i][0],boundry[1][i][1],10,colors[65]);
		DrawSquare(boundry[2][i][0],boundry[2][i][1],10,colors[65]);
		DrawSquare(boundry[3][i][0],boundry[3][i][1],10,colors[65]);
	}
}
void game()
{
	escape_press=0;
	menu=0;
	game_started=1;
	has_game_played_once=1;
	progress_bar=(520.0*score)/highscore;
	score_to_string(score,score_string);
	DrawLine(0,800,800,800,2,colors[55]);
	DrawLine(0,800,800,800,2,colors[55]);
	DrawLine(180,820,700,820,10,colors[55]);
	if (score<highscore)
	{
		DrawLine(180,820,progress_bar+180,820,10,colors[25]);
	}
	else
	{
		DrawLine(180,820,700,820,10,colors[25]);
	}
	DrawString(20,850,"SCORE:",colors[39]);
	DrawString(20,810,"HIGHSCORE:",colors[39]);
	DrawString(120,850,score_string,colors[39]);
	space_press?pause_game=1:pause_game=0;
	if (!pause_game)
	{
		movement(head_x,head_y,direction);
		snake_body_coords(snake_body_x,snake_body_y,board_x,board_y,snake_size,direction);
		collision(); 
		hurdle_timer();
		fruit_time_update();
		bonus_food();
		bonus_food_place();
		food_pick();
	}
	if (game_mode==1)
	{
		Drawboundry();	
	}
	snake_draw(snake_body_x,snake_body_y,snake_size);	
	draw_hurdle();
	draw_food();
}
void menu_screen()
{
	game_started=0;
	int bar[5]={710,650,570,510,450};
	DrawRoundRect(275,bar[menu_counter],200,30,colors[45],10);
	DrawString(320,850,"SNAKE XENZIA",colors[65]);
	DrawString(150,780,"Use the Up Down keys to select. Press Space to enter",colors[39]);
	DrawString(280,720,"START A NEW GAME",colors[52]);
	DrawString(280,660,"RESUME PREVIOUS GAME",colors[53]);
	DrawString(280,580,"CONTROLS",colors[54]);
	DrawString(280,520,"GAME HISTORY",colors[55]);
	DrawString(280,460,"EXIT",colors[56]);
	DrawString(280,400,"HIGHSCORE:",colors[57]);
	DrawString(600,10,"NABEEL DANISH",colors[35]);
	score_to_string(highscore,highscore_string);
	DrawString(550,400,highscore_string,colors[58]);
	
	if(space_press)
	{
		switch(menu_counter)
		{
			case 0:
				menu=2;
			break;
			case 1:
				menu=4;
			break;
			case 2:
				menu=5;
			break;
			case 3:
				menu=3;
			break;
			case 4:
				exit(1);
			break;
			
		}
		space_press=0;
	}
}
void menu_diffculty()
{
	int bar[3]={710,650,570};
	DrawRoundRect(275,bar[menu_counter],200,30,colors[45],10);
	DrawString(320,850,"CHOOSE DIFFICULTY",colors[52]);
	DrawString(150,780,"Use the Up Down keys to select. Press Space to enter",colors[39]);	
	DrawString(280,720,"LEVEL 1",colors[53]);
	DrawString(280,660,"LEVEL 2",colors[54]);
	DrawString(280,580,"LEVEL 3",colors[55]);
	if(space_press)
	{
		menu=7;
		diffculty=menu_counter+1;
		space_press=0;
	}	
}
void game_history()
{
	DrawString(320,850,"PREVIOUS SCORES",colors[53]);
	for (int i=0;i<5;++i)
	{
		score_to_string(perv_score[i],prevscore_string[i]);
	}
	for (int i=0;i<5;++i)
	{
		DrawString(200,660-i*60,"SCORE: ",colors[54]);
		DrawString(320,660-i*60,prevscore_string[i],colors[55]);
	}
	DrawString(200,200,"PRESS SPACE TO GO BACK",colors[35]);
	if(space_press)
	{
		menu=1;
		space_press=0;
	}
}
void resume_game()
{
	if (has_game_played_once)
	{
		if (perv_game_status)
		{
			game();
		}
		else
		{
			DrawString(200,620,"YOU LOST IN THE LAST GAME",colors[35]);
			DrawString(200,200,"PRESS SPACE TO GO BACK TO MAIN MENU",colors[35]);
			if(space_press)
			{
				menu=1;
				space_press=0;
			}
		}
	}
	else
	{
		file.open("save-file.txt",ios::in);
		file>>perv_game_status;
		if (!perv_game_status)
		{
			DrawString(200,620,"YOU LOST IN THE LAST GAME",colors[35]);
			DrawString(200,200,"PRESS SPACE TO GO BACK TO MAIN MENU",colors[35]);
			if(space_press)
			{
				menu=1;
				space_press=0;
			}
		}
		else
		{
			file>>highscore;
			for (int i=0;i<5;++i)
			{
				file>>perv_score[i];
			}
			score=perv_score[0];
			for (int i=0;i<5;++i)
			{
				cout<<perv_score[i]<<endl;
			}
			file>>head_x>>head_y>>snake_size;
			int *temp_arr_1=new int[snake_size];
			int *temp_arr_2=new int[snake_size];
			snake_body_x=temp_arr_1;
			snake_body_y=temp_arr_2;
			for (int i=0;i<snake_size;++i)
			{
				file>>snake_body_x[i]>>snake_body_y[i];
			}
			for (int i=0;i<5;++i)
			{
				file>>fruit_x[i]>>fruit_y[i];
			}
			file>>diffculty>>color>>direction;
			for(int i=0;i<3;++i)
			{
				file>>hurdle_type[i];
			}
			for (int i=0;i<3;++i)
			{
				file>>hurdle_x[i]>>hurdle_y[i];
			}
			file>>game_mode;
			cout<<"GAME MODE: "<<game_mode<<endl;
			if(game_mode)
			{
				initiliaze_boundries();
			}
			game_resumed=1;
			file.close();
			menu=0;
			game_started=1;
			
		}
	}	
}
void help_screen()
{
	DrawString(320,850,"HELP SCREEN",colors[5]);
	DrawString(200,770,"Use ARROW/WASD KEYS to move the snake",colors[35]);
	DrawString(200,720,"Press SPACE to pause the game",colors[35]);
	DrawString(200,670,"Press C to change colors of the snake",colors[35]);
	DrawString(200,620,"Press Escape to Quit",colors[35]);
	DrawString(200,200,"PRESS SPACE TO GO BACK TO MAIN MENU",colors[35]);
	if(space_press)
	{
		menu=1;
		space_press=0;
	}
}
void start_game()
{
	perv_game_status=1;
	head_x=50;
	head_y=50;
	snake_size=5;
	snake_body_x=new int[snake_size];
	snake_body_y=new int[snake_size];
	snake_body_coords_init(snake_body_x,snake_body_y,board_x,board_y,snake_size);
	snake_body_coords(snake_body_x,snake_body_y,board_x,board_y,snake_size,direction);
	game_started=1;
	score=0;
	color=5;
	bonus_food_time=0;
	hurdle_time=0;
	fruit_scatter();
	scatter_hurdles();
}
void level_choose()
{
	int bar[2]={710,650};
	DrawRoundRect(195,bar[menu_counter],200,30,colors[45],10);
	DrawString(320,850,"CHOOSE GAME MODE",colors[47]);
	DrawString(200,720,"BOUNDRY-LESS ENDLESS",colors[35]);
	DrawString(200,660,"CLASSIC",colors[35]);
	if(space_press)
	{
		menu=0;
		game_mode=menu_counter;
		if (game_mode==1)
		{
			initiliaze_boundries();
		}
		space_press=0;
	}
}
#endif
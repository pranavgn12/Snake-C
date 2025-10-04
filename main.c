#include "raylib.h"	// "" maybe used for header created by user and not defualt
#include <time.h>
#include <stdlib.h>
#define w   900		// width of window in pixels
#define hs	30	// width and height of square shaped head in pixels
#define h	(w+2*hs)		// height of window in pixels

//Declare global variables

int l;		   		//  length of body
int d;				//  d->0 top, d->1 right, d->2 bottom, d->2 left
int hx,hy;    		//  Head position of snake 
int tx,ty; 			//	To trace snake body from head using recent keystrokes data
int fx,fy;			//	Food position
int ks[hs*hs]; 	//  Store history of keypresses to draw rest of body of snake
int kp;				//  Store keycode of key pressed
int i,j;			// 	for loops
int sc;				// 	Score
int sp;				// 	Speed (Not using this anymore as speed could be controlled using delay!)
int dly;			// 	Delay
int p;				// To store previous fps


//Declare functions

void draw_frame();          //	Display graphics
void init_game();				//	Reset eveything
void update();				//	Change the game data by talking keyboard input
int check_food_overlap();	// 	As the name indicates , Returns 1 if it overlaps else returns 0
void move_head();			// 	To move head with speed without skipping grid ensuring collsion doesn't miss
void trace_back();			// 	To get position of parts of snake body by tacing back from head postion by single step
void delay();				//	Delay
void get_dir();				//	Get direction from key press
void gameover();			//	Gameover screen
void propogate();

void init_game()
{
	
	// ininitially length of snake is 1 and its headpostion is a center
	
	l  = 0;
	hx = ((hs-1)/2)*hs;					
	hy = (((hs-1)/2)+2)*hs;
	sc = 0;
	sp = 1;
	dly = 10000000;
	srand(time(NULL));
	p=10;							  // To store previous fps
    SetTargetFPS(p);               // Set our game to run at 60 frames-per-second
	// Randomize food position in the begining and size of food is same as head
	
	fx = rand()%(hs-1-0+1) * hs;  // spawn using grid
	fy = 2*hs + rand()%(hs-1-0+1) * hs;  // spawn using grid
	
	
	//direction is random at the begining 
	//rand()%(max-min+1) gives random number between max and min
	
	d = rand()%(3-0+1);
	
}


int check_food_overlap()  // Check if food touches snakes body by tracing
{	
	tx=hx;
	ty=hy;
	for( j=1; j<l+1; j++ )
	{
		if( fx==tx && fy==ty )
			return 1;
		trace_back();
	}
	return 0;
}

int head_touch_body() //Check if head tocuhes snakes body by tracing
{	tx=hx;
	ty=hy;
	for( j=1; j<l; j++ )
	{	
		trace_back();
		
		if(	hx==tx && hy==ty )
			return 1;
	}
	return 0;
}

void trace_back()       // Getting xy cordinates (tx & ty) of every part of snake by tracing from head 
{						// One part at a time by part number j
	if(ks[j-1]==0)		
			
		if(ty+hs<h)
			ty+=hs;
		else
			ty=2*hs;
	else if(ks[j-1]==1)
			
		if(tx-hs>=0)
			tx-=hs;
		else
			tx=w-hs;
	else if(ks[j-1]==2)
			
		if(ty-hs>=2*hs)
			ty-=hs;
		else
			ty=h-hs;
	else if(ks[j-1]==3)
			
		if(tx+hs<w)
			tx+=hs;
		else
			tx=0;
}


void move_head()                //Move head in the direction determined by keyboard
{	

	if(d==0)
		for(i=0; i<sp; i++)		//sp is not used and remains one as speed is controlled using fps
		{	
			if(hy-hs>=2*hs)
			hy-=hs;
			else
			hy=h-hs;
			if(head_touch_body())
			{
				gameover();
			}
		}
		
	else if(d==1)
		for(i=0; i<sp; i++)
		{
			if(hx+hs<=w-hs)
			hx+=hs;
			else
			hx=0;
			if(head_touch_body())
			{
				gameover();
			}
		}
	else if(d==2)
		for(i=0; i<sp; i++)
		{
			if(hy+hs<=h-hs)
			hy+=hs;
			else
			hy=2*hs;
			if(head_touch_body())
			{
				gameover();
			}
		}
	
	else
		for(i=0; i<sp; i++)
		{
			if(hx>=hs)
			hx-=hs;
			else
			hx=w-hs;
			if(head_touch_body())
			{
				gameover();
			}
		}
	
	propogate();		//	Propogate the snake curve throughout its body
}


void propogate(){
	for( i=l; i>0; i-- )
		ks[i] = ks[i-1]; 
	ks[0] = d;
}


void update()
{								// Using Fps as speed control. Score determines speed
	if(p!=(int)sc/40+10)		// I dont need to set everytime fps insted only set when it changes
		{
			SetTargetFPS((int)sc/40+10);
			p=(int)sc/40+10;
		}
	
	
	//	Get direction from pressing	Keys
	get_dir();

	//Move in direction
	move_head();
	
	
	
	/*
	When snake head touches food (ie xy co-ordinate of snake head = xy co-ordinate of food)
	increase score and assign a a new food postion
	*/
	
	if(hx == fx && hy == fy)
	{
		sc+=10;
		l++;
		//check if food spawns on top of snake body and if it is then find new random postion
		do{
		
		// Spawn food at random position

			fx = rand()%(hs-1-0+1) * hs;  // spawn using grid
			fy = 2*hs + rand()%(hs-1-0+1) * hs;  // spawn using grid
		
		} while(check_food_overlap());
		
	}	
	
	
	  
	
}



void draw_frame()   //Draws on Screen after calculating everything using update
{	
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawText("Score:", hs+10, hs-10, 40, BLACK); 
	DrawText(TextFormat("%i", sc), hs+150, hs-10, 40, BLUE);
	//DrawText(TextFormat("%i %i", fx,fy), hs+250, hs-10, 40, BLUE);
	DrawRectangle(hx, hy, hs-2, hs-2, BLACK);
	//j=1;
	tx=hx;
	ty=hy;
	for(j=1; j<l+1; j++)
	{
	trace_back();
	DrawRectangle(tx, ty, hs-2, hs-2, DARKGRAY);
	}
	DrawCircle(fx+hs/2, fy+hs/2, hs/2, RED);
	EndDrawing();
	
}


void delay(){      //not used
	for(i=0; i<dly && !WindowShouldClose(); i++)
	{
		
	}
}




void get_dir(){
	 
	if((IsKeyPressed( KEY_UP) || IsKeyPressed( KEY_W) ) && (d!=2))
	{	
		d = 0;
	}
	else if((IsKeyDown( KEY_RIGHT) || IsKeyDown( KEY_D) ) && (d!=3))
	{
		d = 1;
	}
	else if((IsKeyDown( KEY_DOWN) || IsKeyDown( KEY_S) ) && (d!=0))
	{	
		d = 2;
	}
	else if((IsKeyDown( KEY_LEFT) || IsKeyDown( KEY_A) ) && (d!=1))
	{	
		d = 3;
	}
	
	
}


void gameover(){          // To show gameover screen  
	
	while(!WindowShouldClose() && !IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Game Over!", w/6, h/8, 100, BLACK); 
		DrawText(TextFormat("Score: %i", sc), w/4 + 70, h/6 + 100, 60, BLUE);
		DrawText("Click to restart", w/4-20, h/2 + 100, 60, DARKGRAY); 
		EndDrawing();
	}
	if(!WindowShouldClose())
	{
		init_game();
		update();
	}
	else CloseWindow();
}









//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    InitWindow(w, h, "Snake");
	init_game();
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {	
		update();
		draw_frame();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
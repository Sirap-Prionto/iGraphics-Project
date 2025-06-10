#include "iGraphics.h"


int width = 500, height = 650;
int ball_radius = 10;


typedef struct{
    int x,y;
    //0 for red, 1 for green, 2 for blue
    int red,green,blue;

    int exist;
}staticBall;

//number of static balls = 500 / (10 * 2) * 3(rows);
staticBall all_static_balls[3][25];


void draw_a_static_ball(staticBall aBall){
    if(aBall.exist){
        iSetColor(aBall.red,aBall.green,aBall.blue);
        iFilledCircle(aBall.x,aBall.y,ball_radius);
    }
}

void draw_all_static_ball()
{
    for(int i=0; i<3; i++){

        for(int j=0;j<25;j++){
            draw_a_static_ball(all_static_balls[i][j]);
        }
    }
}
void fillwithballs(){
    int c = 0;
    for(int i=0;i<3;i++){

        for(int j =0;j<25;j++){

            if(i==1 && j==25){
                continue;
            }
            staticBall tempBall;



            //ekhane ekta switchcase diye ball er x fix korte hobe


            tempBall.x =(2*j+1)*ball_radius;
            if(i==1){
                tempBall.x+=ball_radius;
                if(j==24) continue;
            }
            tempBall.y = height - (2*i+1)*ball_radius;
            tempBall.exist =1;

            switch(c%3){
                case 0:tempBall.red =255;tempBall.blue =0;tempBall.green =0;break;
                case 1:tempBall.green =255;tempBall.red =0;tempBall.blue =0;break;
                case 2:tempBall.blue =255;tempBall.red =0;tempBall.green =0;break;
            }
            all_static_balls[i][j] = tempBall;
            c++;
        }
    }
}
void noballs()
{
    staticBall emptyBall;
    for(int i = 0;i<3;i++){
        for(int j=0;j<25;j++){
            all_static_balls[i][j] = emptyBall;
        }
    }
}
void drawAxis()
{
    iSetColor(255, 255, 255);
    iLine(0, 50, width, 50);
}


double angle = 0;


double ball_x = 250;
double ball_y = 50;
int throw_ball = 0;
double dx;
double dy;

void setBall()
{
    throw_ball = 1;
    dx = .4*sin(angle * 3.1416/180);
    dy = .4*cos(angle * 3.1416/180);
}

void resetBall()
{
    throw_ball = 0;
    ball_x = 250;
    ball_y = 50;
}
int color_counter = 0;
int r = 255;
int g = 0;
int b = 0;
void drawBall()
{

    iSetColor(r,g,b);
    iFilledCircle(ball_x,ball_y, ball_radius);

    if(ball_x - ball_radius<0 || ball_x + ball_radius > width)
        dx = -dx;

    if(ball_y + ball_radius > height)
        dy = -dy;



    ball_x+=dx;
    ball_y+=dy;



    if(ball_y - ball_radius<0){
        resetBall();
        color_counter++;
        switch(color_counter%3){
            case 1: r = 0; g = 255; b = 0;break;
            case 2: r = 0; g = 0; b = 255;break;
            case 0: r = 255; g = 0; b = 0;break;
        }

    }

}
void drawCannon()
{
    double cannon_x = 30*sin(angle * 3.1416/180);
    double cannon_y = 30*cos(angle * 3.1416/180);
    iSetLineWidth(2);
    iSetColor(255,255,255);
    iLine(250-10,50,250+cannon_x-10,50+cannon_y);
    iLine(250+10,50,250+cannon_x+10,50+cannon_y);
    if(throw_ball==0){
        iSetColor(r,g,b);
        iFilledCircle(250,40, ball_radius);
    }
}

/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    // place your drawing codes here
    iClear();
    iText(200, 300, "Hello World");



    iSetLineWidth(3);
    drawAxis();

    draw_all_static_ball();
    drawCannon();

    if(throw_ball)
        drawBall();



}
int checkco

/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case 'q':
        // do something with 'q'
        break;
    case 'a':
        if(angle>-85)
            angle-=5;
        break;
    case 'd':
        if(angle<85)
            angle+=5;
        break;
    case 'w':
        if(throw_ball==0)
            setBall();
        break;
    case 'f':
        fillwithballs();
        break;
    case 'r':
        noballs();
        break;

    // place your codes for other keys here
    default:
        break;
    }
}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_END:
        // do something
        break;

    // place your codes for other keys here
    default:
        break;
    }
}

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    // place your own initialization codes here.
    iInitialize(width, height, "Bouncing Balls");

    return 0;
}

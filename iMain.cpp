#include "iGraphics.h"


int width = 500, height = 650;
int ball_radius = 10;
int ball_diameter = 2*ball_radius;


typedef struct{
    int red,green,blue;
    int exist;
    int x,y;
}staticBall;

//number of static balls = 500 / (10 * 2) * 3(rows);
staticBall all_static_balls[31][25];


void draw_a_static_ball(staticBall aBall){


    iSetColor(aBall.red,aBall.green,aBall.blue);
    iFilledCircle(aBall.x,aBall.y,ball_radius);
}

void draw_all_static_ball()
{
    for(int i=0; i<30; i++){

        for(int j=0;j<25;j++){
            if(all_static_balls[i][j].exist)
                draw_a_static_ball(all_static_balls[i][j]);
        }
    }
}
//-------ball er co ordinate draw a static ball e thakbe-----------
staticBall emptyBall;
void set_coordinates(){
    for(int i = 0;i<31;i++){
        for(int j=0;j<25;j++){

            all_static_balls[i][j].x = (2*j+1)*ball_radius;
            all_static_balls[i][j].y = height - (2*i+1)*ball_radius;

        }
    }
}
void set_static_ball(int red,int green,int blue,int i,int j){
    all_static_balls[i][j].exist = 1;
    all_static_balls[i][j].red = red;
    all_static_balls[i][j].green = green;
    all_static_balls[i][j].blue = blue;
    all_static_balls[i][j].x = (2*j+1)*ball_radius;
    all_static_balls[i][j].y = height - (2*i+1)*ball_radius;
}



void fillwithballs(){

    for(int i=0;i<5;i++){
        int c = 0;
        for(int j =0;j<25;j++){
            staticBall tempBall;
            tempBall.exist =1;
            switch(c%6){
                case 0:tempBall.red =255;tempBall.blue =0;tempBall.green =0;break;
                case 1:tempBall.green =255;tempBall.red =0;tempBall.blue =0;break;
                case 2:tempBall.blue =255;tempBall.red =0;tempBall.green =0;break;
                case 3:tempBall.blue =0;tempBall.red =255;tempBall.green =255;break;
                case 4:tempBall.blue =255;tempBall.red =0;tempBall.green =255;break;
                case 5:tempBall.blue =255;tempBall.red =255;tempBall.green =0;break;
            }
            all_static_balls[i][j] = tempBall;
            c++;
        }
    }

    for(int i = 5;i<31;i++){
        for(int j=0;j<25;j++){
            all_static_balls[i][j] = emptyBall;
        }
    }
    set_coordinates();
    set_static_ball(255,255,255,20,20);
}
//void loweringstaticball(){ball.x;ball.y++} --------time mode er gameplay er jonno-----------

void fillwithdiamond()
{


        set_static_ball(255,0,0,5,10);
        set_static_ball(0,255,0,5,11);
        set_static_ball(0,0,255,5,12);
        set_static_ball(0,0,255,5,13);
        set_static_ball(0,255,0,5,14);
        set_static_ball(255,0,0,5,15);

        set_static_ball(255,0,0,6,9);
        set_static_ball(0,255,0,6,10);
        set_static_ball(0,0,255,6,11);
        set_static_ball(0,255,255,6,12);
        set_static_ball(0,255,255,6,13);
        set_static_ball(0,0,255,6,14);
        set_static_ball(0,255,0,6,15);
        set_static_ball(255,0,0,6,16);




}


void noballs()
{
    for(int i = 0;i<30;i++){
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
double velocity = 5;
int color_counter = 0;
int r = 255;
int g = 0;
int b = 0;
void setBall()
{
    throw_ball = 1;
    dx = velocity*sin(angle * 3.1416/180);
    dy = velocity*cos(angle * 3.1416/180);
}

void resetBall()
{
    throw_ball = 0;
    ball_x = 250;
    ball_y = 50;
    color_counter++;
    switch(color_counter%6){
        case 0: r = 255; g = 0; b = 0;break;
        case 1: r = 0; g = 255; b = 0;break;
        case 2: r = 0; g = 0; b = 255;break;
        case 3: r = 0; g = 255; b = 255;break;
        case 4: r = 255; g = 0; b = 255;break;
        case 5: r = 255; g = 255; b = 0;break;

    }
}


void drawCannon() //---------------------working alright-------------------------------
{
    double cannon_x = 30*sin(angle * 3.1416/180);
    double cannon_y = 30*cos(angle * 3.1416/180);
    iSetLineWidth(2);
    iSetColor(255,255,255);
    iLine(250-10,50,250+cannon_x-10,50+cannon_y);
    iLine(250+10,50,250+cannon_x+10,50+cannon_y);
    iSetLineWidth(1);
    iSetColor(r,g,b);
    iLine(250,50,250+5*cannon_x,50+5*cannon_y);
    if(throw_ball==0){
        iSetColor(r,g,b);
        iFilledCircle(250,40, ball_radius);
    }
}
int combo = 0;
void check_neighbour(int i,int j){
    combo++;
    all_static_balls[i][j].exist=0;
    if (i!=0 && all_static_balls[i-1][j].exist){
        if (r==all_static_balls[i-1][j].red && g==all_static_balls[i-1][j].green && b==all_static_balls[i-1][j].blue){
        check_neighbour(i-1,j);
        }
    }
    if (all_static_balls[i+1][j].exist){
        if (r==all_static_balls[i+1][j].red && g==all_static_balls[i+1][j].green && b==all_static_balls[i+1][j].blue){
        check_neighbour(i+1,j);
        }
    }
    if (j!=0 && all_static_balls[i][j-1].exist){
        if (r==all_static_balls[i][j-1].red && g==all_static_balls[i][j-1].green && b==all_static_balls[i][j-1].blue){
        check_neighbour(i,j-1);
        }
    }
    if (j!=24 && all_static_balls[i][j+1].exist){
        if (r==all_static_balls[i][j+1].red && g==all_static_balls[i][j+1].green && b==all_static_balls[i][j+1].blue){
        check_neighbour(i,j+1);
        }
    }
}

void check_collision(int i, int j){
    int startchecking = 0;
    if (all_static_balls[i][j].exist){
        if(all_static_balls[i+1][j].exist==0){
            i = i+1;
            j = j;
        }
        else if(dx>0){
            if(all_static_balls[i][j-1].exist==0){
                i = i;
                j = j-1;
            }
            else{
                i = i+1;
                j = j-1;
            }

        }
        else if(dx<0){
            if(all_static_balls[i][j-1].exist==0){
                i = i;
                j = j+1;
            }
            else{
                i = i+1;
                j = j-1;
            }
        }
        startchecking = 1;
    }
    if(ball_y + ball_radius > height) startchecking = 1;
    //checking around i,j
    if(startchecking){
    if (i!=0 && all_static_balls[i-1][j].exist){
        if (r==all_static_balls[i-1][j].red && g==all_static_balls[i-1][j].green && b==all_static_balls[i-1][j].blue){
        combo++;
        check_neighbour(i-1,j);
        all_static_balls[i-1][j].exist=1;
        }
    }
    if (i!=29 && all_static_balls[i+1][j].exist){
        if (r==all_static_balls[i+1][j].red && g==all_static_balls[i+1][j].green && b==all_static_balls[i+1][j].blue){
        combo++;
        check_neighbour(i+1,j);
        all_static_balls[i+1][j].exist=1;
        }
    }
    if (j!=0 && all_static_balls[i][j-1].exist){
        if (r==all_static_balls[i][j-1].red && g==all_static_balls[i][j-1].green && b==all_static_balls[i][j-1].blue){
        combo++;
        check_neighbour(i,j-1);
        all_static_balls[i][j-1].exist=1;
        }
    }
    if (j!=24 && all_static_balls[i][j+1].exist){
        if (r==all_static_balls[i][j+1].red && g==all_static_balls[i][j+1].green && b==all_static_balls[i][j+1].blue){
        combo++;
        check_neighbour(i,j+1);
        all_static_balls[i][j+1].exist=1;
        }
    }

    if(combo<=2){
        all_static_balls[i][j].exist=1;
        all_static_balls[i][j].red=r;
        all_static_balls[i][j].green=g;
        all_static_balls[i][j].blue=b;
    }
    else{
        if (all_static_balls[i-1][j].exist){
            if (r==all_static_balls[i-1][j].red && g==all_static_balls[i-1][j].green && b==all_static_balls[i-1][j].blue){
                all_static_balls[i-1][j].exist=0;
            }
        }
        if (all_static_balls[i+1][j].exist){
            if (r==all_static_balls[i+1][j].red && g==all_static_balls[i+1][j].green && b==all_static_balls[i+1][j].blue){
                all_static_balls[i+1][j].exist=0;

            }
        }
        if (j!=0 && all_static_balls[i][j-1].exist){
            if (r==all_static_balls[i][j-1].red && g==all_static_balls[i][j-1].green && b==all_static_balls[i][j-1].blue){
                all_static_balls[i][j-1].exist=0;
            }
        }
        if (j!=24 && all_static_balls[i][j+1].exist){
            if (r==all_static_balls[i][j+1].red && g==all_static_balls[i][j+1].green && b==all_static_balls[i][j+1].blue){
                all_static_balls[i][j+1].exist=0;
            }
        }
    }
    resetBall();
    }


}

void drawBall()
{

    iSetColor(r,g,b);
    iFilledCircle(ball_x,ball_y, ball_radius);

    if(ball_x - ball_radius<0 || ball_x + ball_radius > width)
        dx = -dx;

        ball_x+=dx;
        ball_y+=dy;
    /*if(ball_y - ball_radius<0){
        resetBall();
    }*/

}
//----------------------game over function --------------------------------------
void gameover(){
    for(int j = 0;j<25;j++){
        if(all_static_balls[30][j].exist){
            exit(0);
        }
    }
}

/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    // place your drawing codes here
    iClear();



    int j = ball_x / ball_diameter;
    int i = (height - ball_y) / ball_diameter;

    iSetLineWidth(3);
    drawAxis();

    draw_all_static_ball();
    drawCannon();
    check_collision(i,j);

    gameover();

    if(throw_ball)
        drawBall();

    char cmb[12];
    sprintf(cmb,"COMBO: %i",(combo>=2)?combo:0);
    iSetColor(255,255,255);
    iText(200, 300, "Hello World");
    iText(10, 25, cmb);
}


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
        if(angle>-80)
            angle-=2.5;
        break;
    case 'd':
        if(angle<80)
            angle+=2.5;
        break;
    case 'w':
        if(throw_ball==0)
            combo=0;
            setBall();
        break;
    case 'f':
        fillwithballs();
        //fillwithdiamond();
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

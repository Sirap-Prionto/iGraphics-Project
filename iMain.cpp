#include "iGraphics.h"
#include "iSound.h"
#include <bits/stdc++.h>
#include <windows.h>
#include <time.h>

using namespace std;
/*
function iDraw() is called again and again by the system.
*/
int hoveredSaveIdx = -1;
int score_screen = 0;
int savePrompt = 0; // 0=no,1=asking,2=confirmed,3=denied
int saveAction = 0; // 1=menu,2=status,3=modes
int pauseStartTime = 0;
int pauseDuration = 0;
bool gamePaused = false;
int totalTime = 0;
int combo_hove = 0, option_hove = 0, time_hove = 0, rush = 0, blast = 0;
int resume = 0, restart = 0, instruc = 0, back_men = 0, back_stat = 0, back_choce = 0, sav = 0;
bool timeRushActive = false;
int timeRushRound = -1;
int hoverSoundChannel = -1;
bool hoverSoundPlaying = false;
int ch = -1, submode_hover = 0, mode_hover = 0;
double arrow_go_x[] = {319, 319, 387};
double arrow_go_x_1[] = {319, 319, 386};
double arrow_go_y[] = {100, 35, 73};
double arrow_go_y_1[] = {99, 36, 73};
double arrow_back_x[] = {92, 160, 160};
double arrow_back_x_1[] = {93, 160, 160};
double arrow_back_y[] = {73, 35, 100};
double arrow_back_y_1[] = {73, 36, 99};
bool hovered = false;
int about_us = 0, help = 0, new_game = 0, high_score = 0, exit_game = 0, settings = 0, back = 0, proceed = 0, cross = 0;
Image sc3, sc4, sc1, sc2_1, sc2_2, sc2_3, newgame00;
int click, showModeWarning = 0, showSubModeWarning = 0;
int screenCount = 0, screen = 1, option_screen = 0;
int volume = 1;
int music_vol = 1, musicPlaying = -1;
int width = 500, height = 650;
int ball_radius = 10;
int ball_diameter = 2 * ball_radius;
int name_taken = 0, in_menu = 0, in_game = 0;
bool musicPaused = false, shouldStartMuse = true;
int targetVolume = 40, currentVolume = 0;
bool fadingIn = false, fadingOut = false;
int game_muse = -1, menu_muse = -1;
int gameModeValue = 0, subModeSelect = 0;
time_t gameStartTime = 0;
bool gameOverState = false;
char playerName[30] = "";
int nameLength = 0;
bool showCursor = true;
bool showWarning = false;
int mode = 0;
int victoryType = 0;
time_t lastHoverSound = 0;
int getTimeLimitForSubmode(int mode)
{
    switch (mode)
    {
    case 1:
        return 300;
    case 2:
        return 240;
    case 3:
        return 360;
    default:
        return 300;
    }
}
int getMoveThresholdForSubmode(int mode)
{
    switch (mode)
    {
    case 1:
        return 5;
    case 2:
        return 4;
    case 3:
        return 6;
    default:
        return 5;
    }
}

void toggleCursor()
{
    showCursor = !showCursor;
}
int getCharWidth(char c)
{
    if (c == 'i' || c == 'l' || c == 'I')
        return 7;
    if (c == 'W' || c == 'M')
        return 18;
    if (c == ' ')
        return 6;
    return 14;
}

int getTextWidth(const char *text)
{
    int width = 0;
    for (int i = 0; text[i]; i++)
    {
        width += getCharWidth(text[i]);
    }
    return width;
}
struct Bubble { double x,y,speed; };
static vector<Bubble> transitionBubbles;
static bool isTransitioning = false;
static int  pendingScreenCount = -1;
static int  pendingScreen = -1;
void startScreenTransition(int newSC, int newS){
    isTransitioning= true;
    pendingScreenCount= newSC;
    pendingScreen= newS;
    transitionBubbles.clear();
    for(int i=0;i<100;i++){
      transitionBubbles.push_back({
        double(rand()%width), 0.0, double(2 + rand()%3)});
    }
}
struct ScoreEntry
{
    string name;
    int combo;
    int time;
};
static vector<ScoreEntry> highScores;
typedef struct
{
    int red, green, blue;
    int exist;
    int x, y;
} staticBall;

// number of static balls = 500 / (10 * 2) * 3(rows);
staticBall all_static_balls[31][25];

void draw_a_static_ball(staticBall aBall)
{
    iSetColor(aBall.red, aBall.green, aBall.blue);
    iFilledCircle(aBall.x, aBall.y, ball_radius);
    iSetColor(255, 255, 255);
    iFilledCircle(aBall.x - 4, aBall.y + 4, 2);
}

void draw_all_static_ball()
{
    for (int i = 0; i < 30; i++)
    {

        for (int j = 0; j < 25; j++)
        {
            if (all_static_balls[i][j].exist)
                draw_a_static_ball(all_static_balls[i][j]);
        }
    }
}
staticBall emptyBall;
void set_coordinates()
{
    for (int i = 0; i < 31; i++)
    {
        for (int j = 0; j < 25; j++)
        {

            all_static_balls[i][j].x = (2 * j + 1) * ball_radius;
            all_static_balls[i][j].y = height - (2 * i + 1) * ball_radius;
        }
    }
}
void set_static_ball(int color_code, int i, int j)
{
    // color code 0=red, 1=green,2=blue,3=cyan,4=magenta,5=yellow,6=white
    int r, g, b;
    r = g = b = 0;
    switch (color_code)
    {
    case 0:
        r = 255;
        break;
    case 1:
        g = 255;
        break;
    case 2:
        b = 255;
        break;
    case 3:
        g = 255;
        b = 255;
        break;
    case 4:
        r = 255;
        b = 255;
        break;
    case 5:
        r = 255;
        g = 255;
        break;
    case 6:
        r = 255;
        g = 255;
        b = 255;
        break;
    }
    all_static_balls[i][j].exist = 1;
    all_static_balls[i][j].red = r;
    all_static_balls[i][j].green = g;
    all_static_balls[i][j].blue = b;
    all_static_balls[i][j].x = (2 * j + 1) * ball_radius;
    all_static_balls[i][j].y = height - (2 * i + 1) * ball_radius;
}

void all_ball_down()
{
    for (int i = 30; i > 0; i--)
    {
        for (int j = 0; j < 25; j++)
        {
            all_static_balls[i][j] = all_static_balls[i - 1][j];
        }
    }
    for (int j = 0; j < 25; j++)
    {
        all_static_balls[0][j] = emptyBall;
    }
    set_coordinates();
}
void all_ball_up()
{
    int flag = 1;
    for (int j = 0; j < 25; j++)
    {
        if (all_static_balls[0][j].exist)
        {
            flag = 0;
        }
    }
    if (flag)
    {
        for (int j = 0; j < 25; j++)
        {
            all_static_balls[30][j] = emptyBall;
        }
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < 25; j++)
            {
                all_static_balls[i][j] = all_static_balls[i + 1][j];
            }
        }

        set_coordinates();
    }
}
void fillwithballs()
{

    for (int i = 0; i < 5; i++)
    {
        int c = 0;
        for (int j = 0; j < 25; j++)
        {
            staticBall tempBall;
            tempBall.exist = 1;
            switch (rand() % 6)
            {
            case 0:
                tempBall.red = 255;
                tempBall.blue = 0;
                tempBall.green = 0;
                break;
            case 1:
                tempBall.green = 255;
                tempBall.red = 0;
                tempBall.blue = 0;
                break;
            case 2:
                tempBall.blue = 255;
                tempBall.red = 0;
                tempBall.green = 0;
                break;
            case 3:
                tempBall.blue = 0;
                tempBall.red = 255;
                tempBall.green = 255;
                break;
            case 4:
                tempBall.blue = 255;
                tempBall.red = 0;
                tempBall.green = 255;
                break;
            case 5:
                tempBall.blue = 255;
                tempBall.red = 255;
                tempBall.green = 0;
                break;
            }
            all_static_balls[i][j] = tempBall;
            c++;
        }
    }

    for (int i = 5; i < 31; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            all_static_balls[i][j] = emptyBall;
        }
    }
    set_coordinates();
}

void noballs()
{
    for (int i = 0; i < 31; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            all_static_balls[i][j] = emptyBall;
        }
    }
}

void fillwithdiamond()
{
    noballs();
    set_coordinates();
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            set_static_ball(rand() % 6, i + 2, j + 13);
            set_static_ball(rand() % 6, i + 2, 11 - j);
            set_static_ball(rand() % 6, 12 - i, 11 - j);
            set_static_ball(rand() % 6, 12 - i, j + 13);
        }
    }
    for (int i = 1; i < 14; i++)
    {
        set_static_ball(rand() % 6, i, 12);
        set_static_ball(rand() % 6, 7, i + 5);
    }
    all_ball_up();
}
void fillwithpyramid()
{
    noballs();
    set_coordinates();

    for (int i = 1; i < 25; i++)
    {
        for (int j = i; j < 25 - i; j++)
        {
            set_static_ball(rand() % 6, i, j);
        }
    }
    all_ball_up();
}
void fillwithrandom()
{
    int r = rand() % 3;
    if (r == 0)
    {
        fillwithballs();
        subModeSelect = 1;
    }
    else if (r == 1)
    {
        fillwithdiamond();
        subModeSelect = 2;
    }
    else
    {
        fillwithpyramid();
        subModeSelect = 3;
    }
}

void drawAxis()
{
    iSetColor(255, 255, 255);
    iLine(0, 50, width, 50);
}

double angle = 0;
double mouse_x = 0;
double mouse_y = 5;
double mouse_r = 0;
double ball_x = 250;
double ball_y = 50;
int throw_ball = 0;
double dx;
double dy;
double velocity = 16;
int color_counter = 0;
int r = 255;
int g = 0;
int b = 0;
int r2 = 0;
int g2 = 255;
int b2 = 0;
int r3 = 0;
int g3 = 0;
int b3 = 255;
int moves = 0;
void setBall()
{
    throw_ball = 1;
    dx = velocity * mouse_x / mouse_r;
    dy = velocity * mouse_y / mouse_r;
    moves++;
}
int combo = 0;
void resetBall()
{
    throw_ball = 0;
    ball_x = 250;
    ball_y = 50;
    color_counter = rand();
    r = r2;
    g = g2;
    b = b2;
    r2 = r3;
    g2 = g3;
    b2 = b3;
    switch (color_counter % 6)
    {
    case 0:
        r3 = 255;
        g3 = 0;
        b3 = 0;
        break;
    case 1:
        r3 = 0;
        g3 = 255;
        b3 = 0;
        break;
    case 2:
        r3 = 0;
        g3 = 0;
        b3 = 255;
        break;
    case 3:
        r3 = 0;
        g3 = 255;
        b3 = 255;
        break;
    case 4:
        r3 = 255;
        g3 = 0;
        b3 = 255;
        break;
    case 5:
        r3 = 255;
        g3 = 255;
        b3 = 0;
        break;
    }
    if (combo >= 6)
    {
        r = g = b = 127;
    }
    else if (combo >= 5)
    {
        r = g = b = 63;
    }
    else if (combo >= 4)
    {
        r = g = b = 256;
    }
}

int dotdistance = 20;
int dotnum = 30;
void drawCannon()
{
    double sin = mouse_y / mouse_r;
    double cos = mouse_x / mouse_r;
    double cannon_x = 30 * cos;
    double cannon_y = 30 * sin;
    iSetLineWidth(2);
    iSetColor(255, 255, 255);
    iLine(250 - 10, 50, 250 + cannon_x - 10, 50 + cannon_y);
    iLine(250 + 10, 50, 250 + cannon_x + 10, 50 + cannon_y);
    iSetLineWidth(1);
    iSetColor(r, g, b);
    // iLine(250,50,250+3*cannon_x,50+3*cannon_y);

    if (throw_ball == 0)
    {
        double dotx = 250;
        double doty = 50;
        int dots = 0;
        while (dots < dotnum)
        {
            dots++;
            if (dotx < 0 || dotx > width)
                cos *= -1;
            if (dotx < 0)
                dotx *= -1;
            if (dotx > width)
                dotx = 2 * width - dotx;
            iFilledCircle(dotx, doty, 2);

            dotx += dotdistance * cos;
            doty += dotdistance * sin;
        }
        iSetColor(r, g, b);
        iFilledCircle(250, 40, ball_radius);
        iSetColor(255, 255, 255);
        iFilledCircle(250 - 5, 40 + 5, 2);
    }
    iSetColor(r2, g2, b2);
    iFilledCircle(252 + ball_diameter, 40, ball_radius);
    iSetColor(255, 255, 255);
    iFilledCircle(252 + ball_diameter - 5, 40 + 5, 2);
    iSetColor(r3, g3, b3);
    iFilledCircle(254 + ball_diameter * 2, 40, ball_radius);
    iSetColor(255, 255, 255);
    iFilledCircle(254 + ball_diameter * 2 - 5, 40 + 5, 2);
}
double poppingsize = 2;
int sleeptime = 20;

void check_neighbour(int i, int j)
{
    if (i < 0 || i >= 31 || j < 0 || j >= 25)
        return;
    combo++;
    all_static_balls[i][j].exist = 0;
    if (combo > 2)
    {
        iPlaySound("assets/sounds/pop.wav", false, 20);
        iSetColor(all_static_balls[i][j].red, all_static_balls[i][j].green, all_static_balls[i][j].blue);
        iFilledCircle(all_static_balls[i][j].x, all_static_balls[i][j].y, poppingsize * ball_radius);
        Sleep(sleeptime);
    }
    if (i != 0 && all_static_balls[i - 1][j].exist)
    {
        if (r == all_static_balls[i - 1][j].red && g == all_static_balls[i - 1][j].green && b == all_static_balls[i - 1][j].blue)
        {
            check_neighbour(i - 1, j);
        }
    }
    if (all_static_balls[i + 1][j].exist)
    {
        if (r == all_static_balls[i + 1][j].red && g == all_static_balls[i + 1][j].green && b == all_static_balls[i + 1][j].blue)
        {
            check_neighbour(i + 1, j);
        }
    }
    if (j != 0 && all_static_balls[i][j - 1].exist)
    {
        if (r == all_static_balls[i][j - 1].red && g == all_static_balls[i][j - 1].green && b == all_static_balls[i][j - 1].blue)
        {
            check_neighbour(i, j - 1);
        }
    }
    if (j != 24 && all_static_balls[i][j + 1].exist)
    {
        if (r == all_static_balls[i][j + 1].red && g == all_static_balls[i][j + 1].green && b == all_static_balls[i][j + 1].blue)
        {
            check_neighbour(i, j + 1);
        }
    }
    /*
   if (i!=0 && j!=0 && all_static_balls[i-1][j-1].exist){
       if (r==all_static_balls[i-1][j-1].red && g==all_static_balls[i-1][j-1].green && b==all_static_balls[i-1][j-1].blue){
       check_neighbour(i-1,j-1);
       }
   }
   if (i!=0 && j!=24 && all_static_balls[i-1][j+1].exist){
       if (r==all_static_balls[i-1][j+1].red && g==all_static_balls[i-1][j+1].green && b==all_static_balls[i-1][j+1].blue){
       check_neighbour(i-1,j+1);
       }
   }
   if (j!=0 && all_static_balls[i+1][j-1].exist){
       if (r==all_static_balls[i+1][j-1].red && g==all_static_balls[i+1][j-1].green && b==all_static_balls[i+1][j-1].blue){
       check_neighbour(i+1,j-1);
       }
   }
   if (j!=24 && all_static_balls[i+1][j+1].exist){
       if (r==all_static_balls[i+1][j+1].red && g==all_static_balls[i+1][j+1].green && b==all_static_balls[i+1][j+1].blue){
       check_neighbour(i+1,j+1);
       }
   }
   */
}

void check_collision(int i, int j)
{
    int startchecking = 0;

    if (all_static_balls[i][j].exist)
    {
        if (r == 255 && g == 255 && b == 255)
        {
            r = all_static_balls[i][j].red;
            g = all_static_balls[i][j].green;
            b = all_static_balls[i][j].blue;
        }
        if (!((r == 63 && g == 63 && b == 63) || (r == 127 && g == 127 && b == 127)))
        {
            if (all_static_balls[i + 1][j].exist == 0)
            {
                i = i + 1;
                j = j;
            }
            else if (dx > 0)
            {
                if (all_static_balls[i][j - 1].exist == 0)
                {
                    i = i;
                    j = j - 1;
                }
                else
                {
                    i = i + 1;
                    j = j - 1;
                }
            }
            else if (dx < 0)
            {
                if (all_static_balls[i][j + 1].exist == 0)
                {
                    i = i;
                    j = j + 1;
                }
                else
                {
                    i = i + 1;
                    j = j + 1;
                }
            }
        }
        startchecking = 1;
    }
    if (ball_y + ball_radius > height)
    {
        startchecking = 1;
        if ((r == 255 && g == 255 && b == 255) || (r == 127 && g == 127 && b == 127))
        {
            startchecking = 0;
            resetBall();
        }
    }

    // for metal ball
    if (r == 127 && g == 127 && b == 127 && startchecking)
    {
        startchecking = 0;
        all_static_balls[i][j].exist = 0;
        if ((j - 1) >= 0)
            all_static_balls[i][j - 1].exist = 0;
        if ((j + 1) < 25)
            all_static_balls[i][j + 1].exist = 0;
    }

    // for bomb
    if (r == 63 && g == 63 && b == 63 && startchecking && throw_ball)
    {
        startchecking = 0;
        for (int I = -2; I <= 2; I++)
        {
            for (int J = -2; J <= 2; J++)
            {
                if ((I + i) >= 0 && (I + i) < 30 && (J + j) >= 0 && (J + j) <= 24)
                    all_static_balls[i + I][j + J].exist = 0;
            }
        }
        resetBall();
    }

    // checking around i,j
    if (startchecking && throw_ball)
    {
        if (i != 0 && all_static_balls[i - 1][j].exist)
        {
            if (r == all_static_balls[i - 1][j].red && g == all_static_balls[i - 1][j].green && b == all_static_balls[i - 1][j].blue)
            {
                combo++;
                check_neighbour(i - 1, j);
                all_static_balls[i - 1][j].exist = 1;
            }
        }
        if (i != 29 && all_static_balls[i + 1][j].exist)
        {
            if (r == all_static_balls[i + 1][j].red && g == all_static_balls[i + 1][j].green && b == all_static_balls[i + 1][j].blue)
            {
                combo++;
                check_neighbour(i + 1, j);
                all_static_balls[i + 1][j].exist = 1;
            }
        }
        if (j != 0 && all_static_balls[i][j - 1].exist)
        {
            if (r == all_static_balls[i][j - 1].red && g == all_static_balls[i][j - 1].green && b == all_static_balls[i][j - 1].blue)
            {
                combo++;
                check_neighbour(i, j - 1);
                all_static_balls[i][j - 1].exist = 1;
            }
        }
        if (j != 24 && all_static_balls[i][j + 1].exist)
        {
            if (r == all_static_balls[i][j + 1].red && g == all_static_balls[i][j + 1].green && b == all_static_balls[i][j + 1].blue)
            {
                combo++;
                check_neighbour(i, j + 1);
                all_static_balls[i][j + 1].exist = 1;
            }
        }
        /*
       if (i!=0 && j!=0 && all_static_balls[i-1][j-1].exist){
           if (r==all_static_balls[i-1][j-1].red && g==all_static_balls[i-1][j-1].green && b==all_static_balls[i-1][j-1].blue){
           combo++;
           check_neighbour(i-1,j-1);
           all_static_balls[i-1][j-1].exist=1;
           }
       }
       if (i!=0 && j!=24 && all_static_balls[i-1][j+1].exist){
           if (r==all_static_balls[i-1][j+1].red && g==all_static_balls[i-1][j+1].green && b==all_static_balls[i-1][j+1].blue){
           combo++;
           check_neighbour(i-1,j+1);
           all_static_balls[i-1][j+1].exist=1;
           }
       }
       if (j!=0 && all_static_balls[i+1][j-1].exist){
           if (r==all_static_balls[i+1][j-1].red && g==all_static_balls[i+1][j-1].green && b==all_static_balls[i+1][j-1].blue){
           combo++;
           check_neighbour(i+1,j-1);
           all_static_balls[i+1][j-1].exist=1;
           }
       }
       if (j!=24 && all_static_balls[i+1][j+1].exist){
           if (r==all_static_balls[i+1][j+1].red && g==all_static_balls[i+1][j+1].green && b==all_static_balls[i+1][j+1].blue){
           combo++;
           check_neighbour(i+1,j+1);
           all_static_balls[i+1][j+1].exist=1;
           }
       }
       */

        if (combo <= 2)
        {
            all_static_balls[i][j].exist = 1;
            all_static_balls[i][j].red = r;
            all_static_balls[i][j].green = g;
            all_static_balls[i][j].blue = b;
        }
        else if (throw_ball)
        {
            if (all_static_balls[i - 1][j].exist)
            {
                if (r == all_static_balls[i - 1][j].red && g == all_static_balls[i - 1][j].green && b == all_static_balls[i - 1][j].blue)
                {
                    all_static_balls[i - 1][j].exist = 0;
                    iSetColor(all_static_balls[i - 1][j].red, all_static_balls[i - 1][j].green, all_static_balls[i - 1][j].blue);
                    iFilledCircle(all_static_balls[i - 1][j].x, all_static_balls[i - 1][j].y, poppingsize * ball_radius);
                    Sleep(sleeptime);
                }
            }
            if (all_static_balls[i + 1][j].exist)
            {
                if (r == all_static_balls[i + 1][j].red && g == all_static_balls[i + 1][j].green && b == all_static_balls[i + 1][j].blue)
                {
                    all_static_balls[i + 1][j].exist = 0;
                    iSetColor(all_static_balls[i + 1][j].red, all_static_balls[i + 1][j].green, all_static_balls[i + 1][j].blue);
                    iFilledCircle(all_static_balls[i + 1][j].x, all_static_balls[i + 1][j].y, poppingsize * ball_radius);
                    Sleep(sleeptime);
                }
            }
            if (j != 0 && all_static_balls[i][j - 1].exist)
            {
                if (r == all_static_balls[i][j - 1].red && g == all_static_balls[i][j - 1].green && b == all_static_balls[i][j - 1].blue)
                {
                    all_static_balls[i][j - 1].exist = 0;
                    iSetColor(all_static_balls[i][j - 1].red, all_static_balls[i][j - 1].green, all_static_balls[i][j - 1].blue);
                    iFilledCircle(all_static_balls[i][j - 1].x, all_static_balls[i][j - 1].y, poppingsize * ball_radius);
                    Sleep(sleeptime);
                }
            }
            if (j != 24 && all_static_balls[i][j + 1].exist)
            {
                if (r == all_static_balls[i][j + 1].red && g == all_static_balls[i][j + 1].green && b == all_static_balls[i][j + 1].blue)
                {
                    all_static_balls[i][j + 1].exist = 0;
                    iSetColor(all_static_balls[i][j + 1].red, all_static_balls[i][j + 1].green, all_static_balls[i][j + 1].blue);
                    iFilledCircle(all_static_balls[i][j + 1].x, all_static_balls[i][j + 1].y, poppingsize * ball_radius);
                    Sleep(sleeptime);
                }
            }
            /*
           if (i!=0 && j!=0 && all_static_balls[i-1][j-1].exist){
               if (r==all_static_balls[i-1][j-1].red && g==all_static_balls[i-1][j-1].green && b==all_static_balls[i-1][j-1].blue){
               all_static_balls[i-1][j-1].exist=0;
               }
           }
           if (i!=0 && j!=24 && all_static_balls[i-1][j+1].exist){
               if (r==all_static_balls[i-1][j+1].red && g==all_static_balls[i-1][j+1].green && b==all_static_balls[i-1][j+1].blue){
               all_static_balls[i-1][j+1].exist=0;
               }
           }
           if (j!=0 && all_static_balls[i+1][j-1].exist){
               if (r==all_static_balls[i+1][j-1].red && g==all_static_balls[i+1][j-1].green && b==all_static_balls[i+1][j-1].blue){
               all_static_balls[i+1][j-1].exist=0;
               }
           }
           if (j!=24 && all_static_balls[i+1][j+1].exist){
               if (r==all_static_balls[i+1][j+1].red && g==all_static_balls[i+1][j+1].green && b==all_static_balls[i+1][j+1].blue){
               all_static_balls[i+1][j+1].exist=0;
               }
           }
           */
        }
        resetBall();
    }
}

void drawBall()
{

    iSetColor(r, g, b);
    iFilledCircle(ball_x, ball_y, ball_radius);
    iSetColor(255, 255, 255);
    iFilledCircle(ball_x - 5, ball_y + 5, 2);

    // if(ball_x - ball_radius<0 || ball_x + ball_radius > width)
    if (ball_x < 0 || ball_x > width)
        dx = -dx;

    ball_x += dx;
    ball_y += dy;
    /*
    double steps = 4.0;
    for (int s = 0; s < steps; s++) {
        ball_x += dx / steps;
        ball_y += dy / steps;

        int j = ball_x / ball_diameter;
        int i = (height - ball_y) / ball_diameter;

        if (i >= 0 && i < 31 && j >= 0 && j < 25 && all_static_balls[i][j].exist) {
            check_collision(i, j);
            break; // Collision happened, stop moving
        }
    }
    */
}
//----------------------game over function --------------------------------------

void gameover()
{
    for (int j = 0; j < 25; j++)
    {
        if (all_static_balls[30][j].exist)
        {
            screenCount = 5;
            return;
        }
    }
}
void curv(int x, int y, int w, int h, int r)
{
    iFilledRectangle(x + r, y, w - 2 * r, h);
    iFilledRectangle(x, y + r, r, h - 2 * r);
    iFilledRectangle(x + w - r, y + r, r, h - 2 * r);
    iFilledCircle(x + r, y + r, r);
    iFilledCircle(x + w - r, y + r, r);
    iFilledCircle(x + r, y + h - r, r);
    iFilledCircle(x + w - r, y + h - r, r);
}
void curv_border(int x, int y, int w, int h, int r)
{
    iFilledRectangle(x + r - .3, y - .3, w - 2 * r + .6, h + .6);
    iFilledRectangle(x - .3, y + r - .3, r + .6, h - 2 * r + .6);
    iFilledRectangle(x + w - r - .3, y + r - .3, r + .6, h - 2 * r + .6);
    iFilledCircle(x + r, y + r, r + .3);
    iFilledCircle(x + w - r, y + r, r + .3);
    iFilledCircle(x + r, y + h - r, r + .3);
    iFilledCircle(x + w - r, y + h - r, r + .3);
}
void updateMusic()
{
    if (music_vol == 0)
    {
        if (menu_muse != -1)
            iPauseSound(menu_muse);
        if (game_muse != -1)
            iPauseSound(game_muse);
        ch = -1;
        return;
    }
    bool wantGameMusic = (screenCount == 1 && screen == 2);
    int desiredHandle = wantGameMusic ? game_muse : menu_muse;

    int volumeValue = (volume == 0) ? 20 : (volume == 2) ? 60
                                                         : 40;

    if (ch != -1 && ch != desiredHandle)
        iPauseSound(ch);
    if (wantGameMusic && menu_muse != -1)
        iPauseSound(menu_muse);
    if (!wantGameMusic && game_muse != -1)
        iPauseSound(game_muse);
    if (ch != desiredHandle || musicPaused)
    {
        ch = desiredHandle;
        iResumeSound(ch);
        iSetVolume(ch, 0);
        currentVolume = 0;
        targetVolume = volumeValue;
        fadingIn = true;
        musicPaused = false;
    }
}
void fillwithpattern(int pattern)
{
    if (pattern == 1)
        fillwithballs();
    else if (pattern == 2)
        fillwithdiamond();
    else if (pattern == 3)
        fillwithpyramid();
}
void saveGameState()
{
    const char *filename = (gameModeValue == 1) ? "save/time.txt" : "save/space.txt";
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    int elapsed = (int)(time(0) - gameStartTime - pauseDuration);
    fprintf(fp, "mode %d\n", gameModeValue);
    fprintf(fp, "submode %d\n", subModeSelect);
    fprintf(fp, "time %d\n", elapsed);
    fprintf(fp, "name %s\n", playerName);
    fprintf(fp, "combo %d\n", combo);
    fprintf(fp, "moves %d\n", moves);
    for(int i = 0; i < 31; ++i){
        for(int j = 0; j < 25; ++j){
            auto &b = all_static_balls[i][j];
            fprintf(fp, "%d %d %d %d\n",
                    b.exist,
                    b.red,
                    b.green,
                    b.blue);
        }
    }
    fclose(fp);
}
void loadGameState(const string &filepath)
{
    FILE *fp = fopen(filepath.c_str(), "r");
    if (!fp)
    {
        printf("Failed to open save file %s\n", filepath.c_str());
        return;
    }
    int savedTime;
    fscanf(fp, "mode %d\n", &gameModeValue);
    fscanf(fp, "submode %d\n", &subModeSelect);
    fscanf(fp, "time %d\n", &savedTime);
    fscanf(fp, "name %[^\n]", playerName);
    fscanf(fp, "combo %d\n", &combo);
    fscanf(fp, "moves %d\n", &moves);
    for(int i = 0; i < 31; ++i){
        for(int j = 0; j < 25; ++j){
            int exist, r, g, b;
            fscanf(fp, "%d %d %d %d\n", &exist, &r, &g, &b);
            all_static_balls[i][j].exist = exist;
            all_static_balls[i][j].red   = r;
            all_static_balls[i][j].green = g;
            all_static_balls[i][j].blue  = b;
        }
    }
set_coordinates();
    fclose(fp);
    screen = 2;
    screenCount = 1;
    fillwithpattern(subModeSelect);
    gameStartTime = time(0) - savedTime;
    pauseDuration = 0;
    gameOverState = false;
    timeRushActive = (gameModeValue == 1);
    timeRushRound = 0;
    printf("Loaded game from %s\n", filepath.c_str());
}
vector<string> saveFiles;
void listSaveFiles()
{
    saveFiles.clear();
    FILE *fp;
    char buf[256];
    fp = fopen("save/time.txt", "r");
    if (fp)
    {
        while (fgets(buf, sizeof(buf), fp))
        {
            buf[strcspn(buf, "\r\n")] = 0;
            if (buf[0])
                saveFiles.push_back(buf);
        }
        fclose(fp);
    }
    fp = fopen("save/space.txt", "r");
    if (fp)
    {
        while (fgets(buf, sizeof(buf), fp))
        {
            buf[strcspn(buf, "\r\n")] = 0;
            if (buf[0])
                saveFiles.push_back(buf);
        }
        fclose(fp);
    }
}

void loadHighScores(int mode)
{
    highScores.clear();
    ifstream file;

    if (mode == 1)
        file.open("save/timerush.txt");
    else
        file.open("save/spaceblast.txt");

    if (!file)
        return;

    while (!file.eof())
    {
        ScoreEntry s;
        if (mode == 1)
            file >> s.name >> s.combo >> s.time;
        else
        {
            file >> s.name >> s.time;
            s.combo = 0;
        }

        if (!file.fail())
            highScores.push_back(s);
    }
    file.close();

    if (mode == 1)
        sort(highScores.begin(), highScores.end(), [](auto &a, auto &b)
             { return a.combo > b.combo; });
    else
        sort(highScores.begin(), highScores.end(), [](auto &a, auto &b)
             { return a.time < b.time; });
}

Image bg_0, bg_3, bg_4, bg_7, bg_00;
void iDraw()
{
    // place your drawing codes here
    if(isTransitioning){
        iClear();
        for(auto &b:transitionBubbles){
        iSetColor(200,220,255);
        iFilledCircle(b.x,b.y,5);
        b.y += b.speed;
        }
        transitionBubbles.erase(
        std::remove_if(
            transitionBubbles.begin(),
            transitionBubbles.end(),
            [&](auto &b){ return b.y > height; }
        ),
        transitionBubbles.end()
        );
        if(transitionBubbles.empty()){
        isTransitioning = false;
        screenCount     = pendingScreenCount;
        screen          = pendingScreen;
        }
        return;
    }

    if (screenCount == 0)
    {
        if (music_vol == 1 && ch == -1)
            updateMusic();
        iClear();
        iSetColor(147, 213, 230);
        iFilledRectangle(0, 0, 500, 650);
        iShowLoadedImage(-50, -30, &newgame00);
        if (new_game == 0)
            iSetColor(42, 54, 53);
        else
        {
            iSetColor(224, 209, 197);
            curv_border(148, 412, 198, 48, 20);
            iSetColor(61, 46, 33);
            curv(149, 413, 194, 46, 20);
            iSetColor(224, 209, 197);
        }
        iTextBold(185, 428, "Play  Game", GLUT_BITMAP_TIMES_ROMAN_24);
        if (high_score == 0)
            iSetColor(42, 54, 53);
        else
        {
            iSetColor(224, 209, 197);
            curv_border(148, 342, 198, 48, 20);
            iSetColor(61, 46, 33);
            curv(149, 343, 194, 46, 20);
            iSetColor(224, 209, 197);
        }
        iTextBold(185, 358, "High-scores", GLUT_BITMAP_TIMES_ROMAN_24);
        if (settings == 0)
            iSetColor(42, 54, 53);
        else
        {
            iSetColor(224, 209, 197);
            curv_border(147, 273, 197, 48, 20);
            iSetColor(61, 46, 33);
            curv(148, 274, 193, 46, 20);
            iSetColor(224, 209, 197);
        }
        iTextBold(206, 290, "Settings", GLUT_BITMAP_TIMES_ROMAN_24);
        if (exit_game == 0)
            iSetColor(42, 54, 53);
        else
        {
            iSetColor(224, 209, 197);
            curv_border(148, 203, 198, 48, 20);
            iSetColor(61, 46, 33);
            curv(149, 204, 194, 46, 20);
            iSetColor(224, 209, 197);
        }
        iTextBold(220, 218, "Exit", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(232, 167, 160);
        iFilledCircle(483, 634, 15);
        if (help == 0)
            iSetColor(255, 0, 0);
        else
            iSetColor(194, 174, 174);
        iFilledCircle(483, 634, 13);
        iSetColor(0, 0, 0);
        iTextBold(478, 626, "?", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(232, 167, 160);
        curv_border(420, 10, 70, 20, 10);
        if (about_us == 0)
            iSetColor(24, 66, 35);
        else
            iSetColor(151, 184, 167);
        curv(420, 10, 70, 20, 10);
        if (about_us == 0)
            iSetColor(255, 255, 255);
        else
            iSetColor(22, 36, 29);
        iTextBold(429, 15, "About Us", GLUT_BITMAP_HELVETICA_12);
    }
    else if (screenCount == 1)
    {
        // Code for the second screen, new game screen
        iClear();
        if (screen == 1)
        {
            iShowLoadedImage(-110, -120, &sc1);
            in_menu = 1, in_game = 0;
            iSetColor(77, 34, 29);
            iFilledCircle(387, 420, 16);
            if (!cross)
            {
                iSetColor(255, 0, 0);
                iFilledCircle(387, 420, 15);
                iSetColor(255, 255, 255);
            }
            else
            {
                iSetColor(194, 170, 167);
                iFilledCircle(387, 420, 15);
                iSetColor(31, 16, 14);
            }
            iTextBold(381, 414, "X", GLUT_BITMAP_HELVETICA_18);
            iSetColor(139, 201, 198);
            curv_border(98, 232, 138, 36, 13);
            curv_border(241, 232, 138, 36, 13);
            if (back == 0)
                iSetColor(15, 31, 30);
            else
                iSetColor(140, 171, 169);
            curv(99, 233, 136, 34, 13);
            if (proceed == 0)
                iSetColor(15, 31, 30);
            else
                iSetColor(140, 171, 169);
            curv(242, 233, 136, 34, 13);
            if (back == 0)
                iSetColor(202, 224, 223);
            else
                iSetColor(10, 26, 24);
            iTextBold(118, 240, "NOVICE", GLUT_BITMAP_TIMES_ROMAN_24);
            if (proceed == 0)
                iSetColor(202, 224, 223);
            else
                iSetColor(10, 26, 24);
            iTextBold(257, 240, "VETERAN", GLUT_BITMAP_TIMES_ROMAN_24);
        }
        else if (screen == 2)
        {
            iClear();
            iShowLoadedImage(-105, -60, &bg_00);
            iSetTransparentColor(147, 213, 230, 0.2);
            iFilledRectangle(0, 0, 500, 650);
            iSetColor(49, 99, 88);
            iSetLineWidth(6);
            iRectangle(3, 3, width - 6, height - 6);
            iSetLineWidth(1);
            iSetColor(22, 74, 57);
            iFilledRectangle(0, 0, 236, 51);
            iFilledRectangle(236, 0, 36, 28);
            iFilledRectangle(267, 0, 233, 51);
            iSetColor(19, 38, 46);
            curv_border(4, 19, 75, 22, 8);
            curv_border(234.5, 4, 75, 22, 8);
            curv_border(421, 19, 75, 22, 8);
            if (combo_hove == 0 || option_screen != 0)
            {
                iSetColor(145, 209, 235);
                curv(5, 20, 73, 20, 7);
            }
            else if (combo_hove == 1 && option_screen == 0)
            {
                iSetColor(29, 54, 64);
                curv(5, 20, 73, 20, 7);
            }
            if (time_hove == 0 || option_screen != 0)
            {
                iSetColor(145, 209, 235);
                curv(235.5, 5, 73, 20, 7);
            }
            else if (time_hove == 1 && option_screen == 0)
            {
                iSetColor(29, 54, 64);
                curv(235.5, 5, 73, 20, 7);
            }
            if (option_hove == 0 || option_screen != 0)
            {
                iSetColor(145, 209, 235);
                curv(422, 20, 73, 20, 7);
                iSetColor(19, 38, 46);
            }
            else if (option_hove == 1 && option_screen == 0)
            {
                iSetColor(29, 54, 64);
                curv(422, 20, 73, 20, 7);
                iSetColor(160, 202, 219);
            }
            iTextBold(427, 25, "Options", GLUT_BITMAP_9_BY_15);
            in_menu = 0, in_game = 1;
            iSetColor(102, 148, 143);
            iFilledRectangle(236, 28, 70, 23);
            int j = ball_x / ball_diameter;
            int i = (height - ball_y) / ball_diameter;

            iSetLineWidth(3);
            drawAxis();
            draw_all_static_ball();
            int elapsed;
            if (timeRushActive)
            {
                elapsed = time(0) - gameStartTime - pauseDuration;
                int currentLimit = getTimeLimitForSubmode(subModeSelect);
                if (elapsed >= currentLimit)
                {
                    timeRushRound++;
                    if (timeRushRound >= 3)
                    {
                        timeRushActive = false;
                        screenCount = 5;
                    }
                    else
                    {
                        timeRushActive = false;
                        screenCount = 5;
                    }
                }
            }
            else if (gamePaused && pauseStartTime != 0)
            {
                elapsed = pauseStartTime - gameStartTime - pauseDuration;
            }
            else
            {
                elapsed = time(0) - gameStartTime - pauseDuration;
            }
            char timerStr[30];
            int remaining = timeRushActive ? (getTimeLimitForSubmode(subModeSelect) - elapsed) : elapsed;
            if (timeRushActive && remaining < 0)
                remaining = 0;
            sprintf(timerStr, "Time : %02d:%02d", remaining / 60, remaining % 60);
            if (time_hove == 0 || option_screen != 0)
                iSetColor(19, 38, 46);
            else if (time_hove == 1 && option_screen == 0)
                iSetColor(157, 190, 204);
            iTextBold(239, 11, timerStr, GLUT_BITMAP_HELVETICA_12);
            drawCannon();
            check_collision(i, j);
            bool anyLeft = false;
            for (int ii = 0; ii < 31 && !anyLeft; ++ii)
            {
                for (int jj = 0; jj < 25; ++jj)
                {
                    if (all_static_balls[ii][jj].exist)
                    {
                        anyLeft = true;
                        break;
                    }
                }
            }
            if (!anyLeft)
            {
                screen = 6;
                screenCount = 1;
                victoryType = 1;
            }
            gameover();
            if(r==127 && g==127 && b==127) velocity=8;
            else velocity=16;
            if (throw_ball)
                drawBall();

            if (!timeRushActive && gameModeValue == 2 && moves >= getMoveThresholdForSubmode(subModeSelect))
            {
                all_ball_down();
                moves = 0;
            }

            char cmb[12];
            sprintf(cmb, "COMBO: %i", (combo >= 2) ? combo : 0);
            if (combo_hove == 0 || option_screen != 0)
                iSetColor(19, 38, 46);
            else if (combo_hove == 1 && option_screen == 0)
                iSetColor(157, 190, 204);
            iTextBold(10, 25, cmb, GLUT_BITMAP_8_BY_13);
            if (option_screen == 1)
            {
                iSetTransparentColor(159, 181, 176, 0.25);
                iFilledRectangle(0, 0, 500, 650);
                iSetColor(61, 99, 90);
                curv_border(99, 214, 302, 286, 13);
                iSetColor(177, 224, 190);
                curv(100, 214, 300, 286, 13);
                iSetColor(61, 99, 90);
                iFilledCircle(398, 497, 17);
                if (!cross)
                {
                    iSetColor(255, 0, 0);
                    iFilledCircle(398, 497, 16);
                    iSetColor(255, 255, 255);
                }
                else
                {
                    iSetColor(214, 90, 90);
                    iFilledCircle(398, 497, 16);
                    iSetColor(46, 34, 34);
                }
                iTextBold(392, 491, "X", GLUT_BITMAP_HELVETICA_18);
                iSetColor(61, 99, 90);
                curv_border(150, 460, 200, 30, 13);
                if (resume == 0)
                {
                    iSetColor(14, 31, 18);
                    curv(151, 460.5, 198, 28.75, 13);
                    iSetColor(177, 224, 190);
                }
                else
                {
                    iSetColor(110, 128, 114);
                    curv(151, 460.5, 198, 28.75, 13);
                    iSetColor(18, 61, 30);
                }
                iTextBold(220, 470, "Resume", GLUT_BITMAP_HELVETICA_18);
                iSetColor(61, 99, 90);
                curv_border(150, 420, 200, 30, 13);
                if (restart == 0)
                {
                    iSetColor(14, 31, 18);
                    curv(151, 420.5, 198, 28.75, 13);
                    iSetColor(177, 224, 190);
                }
                else
                {
                    iSetColor(110, 128, 114);
                    curv(151, 420.5, 198, 28.75, 13);
                    iSetColor(18, 61, 30);
                }
                iTextBold(222, 430, "Restart", GLUT_BITMAP_HELVETICA_18);
                iSetColor(61, 99, 90);
                curv_border(150, 380, 200, 30, 13);
                if (back_men == 0)
                {
                    iSetColor(14, 31, 18);
                    curv(151, 380.5, 198, 28.75, 13);
                    iSetColor(177, 224, 190);
                }
                else
                {
                    iSetColor(110, 128, 114);
                    curv(151, 380.5, 198, 28.75, 13);
                    iSetColor(18, 61, 30);
                }
                iTextBold(228.3, 390, "Menu", GLUT_BITMAP_HELVETICA_18);
                iSetColor(61, 99, 90);
                curv_border(150, 340, 200, 30, 13);
                if (back_stat == 0)
                {
                    iSetColor(14, 31, 18);
                    curv(151, 340.5, 198, 28.75, 13);
                    iSetColor(177, 224, 190);
                }
                else
                {
                    iSetColor(110, 128, 114);
                    curv(151, 340.5, 198, 28.75, 13);
                    iSetColor(18, 61, 30);
                }
                iTextBold(200, 350, "Status Select", GLUT_BITMAP_HELVETICA_18);
                iSetColor(61, 99, 90);
                curv_border(150, 300, 200, 30, 13);
                if (back_choce == 0)
                {
                    iSetColor(14, 31, 18);
                    curv(151, 300.5, 198, 28.75, 13);
                    iSetColor(177, 224, 190);
                }
                else
                {
                    iSetColor(110, 128, 114);
                    curv(151, 300.5, 198, 28.75, 13);
                    iSetColor(18, 61, 30);
                }
                iTextBold(225, 310, "Modes", GLUT_BITMAP_HELVETICA_18);
                iSetColor(61, 99, 90);
                curv_border(150, 260, 200, 30, 13);
                if (instruc == 0)
                {
                    iSetColor(14, 31, 18);
                    curv(151, 260.5, 198, 28.75, 13);
                    iSetColor(177, 224, 190);
                }
                else
                {
                    iSetColor(110, 128, 114);
                    curv(151, 260.5, 198, 28.75, 13);
                    iSetColor(18, 61, 30);
                }
                iTextBold(204.5, 270, "Instructions", GLUT_BITMAP_HELVETICA_18);
                curv_border(150, 221, 200, 30, 13);
                if (sav == 0)
                {
                    iSetColor(14, 31, 18);
                    curv(151, 221.6, 198, 28.75, 13);
                    iSetColor(177, 224, 190);
                }
                else
                {
                    iSetColor(110, 128, 114);
                    curv(151, 221.6, 198, 28.75, 13);
                    iSetColor(18, 61, 30);
                }
                iTextBold(206, 229.5, "Save Game", GLUT_BITMAP_HELVETICA_18);
            }
            else if (option_screen == 2)
            {
                iSetTransparentColor(159, 181, 176, 0.25);
                iFilledRectangle(0, 0, 500, 650);
                iSetColor(61, 99, 90);
                curv_border(56, 191, 390, 332, 13);
                iSetColor(177, 224, 190);
                curv(57, 190, 388, 330, 13);
                iSetColor(61, 99, 90);
                iFilledCircle(446, 518, 16);
                if (!cross)
                {
                    iSetColor(255, 0, 0);
                    iFilledCircle(446, 518, 15);
                    iSetColor(255, 255, 255);
                }
                else
                {
                    iSetColor(214, 90, 90);
                    iFilledCircle(446, 518, 15);
                    iSetColor(46, 34, 34);
                }
                iTextBold(440, 513, "X", GLUT_BITMAP_HELVETICA_18);
                iSetColor(61, 99, 90);
                curv_border(187, 509, 146, 41, 13);
                iSetColor(190, 237, 237);
                curv(188, 509, 144, 39, 13);
                iSetColor(5, 31, 31);
                iTextBold(201, 521.5, "Instructions", GLUT_BITMAP_TIMES_ROMAN_24);
                iSetColor(61, 99, 90);
                curv_border(62, 196, 377, 295, 13);
                iSetColor(171, 201, 180);
                curv(63, 197, 375, 293, 13);
                iSetColor(6, 20, 20);
                iTextBold(70, 465, "1.Click on the screen or W key to throw the", GLUT_BITMAP_HELVETICA_18);
                iTextBold(70, 433, "   ball upwards.", GLUT_BITMAP_HELVETICA_18);
                iTextBold(70, 401, "2.Change cannon position with the mouse", GLUT_BITMAP_HELVETICA_18);
                iTextBold(70, 369, "   changing its position.", GLUT_BITMAP_HELVETICA_18);
                iTextBold(70, 337, "3.Complete 3 rounds in time to complete the", GLUT_BITMAP_HELVETICA_18);
                iTextBold(70, 305, "   full game.", GLUT_BITMAP_HELVETICA_18);
                iTextBold(70, 273, "4.Higher combos will bring power balls,", GLUT_BITMAP_HELVETICA_18);
                iTextBold(70, 241, "   which are the Blast ball & Any ball.", GLUT_BITMAP_HELVETICA_18);
            }
            else if (option_screen == 3)
            {
                iSetTransparentColor(0, 0, 0, 0.5);
                iFilledRectangle(0, 0, width, height);
                iSetColor(61, 99, 90);
                curv_border(139, 301, 226, 132, 13);
                iSetColor(177, 224, 190);
                curv(140, 302, 224, 130, 13);
                iSetColor(255, 255, 255);
                iFilledCircle(363, 432, 14);
                if (!cross)
                {
                    iSetColor(255, 0, 0);
                    iFilledCircle(363, 432, 12);
                    iSetColor(255, 255, 255);
                }
                else
                {
                    iSetColor(214, 90, 90);
                    iFilledCircle(363, 432, 12);
                    iSetColor(46, 34, 34);
                }
                iTextBold(358, 426, "X", GLUT_BITMAP_HELVETICA_18);
                iSetColor(4, 26, 23);
                iTextBold(173, 390, "Save your game?", GLUT_BITMAP_TIMES_ROMAN_24);
                curv_border(160, 320, 80, 30, 10);
                if (proceed == 0)
                {
                    iSetColor(77, 125, 106);
                    curv(161, 321, 78, 28, 10);
                    iSetColor(4, 11, 28);
                }
                else
                {
                    iSetColor(4, 11, 28);
                    curv(161, 321, 78, 28, 10);
                    iSetColor(77, 125, 106);
                }
                iTextBold(182, 329.6, "Yes", GLUT_BITMAP_HELVETICA_18);
                iSetColor(36, 8, 13);
                curv_border(260, 320, 80, 30, 10);
                if (back == 0)
                {
                    iSetColor(173, 36, 48);
                    curv(261, 321, 78, 28, 10);
                    iSetColor(247, 213, 216);
                }
                else
                {
                    iSetColor(247, 213, 216);
                    curv(261, 321, 78, 28, 10);
                    iSetColor(173, 36, 48);
                }
                iTextBold(288, 329.6, "No", GLUT_BITMAP_HELVETICA_18);
            }
        }
        else if (screen == 3)
        {
            iClear();
            in_menu = 1, in_game = 0;
            iShowLoadedImage(-110, -120, &sc3);
            iSetColor(77, 34, 29);
            curv_border(86, 212, 308, 210, 20);
            iSetColor(232, 226, 183);
            curv(87, 213, 306, 208, 20);
            in_menu = 1, in_game = 0;
            iSetColor(77, 34, 29);
            iFilledCircle(391, 421, 16);
            if (!cross)
            {
                iSetColor(255, 0, 0);
                iFilledCircle(391.5, 421.5, 15);
                iSetColor(255, 255, 255);
            }
            else
            {
                iSetColor(194, 170, 167);
                iFilledCircle(391.5, 421.5, 15);
                iSetColor(31, 16, 14);
            }
            iTextBold(385, 414, "X", GLUT_BITMAP_HELVETICA_18);
            iSetColor(87, 77, 63);
            curv_border(108, 229, 118, 40, 10);
            curv_border(253, 229, 132, 40, 10);
            if (back == 0)
                iSetColor(148, 37, 30);
            else
                iSetColor(207, 149, 140);
            curv(108, 230, 116, 38, 10);
            if (proceed == 0)
                iSetColor(89, 150, 132);
            else
                iSetColor(140, 219, 157);
            curv(254, 230, 128, 38, 10);
            if (back == 1)
                iSetColor(46, 24, 21);
            else
                iSetColor(201, 180, 163);
            iTextBold(137, 240, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
            if (proceed == 0)
                iSetColor(211, 237, 236);
            else
                iSetColor(24, 59, 57);
            iTextBold(263, 240, "CONFIRM", GLUT_BITMAP_TIMES_ROMAN_24);
            iSetColor(42, 54, 53);
            curv_border(108, 290, 265, 40, 10);
            iSetColor(197, 209, 208);
            curv(108.5, 290.3, 264, 38.6, 10);
            iSetColor(28, 20, 14);
            iTextBold(130, 383, "What would you like to", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(170, 353, "be known as?", GLUT_BITMAP_TIMES_ROMAN_24);
            iSetColor(44, 44, 84);
            int nameX = 127;
            int nameY = 304;
            iSetColor(0, 0, 0);
            iTextBold(nameX, nameY, playerName, GLUT_BITMAP_TIMES_ROMAN_24);
            static int pulse = 0;
            static bool increasing = true;
            if (increasing)
            {
                pulse += 3;
                if (pulse >= 255)
                    increasing = false;
            }
            else
            {
                pulse -= 3;
                if (pulse <= 120)
                    increasing = true;
            }
            if (showCursor)
            {
                int nameWidth = getTextWidth(playerName);
                iSetColor(pulse, pulse, pulse);
                iTextBold(nameX + nameWidth, nameY, "|", GLUT_BITMAP_TIMES_ROMAN_24);
            }
            if (showWarning)
            {
                iSetColor(200, 50, 50);
                iTextBold(123, 283, "Only A-Z, a-z, 0-9, and _ allowed!", GLUT_BITMAP_HELVETICA_12);
            }
        }
        else if (screen == 4)
        {
            iClear();
            iShowLoadedImage(-110, -120, &sc4);
            in_menu = 1, in_game = 0;
            iSetColor(219, 204, 147);
            curv_border(257, 202, 165, 58, 13);   // diamond
            curv_border(252.6, 124, 169, 60, 13); // random
            curv_border(70, 121, 160, 58, 13);    // pyramid
            curv_border(68.5, 201, 165, 56, 13);  // box
            if (subModeSelect == 2 || submode_hover == 2)
                iSetColor(219, 204, 147); // diamond
            else
                iSetColor(77, 34, 29);
            curv(259, 203.5, 161, 55, 13);
            if (subModeSelect == 1 || submode_hover == 1)
                iSetColor(219, 204, 147); // box
            else
                iSetColor(77, 34, 29);
            curv(69.5, 202, 163, 54, 13);
            if (subModeSelect == 3 || submode_hover == 3)
                iSetColor(219, 204, 147); // pyramid
            else
                iSetColor(77, 34, 29);
            curv(71.5, 122.5, 157, 55, 13);
            if (subModeSelect == 4 || submode_hover == 4)
                iSetColor(219, 204, 147); // random
            else
                iSetColor(77, 34, 29);
            curv(254, 125, 165, 56, 13);
            if (subModeSelect == 2 || submode_hover == 2)
                iSetColor(26, 38, 37);
            else
                iSetColor(212, 205, 186);
            iTextBold(293, 222, "Diamond", GLUT_BITMAP_TIMES_ROMAN_24);
            if (subModeSelect == 1 || submode_hover == 1)
                iSetColor(26, 38, 37);
            else
                iSetColor(212, 205, 186);
            iTextBold(125, 222, "Box", GLUT_BITMAP_TIMES_ROMAN_24);
            if (subModeSelect == 3 || submode_hover == 3)
                iSetColor(26, 38, 37);
            else
                iSetColor(212, 205, 186);
            iTextBold(108, 142, "Pyramid", GLUT_BITMAP_TIMES_ROMAN_24);
            if (subModeSelect == 4 || submode_hover == 4)
                iSetColor(26, 38, 37);
            else
                iSetColor(212, 205, 186);
            iTextBold(293, 142, "Random", GLUT_BITMAP_TIMES_ROMAN_24);
            iSetColor(180, 209, 207);
            curv_border(107, 410, 265, 47, 15);
            curv_border(107, 342, 265, 47, 15);
            if (mode_hover == 1 || gameModeValue == 1)
                iSetColor(132, 179, 175);
            else
                iSetColor(16, 28, 27);
            curv(108, 411, 263, 45, 15);
            if (mode_hover == 2 || gameModeValue == 2)
                iSetColor(132, 179, 175);
            else
                iSetColor(16, 28, 27);
            curv(108, 343, 263, 45, 15);
            if (mode_hover == 1 || gameModeValue == 1)
                iSetColor(16, 28, 27);
            else
                iSetColor(132, 179, 175);
            iTextBold(165, 423, "TIME RUSH", GLUT_BITMAP_TIMES_ROMAN_24);
            if (mode_hover == 2 || gameModeValue == 2)
                iSetColor(16, 28, 27);
            else
                iSetColor(132, 179, 175);
            iTextBold(160, 355, "SPACE BLAST", GLUT_BITMAP_TIMES_ROMAN_24);
            iSetColor(232, 226, 183);
            iFilledRectangle(244, 51, 77, 38);
            iFilledRectangle(158, 51, 77, 38);
            iFilledPolygon(arrow_go_x, arrow_go_y, 3);
            iFilledPolygon(arrow_back_x, arrow_back_y, 3);
            if (proceed == 0)
                iSetColor(77, 34, 29);
            else
                iSetColor(232, 226, 183);
            iFilledRectangle(245, 52, 75, 36);
            iFilledPolygon(arrow_go_x_1, arrow_go_y_1, 3);
            if (back == 0)
                iSetColor(77, 34, 29);
            else
                iSetColor(232, 226, 183);
            iFilledRectangle(159, 52, 75, 36);
            iFilledPolygon(arrow_back_x_1, arrow_back_y_1, 3);
            if (proceed == 0)
                iSetColor(232, 226, 183);
            else
                iSetColor(77, 34, 29);
            iTextBold(274, 63.3, "GO", GLUT_BITMAP_TIMES_ROMAN_24);
            if (back == 0)
                iSetColor(232, 226, 183);
            else
                iSetColor(77, 34, 29);
            iTextBold(156, 63.3, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
        }
        else if (screen == 5)
        {
            listSaveFiles(); // populates saveFiles vector
            iClear();
            iShowLoadedImage(-110, -120, &sc3);
            iSetTransparentColor(147, 213, 230, 0.2);
            iFilledRectangle(0, 0, 500, 650);
            iSetColor(55, 74, 66);
            curv_border(75, 150, 350, 400, 14);
            iSetColor(189, 215, 217);
            curv(75, 150, 350, 400, 14);
            iSetColor(255, 255, 255);
            iFilledCircle(422, 548, 16);
            if (!cross)
            {
                iSetColor(255, 0, 0);
                iFilledCircle(422, 548, 15);
                iSetColor(255, 255, 255);
            }
            else
            {
                iSetColor(176, 147, 144);
                iFilledCircle(422, 548, 15);
                iSetColor(28, 10, 6);
            }
            iTextBold(416, 542.25, "X", GLUT_BITMAP_HELVETICA_18);
            iSetColor(55, 74, 66);
            curv_border(193, 540, 128, 41, 14);
            iSetColor(183, 232, 212);
            curv(194, 540, 126, 39, 14);
            iSetColor(9, 28, 20);
            iTextBold(200, 550.8, "Load Game", GLUT_BITMAP_TIMES_ROMAN_24);
            const int colX[] = {90, 180, 260, 335};
            iSetColor(9, 20, 20);
            const char *titles[] = {"Mode", "Name", "Time", "Date"};
            for (int c = 0; c < 4; ++c)
                iTextBold(colX[c], 500, titles[c], GLUT_BITMAP_HELVETICA_18);
            if (saveFiles.empty())
            {
                iSetColor(12, 26, 19);
                iTextBold(200, 280, "No save files", GLUT_BITMAP_HELVETICA_18);
                return;
            }
            for (int i = 0; i < (int)saveFiles.size(); ++i)
            {
                int y = 490-i*30;
                if (i == hoveredSaveIdx)
                {
                    iSetColor(15, 36, 28);
                    curv_border(84.2, y-30-0.8, 336.6, 32.6,6);
                    iSetColor(200, 230, 200);
                    curv(85, y-30, 335, 31,6);
                }
                istringstream iss(saveFiles[i]);
                string f[4];
                iss >> f[0] >> f[1] >> f[2] >> f[3];
                iSetColor(12, 26, 19);
                const int colX[] = {90, 180, 260, 335};
                for (int c = 0; c < 4; ++c)
                    iTextBold(colX[c], y-20, f[c].c_str());
            }
        }
        else if (screen == 6)
        {
            iClear();
            iSetTransparentColor(0, 0, 0, 0.15);
            if (victoryType == 1)
            {
                iTextBold(250, 400, "Congratulations! You cleared this level.", GLUT_BITMAP_TIMES_ROMAN_24);
                iTextBold(240, 360, "Do you want to proceed to the next round?", GLUT_BITMAP_HELVETICA_18);
                iRectangle(260, 300, 200, 40); // Proceed
                iTextBold(300, 315, "Proceed");
                iRectangle(260, 240, 200, 40); // Menu
                iTextBold(300, 255, "Go to Menu");
            }
            else if (victoryType == 2)
            {
                iTextBold(250, 400, "Congratulations! You completed this mode!", GLUT_BITMAP_TIMES_ROMAN_24);
                iRectangle(260, 300, 200, 40); // Try again
                iTextBold(290, 315, "Try Game Again");
                iRectangle(260, 240, 200, 40); // Menu
                iTextBold(300, 255, "Go to Menu");
            }
        }
    }
    else if (screenCount == 2)
    {
        // Code for the third screen, high scores screen
        iClear();
        iShowLoadedImage(-50, -30, &bg_7);
        iSetTransparentColor(147, 213, 230, 0.2);
        iFilledRectangle(0, 0, 500, 650);
        listSaveFiles();
        iSetColor(55, 74, 66);
        curv_border(74, 149, 352, 402, 14);
        iSetColor(154, 204, 237);
        curv(75, 150, 350, 400, 14);
        iSetColor(255, 255, 255);
        iFilledCircle(421, 548, 17);
        if (!cross)
        {
            iSetColor(255, 0, 0);
            iFilledCircle(421, 548, 15);
            iSetColor(255, 255, 255);
        }
        else
        {
            iSetColor(176, 147, 144);
            iFilledCircle(421, 548, 15);
            iSetColor(28, 10, 6);
        }
        iTextBold(415, 542.25, "X", GLUT_BITMAP_HELVETICA_18);
        iSetColor(9, 20, 28);
        curv_border(185, 529.4, 140, 40.6, 14);
        iSetColor(172, 227, 222);
        curv(186, 531, 138, 38, 14);
        iSetColor(6, 28, 26);
        iTextBold(196, 540, "High Scores", GLUT_BITMAP_TIMES_ROMAN_24);
        if (score_screen == 0)
        {
            iSetColor(71, 122, 102);
            curv_border(197, 359, 110, 42, 14);
            if (rush == 0)
            {
                iSetColor(12, 25, 31);
                curv(198, 360, 108, 40, 14);
                iSetColor(186, 216, 230);
            }
            else
            {
                iSetColor(186, 216, 230);
                curv(198, 360, 108, 40, 14);
                iSetColor(12, 25, 31);
            }
            iTextBold(207, 374, "Time Rush", GLUT_BITMAP_HELVETICA_18);
            iSetColor(71, 122, 102);
            curv_border(197, 299, 110, 42, 14);
            if (blast == 0)
            {
                iSetColor(12, 25, 31);
                curv(198, 300, 108, 40, 14);
                iSetColor(186, 216, 230);
            }
            else
            {
                iSetColor(186, 216, 230);
                curv(198, 300, 108, 40, 14);
                iSetColor(12, 25, 31);
            }
            iTextBold(204, 314, "Space Blast", GLUT_BITMAP_HELVETICA_18);
        }
        else if (score_screen == 1)
        {
            int y = 430;
            iSetColor(9, 20, 28);
            curv_border(80, 155, 340, 310, 14);
            iSetColor(167, 201, 189);
            curv(81, 156, 338, 308, 14);
            if (highScores.empty())
            {
                iSetColor(12, 26, 19);
                iTextBold(200, 390, "No high scores available", GLUT_BITMAP_HELVETICA_18);
                return;
            }
            iSetColor(12, 26, 19);
            iTextBold(100, 473, "Name - Combo - Time", GLUT_BITMAP_HELVETICA_18);
            for (auto &s : highScores)
            {
                char line[100];
                sprintf(line, "%s - Combo: %d, Time: %ds", s.name.c_str(), s.combo, s.time);
                iTextBold(100, y, line);
                y -= 30;
            }
        }
        else if (score_screen == 2)
        {
            int y = 500;
            iSetColor(9, 20, 28);
            curv_border(80, 155, 340, 310, 14);
            iSetColor(167, 201, 189);
            curv(81, 156, 338, 308, 14);
            if (highScores.empty())
            {
                iSetColor(12, 26, 19);
                iTextBold(200, 390, "No high scores available", GLUT_BITMAP_HELVETICA_18);
                return;
            }
            iSetColor(12, 26, 19);
            iTextBold(100, 473, "Name - Time", GLUT_BITMAP_TIMES_ROMAN_24);
            for (auto &s : highScores)
            {
                char line[100];
                sprintf(line, "%s - Time: %ds", s.name.c_str(), s.time);
                iTextBold(100, y, line);
                y -= 30;
            }
        }
    }
    else if (screenCount == 3)
    {
        // Code for the fourth screen,  settings screen
        iClear();
        iShowLoadedImage(-50, -30, &bg_3);
        iSetColor(77, 34, 29);
        iFilledCircle(406, 435, 16);
        if (!cross)
        {
            iSetColor(255, 0, 0);
            iFilledCircle(406, 435, 15);
            iSetColor(255, 255, 255);
        }
        else
        {
            iSetColor(176, 147, 144);
            iFilledCircle(406, 435, 15);
            iSetColor(255, 255, 255);
        }
        iTextBold(400, 429, "X", GLUT_BITMAP_HELVETICA_18);
        iSetColor(55, 74, 66);
        curv_border(232, 357, 166, 40, 14);
        iSetColor(154, 204, 237);
        curv(232, 357, 166, 40, 14);
        if (volume == 1)
        {
            iSetColor(42, 54, 53);
            iTextBold(239, 370, "Low", GLUT_BITMAP_HELVETICA_18);
            iSetColor(255, 255, 255);
            curv_border(288, 356, 55, 40, 13);
            iSetColor(42, 54, 53);
            curv(288, 356, 55, 40, 13);
            iSetColor(220, 247, 235);
            iTextBold(298, 370, "Mid", GLUT_BITMAP_HELVETICA_18);
            iSetColor(42, 54, 53);
            iTextBold(352, 370, "High", GLUT_BITMAP_HELVETICA_18);
        }
        else if (volume == 0)
        {
            iSetColor(220, 247, 235);
            curv_border(232, 356, 55, 40, 13);
            iSetColor(42, 54, 53);
            curv(232, 356, 55, 40, 13);
            iSetColor(203, 242, 234);
            iTextBold(242, 370, "Low", GLUT_BITMAP_HELVETICA_18);
            iSetColor(42, 54, 53);
            iTextBold(298, 370, "Mid", GLUT_BITMAP_HELVETICA_18);
            iSetColor(42, 54, 53);
            iTextBold(352, 370, "High", GLUT_BITMAP_HELVETICA_18);
        }
        else if (volume == 2)
        {
            iSetColor(42, 54, 53);
            iTextBold(239, 370, "Low", GLUT_BITMAP_HELVETICA_18);
            iSetColor(42, 54, 53);
            iTextBold(298, 370, "Mid", GLUT_BITMAP_HELVETICA_18);
            iSetColor(220, 247, 235);
            curv_border(344, 356, 55, 40, 13);
            iSetColor(42, 54, 53);
            curv(344, 356, 55, 40, 13);
            iSetColor(203, 242, 234);
            iTextBold(355, 370, "High", GLUT_BITMAP_HELVETICA_18);
        }
        iSetColor(55, 74, 66);
        curv_border(232, 285, 168, 40, 13);
        iSetColor(154, 204, 237);
        curv(232, 285, 168, 40, 13);

        if (music_vol == 0)
        {
            iSetColor(42, 54, 53);
            iTextBold(260, 299, "On", GLUT_BITMAP_HELVETICA_18);
            iSetColor(220, 247, 235);
            curv_border(314, 285, 84, 40, 13); //
            iSetColor(42, 54, 53);
            curv(314, 285, 84, 40, 13); //
            iSetColor(203, 242, 234);
            iTextBold(339, 299, "Off", GLUT_BITMAP_HELVETICA_18);
            // PlaySound(0,0,0);
        }
        else if (music_vol == 1)
        {
            iSetColor(220, 247, 235);
            curv_border(232, 285, 84, 40, 13);
            iSetColor(42, 54, 53);
            curv(232, 285, 84, 40, 13);
            iSetColor(203, 242, 234);
            iTextBold(260, 299, "On", GLUT_BITMAP_HELVETICA_18);
            iSetColor(42, 54, 53);
            iTextBold(339, 299, "Off", GLUT_BITMAP_HELVETICA_18);
        }
    }
    else if (screenCount == 4)
    {
        // Code for the fifth screen,  exit confirmation screen
        iClear();
        iShowLoadedImage(-50, -30, &bg_4);
        iSetColor(147, 213, 230);
        iFilledCircle(404, 400, 19);
        if (!cross)
        {
            iSetColor(255, 0, 0);
            iFilledCircle(404, 400, 17.5);
            iSetColor(255, 255, 255);
        }
        else
        {
            iSetColor(176, 147, 144);
            iFilledCircle(404, 400, 17.5);
            iSetColor(31, 16, 14);
        }
        iTextBold(398, 395.2, "X", GLUT_BITMAP_HELVETICA_18);
    }
    else if (screenCount == 5)
    {
        // Code for the sixth screen,  game over screen
        iClear();
        iShowLoadedImage(-110, -100, &bg_00);
        iSetColor(255, 255, 255);
        iTextBold(180, 500, "Game Over", GLUT_BITMAP_TIMES_ROMAN_24);
        if (!gameOverState)
        {
            saveGameState();
            gameOverState = true;
        }
        iSetColor(150, 0, 0);
        curv(120, 400, 260, 50, 15);
        iSetColor(255, 255, 255);
        iTextBold(170, 415, "Retry This Set", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(0, 100, 0);
        curv(120, 320, 260, 50, 15);
        iSetColor(255, 255, 255);
        iTextBold(150, 335, "Try Different Set", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(0, 0, 150);
        curv(120, 240, 260, 50, 15);
        iSetColor(255, 255, 255);
        iTextBold(150, 255, "Back To Menu", GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else if (screenCount == 7)
    {
        // Code for the eighth screen,  help screen
        iClear();
        iShowLoadedImage(-50, -30, &bg_7);
        iSetTransparentColor(147, 213, 230, 0.5);
        iFilledRectangle(0, 0, 500, 650);
        iSetColor(204, 163, 131);
        curv_border(30, 205, 433, 281, 10);
        iSetColor(168, 108, 77);
        curv(30, 205, 433, 281, 10);
        iSetColor(77, 34, 29);
        iFilledCircle(460, 484, 16);
        if (!cross)
        {
            iSetColor(255, 0, 0);
            iFilledCircle(460, 484, 15);
            iSetColor(255, 255, 255);
        }
        else
        {
            iSetColor(176, 147, 144);
            iFilledCircle(460, 484, 15);
            iSetColor(31, 16, 14);
        }
        iTextBold(453.8, 478, "X", GLUT_BITMAP_HELVETICA_18);
        iSetColor(157, 177, 224);
        curv_border(41, 219.5, 412, 201, 10);
        iSetColor(23, 35, 64);
        curv(41, 219.5, 412, 201, 10);
        iSetColor(157, 177, 224);
        iFilledEllipse(235, 449, 100, 20);
        iSetColor(23, 35, 64);
        iFilledEllipse(234, 450, 100, 20);
        iSetColor(255, 255, 255);
        iTextBold(170, 442, "How To Play", GLUT_BITMAP_TIMES_ROMAN_24);
        iTextBold(50, 399.3, "1. Click on 'New Game' to start a new game.", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 369.3, "2. Click on 'Load Game' to load a saved game.", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 339.3, "3. Click on 'Settings' to change game settings.", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 309.3, "4. Click on 'Exit' to close the game.", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 279.3, "5. Click on '?' for help or instructions.", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 249.3, "6. Click on 'About Us' to know about the", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 223.9, "    game developers.", GLUT_BITMAP_HELVETICA_18);
    }
    else if (screenCount == 8)
    {
        iClear();
        iShowLoadedImage(-50, -30, &bg_7);
        iSetTransparentColor(147, 213, 230, 0.5);
        iFilledRectangle(0, 0, 500, 650);
        iSetColor(32, 59, 97);
        curv_border(30, 185, 433, 321, 10);
        iSetColor(139, 190, 232);
        curv(30, 185, 433, 321, 10);
        iSetColor(77, 34, 29);
        iFilledCircle(460, 504, 16);
        if (!cross)
        {
            iSetColor(255, 0, 0);
            iFilledCircle(460, 504, 15);
            iSetColor(255, 255, 255);
        }
        else
        {
            iSetColor(176, 147, 144);
            iFilledCircle(460, 504, 15);
            iSetColor(31, 16, 14);
        }
        iTextBold(453.8, 498, "X", GLUT_BITMAP_HELVETICA_18);
        iSetColor(245, 207, 169);
        curv_border(182, 490, 130, 40, 10);
        iSetColor(64, 43, 32);
        curv(182, 490, 130, 40, 10);
        iSetColor(230, 207, 195);
        iTextBold(200, 500, "About Us", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(6, 15, 20);
        iTextBold(50, 450, "Version 1.0.0", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 420, "Developed by:", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 380, "1. Siam Rafsan Prionto (BUET CSE-24)", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 350, "2. Kazi Fahin Abraz (BUET CSE-24)", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 310, "This was completed under the supervision of ", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 280, "Abdur Rafi sir, immense thanks to him.", GLUT_BITMAP_HELVETICA_18);
        iTextBold(50, 240, "Thank you for trying it out!", GLUT_BITMAP_HELVETICA_18);
        iSetColor(255, 255, 255);
        iCircle(53, 206, 6);
        iSetColor(6, 15, 20);
        iTextBold(50, 203, "c", GLUT_BITMAP_HELVETICA_12);
        iTextBold(62, 200, "Copyright to the owners", GLUT_BITMAP_HELVETICA_18);
    }
    else if (screenCount == 9)
    {
    }
    if (shouldStartMuse)
    {
        updateMusic();
        shouldStartMuse = false;
    }
    if (fadingIn)
    {
        currentVolume += 2;
        if (currentVolume >= targetVolume)
        {
            currentVolume = targetVolume;
            fadingIn = false;
        }
        if (ch != -1)
            iSetVolume(ch, currentVolume);
    }
    if (fadingOut)
    {
        currentVolume -= 2;
        if (currentVolume <= 0)
        {
            currentVolume = 0;
            fadingOut = false;
            if (ch != -1)
                iPauseSound(ch);
        }
        if (ch != -1)
            iSetVolume(ch, currentVolume);
    }
    static time_t lastHoverSound = 0;
    if (hovered && difftime(time(0), lastHoverSound) >= 0.2)
    {
        iPlaySound("assets/sounds/click.wav", false, 4);
        lastHoverSound = time(0);
        hovered = false;
    }
}

/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{

    // place your codes here
    combo_hove = option_hove = time_hove = 0;
    cross = back = proceed = 0;
    if (screenCount == 1 && screen == 2 && option_screen == 0)
    {
        mouse_x = mx - 250;
        mouse_y = my - 50;
        if (mouse_y <= 5)
            mouse_y = 5;
        mouse_r = sqrt(mouse_x * mouse_x + mouse_y * mouse_y);
        if (mx >= 5 && mx <= 78 && my >= 20 && my <= 40)
        {
            combo_hove = 1;
            hovered = true;
        }
        else
            combo_hove = 0;
        if (mx >= 235 && mx <= 308 && my >= 5 && my <= 25)
        {
            time_hove = 1;
            hovered = true;
        }
        else
            time_hove = 0;
        if (mx >= 422 && mx <= 495 && my >= 20 && my <= 40)
        {
            option_hove = 1;
            hovered = true;
        }
        else
            option_hove = 0;
    }
    else if (screenCount == 1)
    {
        if (screen == 1)
        {
            if ((mx - 387) * (mx - 387) + (my - 420) * (my - 420) <= 256)
            {
                cross = 1;
                hovered = true;
            }
            else
                cross = 0;
            if (mx >= 98 && mx <= 236 && my >= 232 && my <= 268)
            {
                back = 1;
                hovered = true;
            }
            else
                back = 0;
            if (mx >= 254 && mx <= 392 && my >= 232 && my <= 268)
            {
                proceed = 1;
                hovered = true;
            }
            else
                proceed = 0;
        }
        else if (screen == 4)
        {
            if (mx >= 264 && mx <= 433 && my >= 207 && my <= 255)
            {
                submode_hover = 2;
            }
            else if (mx >= 68 && mx <= 234 && my >= 207 && my <= 253)
            {
                submode_hover = 1;
            }
            else if (mx >= 71 && mx <= 229 && my >= 127 && my <= 175)
            {
                submode_hover = 3;
            }
            else if (mx >= 254 && mx <= 410 && my >= 127 && my <= 173)
            {
                submode_hover = 4;
            }
            else
                submode_hover = 0;
            if (mx >= 110 && mx <= 369 && my >= 413 && my <= 454)
            {
                mode_hover = 1;
            }
            else if (mx >= 110 && mx <= 369 && my >= 345 && my <= 386)
            {
                mode_hover = 2;
            }
            else
                mode_hover = 0;
            if (mx >= 245 && mx <= 355 && my >= 52 && my <= 82)
            {
                proceed = 1;
                hovered = true;
            }
            else
                proceed = 0;
            if (mx >= 122 && mx <= 232 && my >= 52 && my <= 82)
            {
                back = 1;
                hovered = true;
            }
            else
                back = 0;
        }
        else if (screen == 3)
        {
            if ((mx - 387) * (mx - 387) + (my - 420) * (my - 420) <= 256)
            {
                cross = 1;
                hovered = true;
            }
            else
                cross = 0;
            if (mx >= 254 && mx <= 392 && my >= 230 && my <= 268)
            {
                proceed = 1;
                hovered = true;
            }
            else
                proceed = 0;
            if (mx >= 108 && mx <= 226 && my >= 230 && my <= 268)
            {
                back = 1;
                hovered = true;
            }
            else
                back = 0;
        }
        else if (screen == 2)
        {
            if (option_screen == 1)
            {
                if (mx >= 151 && mx <= 349 && my >= 461 && my <= 490)
                {
                    resume = 1;
                    hovered = true;
                }
                else
                    resume = 0;
                if (mx >= 151 && mx <= 349 && my >= 421 && my <= 450)
                {
                    restart = 1;
                    hovered = true;
                }
                else
                    restart = 0;
                if (mx >= 151 && mx <= 349 && my >= 381 && my <= 410)
                {
                    back_men = 1;
                    hovered = true;
                }
                else
                    back_men = 0;
                if (mx >= 151 && mx <= 349 && my >= 341 && my <= 370)
                {
                    back_stat = 1;
                    hovered = true;
                }
                else
                    back_stat = 0;
                if (mx >= 151 && mx <= 349 && my >= 301 && my <= 330)
                {
                    back_choce = 1;
                    hovered = true;
                }
                else
                    back_choce = 0;
                if (mx >= 151 && mx <= 349 && my >= 261 && my <= 290)
                {
                    instruc = 1;
                    hovered = true;
                }
                else
                    instruc = 0;
                if ((mx - 398) * (mx - 398) + (my - 497) * (my - 497) <= 256)
                {
                    cross = 1;
                    hovered = true;
                }
                else
                    cross = 0;
                if ((mx >= 151 && mx <= 349) && (my >= 221 && my <= 250))
                {
                    sav = 1;
                    hovered = true;
                }
                else
                    sav = 0;
            }
            else if (option_screen == 2)
            {
                if ((mx - 446) * (mx - 446) + (my - 518) * (my - 518) <= 225)
                {
                    cross = 1;
                    hovered = true;
                }
                else
                    cross = 0;
            }
            else if (option_screen == 3)
            {
                if (mx >= 161 && mx <= 239 && my >= 321 && my <= 350)
                {
                    proceed = 1;
                    hovered = true;
                }
                else
                    proceed = 0;
                if (mx >= 261 && mx <= 339 && my >= 321 && my <= 350)
                {
                    back = 1;
                    hovered = true;
                }
                else
                    back = 0;
                if ((mx - 364) * (mx - 364) + (my - 432) * (my - 432) <= 225)
                {
                    cross = 1;
                    hovered = true;
                }
                else
                    cross = 0;
            }
        }
        else if (screen == 5)
        {
            hoveredSaveIdx = -1;
            const int startY = 490;
            const int rowH = 30;
            const int leftX = 90;
            const int rightX = 420; // tweak to cover full row width
            for (int i = 0; i < (int)saveFiles.size(); ++i)
            {
                int rowTop = startY - i * rowH;
                int rowBot = rowTop - rowH;
                if (mx >= leftX && mx <= rightX && my >= rowBot && my <= rowTop)
                {
                    hoveredSaveIdx = i;
                    break;
                }
            }
            if ((mx - 422) * (mx - 422) + (my - 548) * (my - 548) <= 289)
            {
                cross = 1;
                hovered = true;
            }
            else
                cross = 0;
        }
    }
    else if (screenCount == 0)
    {
        if (mx >= 420 && mx <= 490 && my >= 10 && my <= 30)
        {
            about_us = 1;
            hovered = true;
        }
        else
        {
            about_us = 0;
        }
        if ((mx - 483) * (mx - 483) + (my - 634) * (my - 634) <= 225)
        {
            help = 1;
            hovered = true;
        }
        else
        {
            help = 0;
        }
        if (mx >= 153 && mx <= 339 && my >= 202 && my <= 248)
        {
            exit_game = 1;
            hovered = true;
        }
        else
            exit_game = 0;
        if (mx >= 153 && mx <= 339 && my >= 414 && my <= 460)
        {
            new_game = 1;
            hovered = true;
        }
        else
            new_game = 0;
        if (mx >= 153 && mx <= 339 && my >= 345 && my <= 391)
        {
            high_score = 1;
            hovered = true;
        }
        else
            high_score = 0;
        if (mx >= 155 && mx <= 333 && my >= 274 && my <= 324)
        {
            settings = 1;
            hovered = true;
        }
        else
            settings = 0;
    }
    else if (screenCount == 8)
    {
        if ((mx - 460) * (mx - 460) + (my - 504) * (my - 504) <= 225)
        {
            cross = 1;
            hovered = true;
        }
        else
            cross = 0;
    }
    else if (screenCount == 7)
    {
        if ((mx - 460) * (mx - 460) + (my - 484) * (my - 484) <= 225)
        {
            cross = 1;
            hovered = true;
        }
        else
            cross = 0;
    }
    else if (screenCount == 3)
    {
        if ((mx - 406) * (mx - 406) + (my - 435) * (my - 435) <= 225)
        {
            cross = 1;
            hovered = true;
        }
        else
            cross = 0;
    }
    else if (screenCount == 4)
    {
        if ((mx - 404) * (mx - 404) + (my - 400) * (my - 400) <= 289)
        {
            cross = 1;
            hovered = true;
        }
        else
            cross = 0;
    }
    else if (screenCount == 2)
    {
        if ((mx - 421) * (mx - 421) + (my - 548) * (my - 548) <= 289)
        {
            cross = 1;
            hovered = true;
        }
        else
            cross = 0;
        if (option_screen == 0)
        {
            if (mx >= 198 && mx <= 308 && my >= 360 && my <= 400)
            {
                rush = 1;
                hovered = true;
            }
            else
                rush = 0;
            if (mx >= 198 && mx <= 308 && my >= 300 && my <= 340)
            {
                blast = 1;
                hovered = true;
            }
            else
                blast = 0;
        }
    }
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
        cout << mx << my << screenCount << endl;
        iPlaySound("assets/sounds/click.wav", false, 10);
        if (screenCount == 0)
        {
            if ((mx - 483) * (mx - 483) + (my - 634) * (my - 634) <= 225)
            {
                screenCount = 7;
                proceed = 0;
            }
            else if (mx >= 155 && mx <= 333 && my >= 417 && my <= 457)
            {
                startScreenTransition(1,1);
                in_menu     = 1;
                in_game     = 0;
                updateMusic();
                proceed = 0;
            }
            else if (mx >= 155 && mx <= 333 && my >= 346 && my <= 391)
            {
                screenCount = 2;
                proceed = 0;
            }
            else if (mx >= 155 && mx <= 333 && my >= 274 && my <= 318)
            {
                screenCount = 3;
                proceed = 0;
            }
            else if (mx >= 155 && mx <= 333 && my >= 203 && my <= 251)
            {
                screenCount = 4;
                proceed = 0;
            }
            else if (mx >= 420 && mx <= 490 && my >= 10 && my <= 30)
            {
                screenCount = 8;
                proceed = 0;
            }
        }
        else if (screenCount == 1)
        {
            // Code for play game screen
            if (screen == 1)
            {
                in_game = 1, in_menu = 0;
                if ((mx - 387) * (mx - 387) + (my - 420) * (my - 420) <= 256)
                {
                    screenCount = 0;
                    in_game = 0, in_menu = 1;
                    proceed = 0;
                    updateMusic();
                }
                else if (mx >= 99 && mx <= 235 && my >= 236 && my <= 265)
                {
                    screen = 3;
                    updateMusic();
                    playerName[0] = '\0';
                    nameLength = 0;
                    name_taken = 0;
                    proceed = 0;
                }
                else if (mx >= 254 && mx <= 392 && my >= 236 && my <= 265)
                { // VETERAN
                    screen = 5;
                    updateMusic();
                    playerName[0] = '\0';
                    nameLength = 0;
                    name_taken = 0;
                    proceed = 0;
                }
            }
            else if (screen == 3)
            {
                if (mx >= 108 && mx <= 126 && my >= 230 && my <= 268)
                {
                    screen = 1;
                    showWarning = false;
                    proceed = 0;
                    updateMusic();
                    return;
                }
                else if (mx >= 254 && mx <= 392 && my >= 230 && my <= 268)
                {
                    if (playerName[0] == '\0')
                    {
                        showWarning = true;
                        return;
                    }
                    else
                    {
                        showWarning = false;
                        screen = 4;
                        shouldStartMuse = true;
                        proceed = 0;
                        updateMusic();
                    }
                }
                else if ((mx - 389) * (mx - 389) + (my - 423) * (my - 423) <= 256)
                {
                    screenCount = 1, screen = 1;
                    proceed = 0;
                    in_game = 0, in_menu = 1;
                    shouldStartMuse = true;
                    updateMusic();
                }
            }
            else if (screen == 4)
            {
                if (mx >= 264 && mx <= 433 && my >= 207 && my <= 255)
                {
                    subModeSelect = 2;
                }
                else if (mx >= 68 && mx <= 233 && my >= 207 && my <= 255)
                {
                    subModeSelect = 1; // Box
                }
                else if (mx >= 68 && mx <= 233 && my >= 124 && my <= 169)
                {
                    subModeSelect = 3;
                }
                else if (mx >= 264 && mx <= 433 && my >= 124 && my <= 169)
                {
                    subModeSelect = 4;
                }
                else if (mx >= 123 && mx <= 226 && my >= 62 && my <= 90)
                {
                    screen = 3;
                    proceed = 0;
                    updateMusic();
                }
                else if (mx >= 110 && mx <= 369 && my >= 413 && my <= 454)
                {
                    gameModeValue = 1; // Time Rush
                }
                else if (mx >= 110 && mx <= 369 && my >= 345 && my <= 386)
                {
                    gameModeValue = 2; // Space Blast
                }
                else if (mx >= 170 && mx <= 367 && my >= 62 && my <= 90)
                {
                    if (gameModeValue != 0 && subModeSelect == 0)
                    {
                        showSubModeWarning = true;
                        return;
                    }
                    else if (gameModeValue == 0 && subModeSelect != 0)
                    {
                        showModeWarning = true;
                        return;
                    }
                    else if (gameModeValue == 0 && subModeSelect == 0)
                    {
                        showSubModeWarning = true;
                        showModeWarning = true;
                        return;
                    }
                    else
                    {
                        showSubModeWarning = false;
                        showModeWarning = false;

                        if (gameModeValue == 1)
                        {
                            timeRushActive = true;
                            timeRushRound = subModeSelect - 1;
                            fillwithpattern(subModeSelect);
                        }
                        else
                        {
                            if (subModeSelect == 1)
                                fillwithballs();
                            else if (subModeSelect == 2)
                                fillwithdiamond();
                            else if (subModeSelect == 3)
                                fillwithpyramid();
                            else if (subModeSelect == 4)
                                fillwithrandom();
                        }
                        screen = 2;
                        shouldStartMuse = true;
                        updateMusic();
                        gameStartTime = time(0);
                    }
                }
            }
            else if (screen == 2)
            {
                if (option_screen == 0)
                {
                    if (my > 50 && my <= 650)
                    {
                        if (throw_ball == 0)
                            combo = 0;
                        setBall();
                    }
                    else if (mx >= 423 && mx <= 496 && my >= 20 && my <= 41)
                    {
                        option_screen = 1; // option e click er porer screen er jnno
                        pauseStartTime = time(0);
                        gamePaused = true;
                    }
                }
                else if (option_screen == 1)
                {
                    if (mx >= 151 && mx <= 349)
                    {
                        if (my >= 461 && my <= 490)
                        { // Resume
                            if (pauseStartTime != 0)
                            {
                                pauseDuration += (time(0) - pauseStartTime);
                                pauseStartTime = 0;
                            }
                            gamePaused = false;
                            option_screen = 0;
                        }
                        else if (my >= 421 && my <= 450)
                        { // Restart
                            fillwithpattern(subModeSelect);
                            gameStartTime = time(0);
                            option_screen = 0;
                            pauseDuration = 0;
                            gamePaused = false;
                        }
                        else if (my >= 381 && my <= 410)
                        { // Back to Menu
                            screenCount = 0;
                            option_screen = 0;
                            gamePaused = false;
                            pauseDuration = 0;
                            updateMusic();
                            strcpy(playerName, "\0");
                            showCursor = true;
                            name_taken = false;
                            savePrompt = 1;
                            saveAction = 1;
                        }
                        else if (my >= 341 && my <= 370)
                        { // Back to Status
                            screenCount = 1, screen = 1;
                            option_screen = 0;
                            updateMusic();
                            gamePaused = false;
                            pauseDuration = 0;
                            updateMusic();
                            strcpy(playerName, "\0");
                            showCursor = true;
                            name_taken = false;
                            savePrompt = 1;
                            saveAction = 2;
                        }
                        else if (my >= 301 && my <= 330)
                        { // Back to Choices
                            screenCount = 1, screen = 4;
                            option_screen = 0;
                            gamePaused = false;
                            if (gameModeValue == 1)
                            {
                                timeRushActive = false;
                                timeRushRound = -1;
                                subModeSelect = 0; // Reset subModeSelect for Time Rush
                            }
                            else
                            {
                                subModeSelect = 0; // Reset subModeSelect for Space Blast
                            }
                            updateMusic();
                            savePrompt = 1;
                            saveAction = 1;
                        }
                        else if (my >= 261 && my <= 291)
                        { // Settings
                            screenCount = 1, screen = 2;
                            option_screen = 2;
                        }
                        else if (my >= 220 && my <= 250)
                        { // Save Game button
                            option_screen = 3;
                            savePrompt = 1;
                            saveAction = 1;
                            pauseStartTime = time(0);
                            gamePaused = true;
                        }
                    }
                    else if ((mx - 398) * (mx - 398) + (my - 497) * (my - 497) <= 256)
                    {
                        screenCount = 1, screen = 2;
                        cross = 1;
                        hovered = true;
                        option_screen = 0;
                        shouldStartMuse = true;
                        updateMusic();
                        if (pauseStartTime != 0)
                        {
                            pauseDuration += (time(0) - pauseStartTime);
                            pauseStartTime = 0;
                        }
                        gamePaused = false;
                    }
                }
                else if (option_screen == 2)
                {
                    if ((mx - 440) * (mx - 440) + (my - 513) * (my - 513) <= 225)
                    {
                        screenCount = 1, screen = 2;
                        option_screen = 1;
                    }
                }
                else if (option_screen == 3)
                {
                    if (mx >= 160 && mx <= 240 && my >= 320 && my <= 350)
                    {
                        saveGameState();
                        option_screen = 1;
                    }
                    else if (mx >= 260 && mx <= 340 && my >= 320 && my <= 350)
                    {
                        option_screen = 1;
                    }
                    else if ((mx - 364) * (mx - 364) + (my - 432) * (my - 432) <= 225)
                    {
                        option_screen = 1;
                    }
                }
            }
            else if (screen == 5)
            {
                if ((mx - 422) * (mx - 422) + (my - 548) * (my - 548) <= 289)
                {
                    screenCount = 1, screen = 1;
                    updateMusic();
                }
                else if (hoveredSaveIdx >= 0)
                {
                    if (saveFiles[hoveredSaveIdx].find("TimeRush") != string::npos)
                        loadGameState("save/time.txt");
                    else if (saveFiles[hoveredSaveIdx].find("SpaceBlast") != string::npos)
                        loadGameState("save/space.txt");
                    screen = 2;
                    option_screen = 0;
                    hoveredSaveIdx = -1;
                }
            }
            else if (screen == 6)
            {
                if (victoryType == 1 && mx >= 260 && mx <= 460 && my >= 300 && my <= 340)
                {
                    subModeSelect = (subModeSelect + 1) % 4;
                    if (subModeSelect == 0)
                        subModeSelect = 1;
                    fillwithpattern(subModeSelect);
                    gameStartTime = time(0);
                    pauseDuration = 0;
                    option_screen = 0;
                    gamePaused = false;
                    timeRushActive = true;
                    screen = 2;
                    screenCount = 1;
                }
                else if (victoryType == 2 && mx >= 260 && mx <= 460 && my >= 300 && my <= 340)
                {
                    screen = 4;
                    screenCount = 1;
                }
                else if (mx >= 260 && mx <= 460 && my >= 240 && my <= 280)
                {
                    screen = 0;
                    screenCount = 1;
                }
            }
        }
        else if (screenCount == 2)
        {
            // Code for high-score screen
            if ((mx - 421) * (mx - 421) + (my - 548) * (my - 548) <= 289)
            {
                screenCount = 0;
            }
            if (score_screen == 0)
            {
                if (mx >= 220 && mx <= 380 && my >= 360 && my < 400)
                {
                    // Try different set
                    score_screen = 1;
                    loadHighScores(1);
                }
                else if (mx > 220 && mx < 380 && my >= 280 && my < 320)
                {
                    // Back to menu
                    score_screen = 2;
                    loadHighScores(2);
                }
            }
        }
        else if (screenCount == 5)
        {
            // Code for game over screen
            if (mx >= 120 && mx <= 380 && my >= 400 && my <= 450)
            {
                if (subModeSelect == 1)
                    fillwithballs();
                else if (subModeSelect == 2)
                    fillwithdiamond();
                else if (subModeSelect == 3)
                    fillwithpyramid();
                else if (subModeSelect == 4)
                    fillwithrandom();
                screenCount = 1;
                screen = 2;
                shouldStartMuse = true;
                updateMusic();
                gameStartTime = time(0);
            }
            else if (mx >= 120 && mx <= 380 && my >= 320 && my <= 370)
            {
                // Try different set
                screen = 4;
                screenCount = 1;
            }
            else if (mx >= 120 && mx <= 380 && my >= 240 && my <= 290)
            {
                screen = 1;
                shouldStartMuse = true;
                timeRushActive = false;
                screenCount = 0;
                updateMusic();
            }
        }
        else if (screenCount == 6)
        {
            // Code for victory screen
        }
        else if (screenCount == 4)
        {
            if ((mx - 404) * (mx - 404) + (my - 400) * (my - 400) <= 289)
            {
                screenCount = 0;
            }
            else if (mx >= 103 && mx <= 245 && my >= 206 && my <= 242)
            {
                exit(0);
            }
            else if (mx >= 257 && mx <= 391 && my >= 204 && my <= 242)
            {
                screenCount = 0;
            }
        }
        else if (screenCount == 7)
        {
            if ((mx - 460) * (mx - 460) + (my - 484) * (my - 484) <= 225)
            {
                screenCount = 0;
            }
        }
        else if (screenCount == 3)
        {
            if ((mx - 406) * (mx - 406) + (my - 435) * (my - 435) <= 225)
            {
                screenCount = 0;
            }
            else if (mx >= 236 && mx <= 312 && my >= 290 && my <= 325)
            {
                music_vol = 1;
                musicPaused = true;
                shouldStartMuse = true;
            }

            else if (mx >= 313 && mx <= 400 && my >= 288 && my <= 331)
            {
                music_vol = 0;
                if (ch != -1)
                    iPauseSound(ch);
            }
            else if (mx >= 232 && mx <= 287 && my >= 356 && my <= 396)
            {
                volume = 0;
                if (music_vol == 1 && ch != -1)
                    iSetVolume(ch, 20);
            }
            else if (mx >= 288 && mx <= 343 && my >= 356 && my <= 396)
            {
                volume = 1;
                if (music_vol == 1 && ch != -1)
                    iSetVolume(ch, 40);
            }
            else if (mx >= 346 && mx <= 401 && my >= 356 && my <= 396)
            {
                volume = 2;
                if (music_vol == 1 && ch != -1)
                    iSetVolume(ch, 70);
            }
        }

        else if (screenCount == 8)
        {
            if ((mx - 460) * (mx - 460) + (my - 504) * (my - 504) <= 225)
            {
                screenCount = 0;
            }
        }

        proceed = 0;
        back = 0;
        cross = 0;
        new_game = 0;
        high_score = 0;
        settings = 0;
        exit_game = 0;
        help = 0;
        about_us = 0;
        mode_hover = 0;
        submode_hover = 0;
        resume = 0, restart = 0, back_men = 0, back_stat = 0, back_choce = 0, instruc = 0, sav = 0;
        updateMusic();
    }
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir =1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a keyin keyboard.
key- holds the ASCII value of the keypressed.
*/
void iKeyboard(unsigned char key)
{
    if (screenCount == 1)
    {
        if (screen == 2)
        {
            switch (key)
            {
            case 'q':
                // do something with 'q'
                screen = 4;
                updateMusic();
                iDraw();
                break;
            case 'a':
                if (angle > -80)
                    angle -= 2.5;
                break;
            case 'd':
                if (angle < 80)
                    angle += 2.5;
                break;
            case 'w':
                if (throw_ball == 0)
                    combo = 0;
                setBall();
                break;
            case 'f':
                all_ball_down();
                break;
            default:
                break;
            }
        }
        else if (screen == 3)
        {
            if (key == '\b')
            {
                if (nameLength > 0)
                {
                    playerName[--nameLength] = '\0';
                }
            }
            else if ((key == '\r' || key == '\n') && subModeSelect > 0 && nameLength > 0)
            { // Enter key
                iPlaySound("assets/sounds/click.wav", false, 20);
                printf("Name confirmed: %s\n", playerName);
                name_taken = true;
                screen = 2;
                showWarning = false;
                shouldStartMuse = true;
                updateMusic();
            }
            else if ((key >= 'a' && key <= 'z') ||
                     (key >= 'A' && key <= 'Z') ||
                     (key >= '0' && key <= '9') ||
                     key == '_')
            {
                iPlaySound("assets/sounds/music_game.wav", false, 40);
                if (nameLength < 49)
                {
                    playerName[nameLength++] = key;
                    playerName[nameLength] = '\0';
                }
                showWarning = false;
            }
            else
            {
                showWarning = true;
            }
        }
    }
    if (key == 27)
    { // ESC key
        if (screenCount == 1 && screen == 2)
        { // Only from game screen
            if (option_screen == 0)
            {
                option_screen = 1; // open options
                gamePaused = true;
                pauseStartTime = time(0);
            }
            else
            {
                option_screen = 0; // close options
                pauseDuration += time(0) - pauseStartTime;
                gamePaused = false;
            }
        }
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
    if (screenCount == 1 && screen == 2)
    {
        if (key == GLUT_KEY_LEFT && angle > -80)
            angle -= 2.5;
        else if (key == GLUT_KEY_RIGHT && angle < 80)
            angle += 2.5;
        else if (key == GLUT_KEY_UP && throw_ball == 0)
        {
            combo = 0;
            setBall();
        }
    }

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
    iInitializeSound();
    menu_muse = iPlaySound("assets/sounds/music_menu.wav", true, 0);
    ch = menu_muse;
    iSetVolume(menu_muse, 0);
    fadingIn = true;
    targetVolume = 40;
    game_muse = iPlaySound("assets/sounds/music_game.wav", true, 0);
    iPauseSound(game_muse);
    // iPauseSound(loadGameMus);
    iLoadImage(&sc3, "assets/images/screen3.jpeg");
    iLoadImage(&sc4, "assets/images/screen4.jpeg");
    iLoadImage(&sc1, "assets/images/new_game_1.jpeg");
    iLoadImage(&newgame00, "assets/images/bg_0.png");
    iLoadImage(&bg_3, "assets/images/bg_3.png");
    iLoadImage(&bg_4, "assets/images/bg_4.png");
    iLoadImage(&bg_7, "assets/images/bg_7.png");
    iLoadImage(&bg_00, "assets/images/bg00.jpg");
    iInitialize(width, height, "Bouncy Bonanza");
    iSetTimer(400, toggleCursor);
    return 0;
}
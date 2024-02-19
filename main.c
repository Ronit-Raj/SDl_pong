#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

void init();
void drawBg();
void drawPlayers();
void drawBall();
void checkCollisions();

SDL_Renderer* gameRenderer;
SDL_Window* window;
SDL_Texture* bg_texture;
SDL_Texture* platform;
SDL_Texture* ball_texture;
SDL_Rect ball;
SDL_Rect player1,player2;  //player1 is on top and player2 is at the bottom 
const int platformH=30;
const int platformW=139;
const int ballH=30;
const int ballW=30;
const int platformVel=10;
int ballVX;
int ballVY;
int ballStatus; //1 if the ball is with player 1 same for 2 , 0 if none of the player is in control of the ball

int main(int argc , char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    window=SDL_CreateWindow("Pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,650,SDL_WINDOW_ALLOW_HIGHDPI);
    if(window==NULL)
        printf("error in creating window %s",SDL_GetError());
    init();
    while(1){
        SDL_Event e;
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)
            break;
        else if(e.type==SDL_KEYDOWN){
            char* keyState=SDL_GetKeyboardState(NULL);
            if(keyState[SDL_SCANCODE_LEFT]){
                if(player2.x!=0)
                    (player2.x)-=platformVel;
            }
            if(keyState[SDL_SCANCODE_RIGHT]){
                if(player2.x<460)
                    (player2.x)+=platformVel;
            }
            if(keyState[SDL_SCANCODE_A]){
                if(player1.x!=0)
                    (player1.x)-=platformVel;
            }
            if(keyState[SDL_SCANCODE_D]){
                if(player1.x<460)
                    (player1.x)+=platformVel;
            }
            if(keyState[SDL_SCANCODE_SPACE]){
                if(ballStatus==1 || ballStatus==2)
                    ballStatus=0;
            }
        }
        SDL_RenderClear(gameRenderer);
        drawBg();
        drawPlayers();
        checkCollisions();
        drawBall();
        SDL_RenderPresent(gameRenderer);
        SDL_Delay(16); //capping fps to 60 
    }
    printf("%s",SDL_GetError());
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyTexture(platform);
    SDL_DestroyTexture(ball_texture);
    SDL_DestroyTexture(bg_texture);
    SDL_DestroyWindow(window);
    window=NULL;
    SDL_Quit();
}
void checkCollisions(){
    int ballExtremeEnd=ball.x+ballW;
    if(ball.x+ballW>=600){  //collision with right wall 
        ballVX=-3;
    }
    if(ball.x<=0){  //collison with left wall 
        ballVX=3 ;
    }

    if(ball.y-ballH<=player1.y){
        if((ball.x>=player1.x && ball.x<=player1.x+platformW) ||
            (ballExtremeEnd>=player1.x && ballExtremeEnd<=player1.x+platformW)){
            ballVY=5;
        }
    }
    if(ball.y+ballH>=player2.y){
        if((ball.x>=player2.x && ball.x<=player2.x+platformW) ||
            (ballExtremeEnd>=player2.x && ballExtremeEnd<=player2.x+platformW)){
            ballVY=-5;
        }
    }

    if(ball.y<=0){
        ballStatus=2;
    }
    else if(ball.y+ballH>=600)
        ballStatus=1;
}
void drawBall(){
    if(ballStatus==1){
        ball.x=player1.x+(platformW)/2-ballW/2;
        ball.y=player1.y+platformH-1;
    }
    else if(ballStatus==2){
        ball.x=player2.x+(platformW)/2-ballW/2;
        ball.y=600-platformH-ballH+1;
    }
    else{
        ball.x+=ballVX;
        ball.y+=ballVY;
    }
    SDL_RenderCopy(gameRenderer,ball_texture,NULL,&ball);
}
void drawBg(){
    if(SDL_RenderCopy(gameRenderer,bg_texture,NULL,NULL))
        printf("error in rendering background: %s\n",SDL_GetError());
}
void drawPlayers(){
    SDL_RenderCopy(gameRenderer,platform,NULL,&player1);
    SDL_RenderCopy(gameRenderer,platform,NULL,&player2);
}
void init(){
    gameRenderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_PRESENTVSYNC);
    if(gameRenderer==NULL)
        printf("error in creating game renderer: %s\n",SDL_GetError());
    bg_texture=IMG_LoadTexture(gameRenderer,"bg.png");
    platform=IMG_LoadTexture(gameRenderer,"player.png");
    ball_texture=IMG_LoadTexture(gameRenderer,"ball.png");
    SDL_Rect gameArea;
    gameArea.x=0;
    gameArea.y=50;
    gameArea.h=600;
    gameArea.w=600;
    SDL_RenderSetViewport(gameRenderer,&gameArea);

    player1.x=160;  
    player1.y=0;
    player1.w=platformW;
    player1.h=platformH;

    player2.x=100;
    player2.y=570;
    player2.w=platformW;
    player2.h=platformH;

    ball.h=ballH;
    ball.w=ballW;
    drawBg();
    drawPlayers();
    ballStatus=1;
    ballVX=3;
    ballVY=5;
    SDL_RenderPresent(gameRenderer);
    
}
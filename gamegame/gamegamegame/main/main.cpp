#include "Game.h"
#include "defs.h"
#undef main


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);// tạo ra 1 cửa sổ
//    g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);


    Game game("Flappy bird", WIDTH, HEIGHT); // đặt tên cho cửa sổ ,chiều rộng ,cao

    game.Start(); // game chạy 

    return 0;
}

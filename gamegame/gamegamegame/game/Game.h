#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"

#include <list>
#include <chrono>
#include <random>
#include <time.h>
#include <string>
#include <chrono>
#include <thread>

#include "Bird.h"
#include "Pipe.h"
#include "defs.h"
#include "Menu.h"

static Mix_Chunk* g_sound_background;



class Game
{
    public:
        SDL_Texture *tex_background; //texture cho nền của trò chơi.
        SDL_Texture *tex_ground;  //  texture cho đất của trò chơi.
        SDL_Texture *tex_playerUp; //texture cho chim bay lên của trò chơi.
        SDL_Texture *tex_playerMid;//texture cho chim cân bằng của trò chơi.
        SDL_Texture *tex_playerDown;//texture cho chim bay xuống của trò chơi.
        SDL_Texture *tex_pipe; //texture cho ống của trò chơi.
        SDL_Texture *tex_numbers[10]; // đếm điểm số trò chơi
        SDL_Texture *tex_gameover; //  texture cho thông báo game over trong trò chơi.
        SDL_Texture *tex_hp;   //texture cho thông báo máu trong trò chơi.
    
        SDL_Texture *tex_background1; //texture backgrond1
        SDL_Texture *tex_background2; //texture backgrond2
        SDL_Texture *tex_background3; //texture backgrond3
        SDL_Texture *tex_background4; //texture backgrond4
    
        SDL_Texture *tex_winner; // texture chien thang
        
        bool isRunning; // kiem tra chuong trinh co chay khong

        Game(const char *title, int _width, int _height); // khởi tại tiêu đề và kích thước cho trước
        
        void Start(); // Bắt đầu trò chơi
        void Close(); // đóng trò chơi
        void update(bool jump,bool moveLeft,bool moveRight, float elapsedTime, bool &gameover, Mix_Chunk* g_sound_background);
        void render(); // hiển thị đối tượng lên màn hình

        void gameOver(); // Hàm xử lý khi trò chơi kết thúc.
        void init(); // Hàm khởi tạo các giá trị cho các thuộc tính của lớp Game, như tải các texture và âm
        void StopGame(); // Khai báo hàm StopGame
        SDL_Texture* GetTexture() const { return m_texture; }

    protected:
        SDL_Texture* texture;
        SDL_Rect src, dst; // lưu texture trong bộ nhớ và màn hình

        Game(SDL_Texture* texture, int x, int y, int w, int h);


    private:
        SDL_Renderer *renderer ; // để vẽ
        SDL_Window *window; // hiển thị cửa sổ trên màn hình
        SDL_Texture* m_texture; // thêm biến m_texture ở đây

        SDL_Event event; // kiem tra sự kiện

        Bird *bird; // đại diện cho chú chim người di chuyển
        
        Menu* menu; // menu

        std::list<Pipe*> pipes; //đại diện cho đường ống mà chim phải bay qua.
    
        int ground1, ground2; // lưu các lớp đất
    
 
        bool gameStarted, gameover;  // biểu thị trò chơi đã bắt đầu , kết thúc

        void loadTextures(); // tải các texture được sử dụng trong game

};

#endif // GAME_H


#ifndef BIRD_H
#define BIRD_H

#include <SDL2/SDL.h>
#include <iostream>
#include "SDL2_mixer/SDl_mixer.h"

#include "Pipe.h"
#include "defs.h"
#include <vector>


class Bird
{
    private:
        float velocity; // tốc độ di chuyển của chim
        float acceleration; // gia tốc của chim
        float m_x; // toạ độ x của chim
        float m_y; // toạ độ y của chim
        SDL_Rect pos; //vị trí của chim trên màn hình, được đại diện bởi một đối tượng SDL_Rect
        static Bird* s_Instance; //con trỏ tĩnh (static) để duy trì một thể hiện duy nhất của lớp Bird thông qua toàn bộ chương trình.
        //các texture của chim để render lên màn hình.
        SDL_Texture *up; //đại diện cho texture của chim khi nó bay lên trên.
        SDL_Texture *down; //đại diện cho texture của chim khi nó rơi xuống.
        SDL_Texture *mid; //đại diện cho texture của chim khi nó đang trong trạng thái chuyển động giữa việc bay lên và rơi xuống.
        SDL_Texture *currentRenderingTexture; //đại diện cho texture của chim hiện tại đang được sử dụng để render lên màn hình.
        SDL_Renderer *renderer; // để vẽ lên màn hình

        int animationFrames; // tốc độ của khung hình
    
        bool isOnFire; // kiểm tra xem chim chạm lửa hay chưa
        
    public:
        int score; // điểm của trò chơi
        int hp=3; // mạng của chim

        Bird(SDL_Texture *up, SDL_Texture *mid, SDL_Texture *down, SDL_Renderer *renderer); // khởi tạo các thông tin của hàm BIRD
        SDL_Rect dst; // Vị trí và kích thước của chim


        void init(); // khởi tạo vị trí, tốc độ, gia tốc, điểm số và texture của chim.
        void render(); // vẽ chim lên màn hình.

        float getX() const { return m_x; } // trả về toạ độ x của chim
        float getY() const { return m_y; }
        void moveLeft(){
               pos.x -= 25; // di chuyển sang trái 10 pixel
               pos.y += 15;
       }
       void moveRight()
       {
           pos.x += 25; // di chuyển sang phải 10 pixel
           pos.y += 15;
       }

        void update(bool jump, bool moveLeft, bool moveRight, float elapsedTime); // cập nhật vị trí và tốc độ của chim dựa trên thời gian trôi qua và trạng thái "jump".
        void animation(); // hoạt hình trò chơi
        bool collisionDetector(Pipe *pipe); // kiểm tra va chạm chim với ống
        
        Bird();
        ~Bird(); // Hàm huỷ
        static Bird* GetInstance(); //trả về con trỏ đến thể hiện duy nhất của lớp Bird
    
        void setOnFire(bool onFire) { isOnFire = onFire; } // hàm để đặt trạng thái cháy cho chim
        bool getOnFire() const { return isOnFire; } // hàm để lấy trạng thái cháy của chim
        
};
#endif

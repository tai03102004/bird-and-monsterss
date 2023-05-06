#ifndef Menu_h
#define Menu_h

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <string>

class Menu {
public:
    Menu(SDL_Renderer* renderer, TTF_Font* font); // hàm vẽ , load font
    ~Menu();
    int show(); // hàm in ra menu và thực hiện khi ấn nút space
    bool isVisible(); // Thêm phương thức này để kiểm tra menu có đang hiển thị hay không

private:
    SDL_Renderer* renderer; // hàm vẽ
    TTF_Font* font; // font
    SDL_Texture* menu_texture; // hình menu
    SDL_Rect play_rect; // toạ độ nút play
    SDL_Rect exit_rect; // toạ độ nút exit
    bool visible; // Thêm biến boolean này để kiểm tra menu có đang hiển thị hay không
};

#endif /* Menu_h */

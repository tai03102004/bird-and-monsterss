#include "Menu.h"

Menu::Menu(SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font) {
    
    // Tải hình ảnh menu
    menu_texture = IMG_LoadTexture(renderer, "menu.png");

}

Menu::~Menu() {

    SDL_DestroyTexture(menu_texture);
}

int Menu::show() {
    SDL_Event event; // sự lý xự kiện
    visible = true;

    while (visible) {
        while (SDL_PollEvent(&event)) { // kiểm tra sự kiện
            if (event.type == SDL_QUIT) { //
                visible = false;
                return -1;
            }  else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                visible = false;
            }
        }

        // Vẽ menu lên màn hình
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menu_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    return -1;
}

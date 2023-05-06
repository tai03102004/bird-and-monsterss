#ifndef PIPE_H_
#define PIPE_H_

#include <SDL2/SDL.h>
#include "defs.h"

struct Pipe
{
    SDL_Rect top_src, top_dst;
    SDL_Rect bottom_dst, bottom_src;


    bool passed;
    float angle = 0.0f;
    // Phương thức xử lý khi Pipe di chuyển
    void Move(float delta_time, float speed) {
        top_dst.x -= speed * delta_time;
        bottom_dst.x -= speed * delta_time;
    }

    // Phương thức xử lý khi Pipe được tạo mới
    void GenerateNewPipe(int gap_size, int screen_width, int screen_height) {
        int min_pipe_height = 50;
        int max_pipe_height = screen_height - min_pipe_height - gap_size;

        int random_height = rand() % max_pipe_height + min_pipe_height;

        top_dst.x = screen_width;
        top_dst.y = random_height - top_dst.h;
        bottom_dst.x = screen_width;
        bottom_dst.y = random_height + gap_size;

        passed = false;
    }

    // Phương thức xử lý khi Pipe vượt qua Bird
    void SetPassed(bool value) {
        passed = value;
    }

    // Phương thức xử lý khi Pipe xoay góc
    void Rotate(float angle) {
        this->angle = angle;
    }
    Pipe() {
        // Khởi tạo các biến thành giá trị mặc định
        bottom_dst = {0, 0, 0, 0};
        bottom_src = {0, 0, 0, 0};
        top_dst = {0, 0, 0, 0};
        top_src = {0, 0, 0, 0};
        passed = false;
    }

    Pipe(float x, float height)
    {
        // x là toạ độ của đầu ống(tính từ cạnh trái của cửa số game)
        //height: chiều cao của ống dưới, tính từ đáy cửa sổ game lên đến độ cao của ống dưới.
        // Ống dưới
        bottom_dst.x = x; // ta đặt toạ độ x đỉnh trái là x
        bottom_dst.y = HEIGHT - height; // toạ độ y = chiều cao trò chs - chiều cao ống dưới
        bottom_dst.w = PIPE_WIDTH; // chiều rông của ống
        bottom_dst.h = height ; // chiều cao của ống

        bottom_src.x = 0; //được đặt là 0, vì phần hình ảnh của ống dưới được lấy từ đầu file ảnh.
        bottom_src.y = 0; // được đặt là 0, vì phần hình ảnh của ống dưới được lấy từ đầu file ảnh.
        bottom_src.w = PIPE_WIDTH_SRC; // vì chiều rộng của phần hình ảnh ống dưới là cố định và được xác định trong file nguồn ảnh.
        bottom_src.h = height; // vì chiều cao phần ống dưới phải bằng chiều cao của trò chới

        // Ống trên
        top_dst.x = x; // Toạ độ x của đỉnh trái đặt là x
        top_dst.y = 0; // Tọa độ y của đỉnh trái của ống được đặt là 0, vì đỉnh của ống trên phải nằm ở phía trên cùng của cửa sổ nên phải đặt là 0.
        top_dst.w = PIPE_WIDTH; // chiều rộng của ống
        top_dst.h = (HEIGHT - height - PIPE_GAP); // chiều cao của ông = chiều cao của số trò chs - chiều cao của ống dưới - khoảng cách 2 ống

        top_src.x = top_src.y = 0;//được đặt bằng 0, vì phần hình ảnh của ống trên cũng được lấy từ đầu file ảnh.
        top_src.w = PIPE_WIDTH_SRC; // vì chiều rộng của phần hình ảnh ống trên cũng là cố định và được xác định trong file nguồn ảnh.
        top_src.h = top_dst.h;

        passed = false;
    }
    
    // sao chép 1 cột này sang cột khác
    void render(SDL_Renderer *renderer, SDL_Texture *tex) // con trỏ đến bộ vẽ , con trỏ đến bộ nhớ chứa hình ảnh ống
    {
        SDL_RenderCopyEx(renderer, tex, &bottom_src, &bottom_dst, 180 - angle , NULL, SDL_FLIP_VERTICAL);
        // Vẽ ống dưới lên màn hình. Hàm này sử dụng renderer để vẽ tex lên màn hình theo tọa độ và kích thước được xác định bởi bottom_src và bottom_dst.
        /*renderer: Con trỏ đến bộ vẽ được sử dụng để vẽ hình ảnh lên màn hình.

        tex: Con trỏ đến bộ nhớ chứa hình ảnh của ống.

        bottom_src: Kích thước và vị trí của phần hình ảnh của ống trong bộ nhớ tex.

        bottom_dst: Kích thước và vị trí của ống trên màn hình.  */
        SDL_RenderCopyEx(renderer, tex, &top_src, &top_dst, angle , NULL, SDL_FLIP_VERTICAL);
        //Vẽ ống trên lên màn hình. Hàm này cũng sử dụng renderer để vẽ tex lên màn hình, nhưng nó sử dụng thêm các tham số để lật ngược phần hình ảnh của ống trên.
        /*renderer: Con trỏ đến bộ vẽ được sử dụng để vẽ hình ảnh lên màn hình.

         tex: Con trỏ đến bộ nhớ chứa hình ảnh của ống.

         top_src: Kích thước và vị trí của phần hình ảnh của ống trong bộ nhớ tex.

         top_dst: Kích thước và vị trí của ống trên màn hình.

         0: Góc xoay của phần hình ảnh của ống trên (không xoay).

         NULL: Trục quay của phần hình ảnh của ống trên (không sử dụng).

         SDL_FLIP_VERTICAL: Tham số này được sử dụng để lật ngược phần hình ảnh của ống trên, để nó xuất hiện đúng vị trí trên màn hình.*/
    }
    bool m_isScored;
    bool isScored() const { return m_isScored; } // phương thức getter cho thuộc tính isScored
    void setScored(bool scored) { m_isScored = scored; } // ktra có chạm cột không
    bool isClosed;
    void update(float dt, float velocity)
    {
        angle = (float)(sin(SDL_GetTicks() * 0.001f) * 20.0f); // Update the angle of the pipe
        bottom_dst.x -= velocity * dt; // Move the pipe to the left
        top_dst.x = bottom_dst.x; // Move the top pipe together with the bottom pipe
    }


};

#endif

#ifndef DEFS_H_
#define DEFS_H_

#define WIDTH 1150 // chiều rộng
#define HEIGHT 800 // chiều cao

#define FPS 60 //  độ trễ cho khung chương trình
#define frameDelay 1000 / FPS // chạy với tốc độ 60 khung hình mỗi giây

#define GRAVITY 85.0f // trọng lưc càng thấp thì sẽ rơi xuống đất càng chậm

#define BIRD_WIDTH 55  // chiều rộng của chim
#define BIRD_HEIGHT 39 // chiều cao của chim

// Kích thước của đường ống

#define PIPE_WIDTH 80 // chiều rộng của ống
#define PIPE_WIDTH_SRC 52 // chiều rộng của hình ảnh ống trong tệp nguồn
#define PIPE_GAP 270 // khoảng cách giữa ống dưới và ống trên
#define PIPE_DISTANCE 410 // khoảng cách giữa các cặp ống
#define PIPE_V 3.3 // Tốc độ di chuyển của ống

#define PIPE_MIN_H 140 // Chiều cao tối thiểu của ống
#define PIPE_MAX_H 430 // Chiều cao tối đa của ống

#define PIPE_SPAWN_INTERVAL 1.2f // giá trị thời gian giữa mỗi lần sinh ra hai cột mới là 1.2 giây


// Định nghĩa mặt đất
#define GROUND_HEIGHT 112 // mặt đất cách đáy cửa sổ
#define GROUND_WIDTH 400 // chiều rộng mặt đất


static SDL_Surface* g_screen = NULL;

#define PIPE_GAP_MIN  100
#define PIPE_GAP_MAX  200
#define PIPE_MIN_HEIGHT  50
#define PIPE_MAX_HEIGHT 300
#define PIPE_SPEED_INCREMENT 0.01f
#define PIPE_MAX_SPEED  8.0f
#define pipe_speed  0.0f

#endif

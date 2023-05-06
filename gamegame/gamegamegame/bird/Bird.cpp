#include "Bird.h"

void Bird::init()
{
    currentRenderingTexture = mid; // đặt vị trí hiện tại của chim là mid
    animationFrames = 0; //đặt số frame của animation của chim bằng 0.
    score = 0;// đặt điểm ban đầu = 0

    velocity = 0.0f; //đặt tốc độ di chuyển của chim bằng 0
    acceleration = 0.0f; // đặt gia tốc của chim bằng 0

    pos.x = WIDTH / 3 - 32; //đặt tọa độ x của chim sao cho nó xuất hiện ở phía bên trái màn hình
    pos.y = HEIGHT / 2; // đặt tọa độ y của chim sao cho nó xuất hiện ở giữa màn hình theo chiều dọc.
    pos.w = BIRD_WIDTH; // đặt chiều rộng của chim bằng độ rộng của texture.
    pos.h = BIRD_HEIGHT; // đặt chiều cao của chim bằng độ cao của texture.
}

Bird::Bird(SDL_Texture *up, SDL_Texture *mid, SDL_Texture *down, SDL_Renderer *renderer) : up(up), mid(mid), down(down), renderer(renderer)
{
    init(); // khởi tạo trạng thái ban đầu cho chim
}
// cập nhật trạng thái cho game như sự kiện, trạng thái
void Bird::update(bool jump,bool moveLeft,bool moveRight, float elapsedTime)
// jump ktra xem người chơi đã nhấn cho chim nhảy hay chưa
//elapsedTime biểu thị thời gian trôi qua kể cả lần cập nhật trước
{
    elapsedTime *= 5; // tăng tốc độ di chuyển của chim kể từ lần cập nhật trước
    if(jump) // nếu bay
    {
        acceleration = 0.0f; // đặt trọng lực hay gia tốc =0
        velocity = -GRAVITY; // cập nhật lại để cho chim bay lên
    }
    else if(moveLeft) // nếu bay
    {
        acceleration = 0.0f; // đặt trọng lực hay gia tốc =0
        velocity = -GRAVITY; // cập nhật lại để cho chim bay lên
    }
    else if(moveRight) // nếu bay
    {
        acceleration = 0.0f; // đặt trọng lực hay gia tốc =0
        velocity = -GRAVITY; // cập nhật lại để cho chim bay lên
    }
    
    else
    {
        acceleration += GRAVITY * elapsedTime; // nếu ko bay thì ta sẽ đặt lại trọng lực để chim rơi xuống
    }

    if(acceleration >= GRAVITY) // luôn gán trọng lực = trọng lực cho phép
        acceleration = GRAVITY;  // để chim có thể rơi xuống từ từ
    if (getOnFire())
   {
       hp--; // giảm mạng của chim đi một đơn vị
       setOnFire(false); // đặt trạng thái cháy của chim thành false
   }

    // nếu ko có 2 hàm thì đối tượng sẽ không thể cập nhập vị trí và vận tốc của vật
    velocity += acceleration * elapsedTime; // vận tốc += gia tốc * thời gian
    pos.y += velocity * elapsedTime; // vị trí của vật so với trục y

}

bool Bird::collisionDetector(Pipe *pipe)
{
    // pipe
    // pipe->top_dst.x : toạ độ x trên ống nước
    // pipe->top_dst.y : toạ độ y trên ống nước
    // pos.x : toạ độ x của chim
    // pos.w : chiều rộng của chim
    // pos.y : Toạ độ y của con chim
    // pipe->top_dst.h : CHiều cao phần trên ống nước
    // pipe->bottom_dst.y: Toạ độ y phần dưới ống nước

    bool collided = false;


//    if(pipe->top_dst.x <= pos.x + pos.w && pipe->top_dst.x + PIPE_WIDTH >= pos.x + pos.w)
//    {
//        if(pos.y < pipe->top_dst.y + pipe->top_dst.h || pos.y + pos.h > pipe->bottom_dst.y)
//        {
//            collided = true;
//        }
//    }

    // ground
    // kiểm tra xem nếu toạ độ y của chim > chiều cao của khung - mặt đất
    if(pos.y + pos.h >HEIGHT - GROUND_HEIGHT)
    {
        collided = true;
    }
    // ceiling
    if(pos.y < 0) // nếu toạ độ y của chim < 0
    {
        collided = true;
    }

    // Check collision with left and right boundaries
    if (pos.x < 0 || pos.x + pos.w > WIDTH)
    {
        collided = true;
    }

    if(!pipe->passed && pipe->top_dst.x + PIPE_WIDTH < pos.x)
    {
        pipe->passed = true;
        Mix_Chunk* beep_sound = Mix_LoadWAV("ting.wav");
        if (beep_sound != NULL)
        {
            Mix_PlayChannel(-1, beep_sound, 0);
            Mix_FreeChunk(beep_sound);
        }
        else std:: cout << "No load file beep_sound" <<std::endl;
        score++;
    }

    return collided;
}

void Bird::render()
{
    animation(); // Phương thức này được gọi để cập nhật trạng thái của con chim, ví dụ như thay đổi hình ảnh để tạo hiệu ứng bay.

    if(velocity == 0) 
        SDL_RenderCopy(renderer, mid, NULL, &pos);
    else if(velocity < 50)
        SDL_RenderCopyEx(renderer, currentRenderingTexture, NULL, &pos, -30.0, NULL, SDL_FLIP_NONE);
    else if(velocity >= 50 && velocity < 200)
        SDL_RenderCopyEx(renderer, currentRenderingTexture, NULL, &pos, 30.0, NULL, SDL_FLIP_NONE);
    else if(velocity >= 200)
        SDL_RenderCopyEx(renderer, mid, NULL, &pos, 90.0, NULL, SDL_FLIP_NONE); // hình ảnh con chim khi rơi xuống
    // SDL_FLIP_NONE hình ảnh sẽ không bị lật theo chiều ngang hoặc dọc khi được vẽ trên màn hình.

}

void Bird::animation()
{
    animationFrames++;
    // animationFrames: biến đếm số khung hình đã hiển thị kể từ lần cuối cùng Bird đổi hình ảnh.
    //currentRenderingTexture: biến đại diện cho texture (hình ảnh) hiện tại cần được vẽ.
    if(animationFrames == 5)
        currentRenderingTexture = down;
    else if(animationFrames == 15)
        currentRenderingTexture = mid;
    else if(animationFrames == 20)
    {
        animationFrames = 0; // nếu ko đặt lại về ko thì có thể bị crash hoặc lỗi
        currentRenderingTexture = up;
    }
    // hàm animation() đảm bảo rằng hình ảnh của đối tượng Bird sẽ được thay đổi đều đặn và phù hợp với trạng thái di chuyển của nó
}

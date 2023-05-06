#include "Game.h"
#include "iomanip"
#include "cmath"

TTF_Font* g_front_text = NULL;

Game::Game(const char *title, int width, int height) // tiêu đề ,chiều rộng ,chiều cao
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN); // tạo ra 1 cửa sổ có kích thước là w,h
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //  giá trị là -1, SDL sẽ chọn driver GPU tốt nhất.
    //SDL_RENDERER_ACCELERATED: Cho phép renderer sử dụng GPU để tăng tốc độ vẽ hình ảnh lên màn hình, đặc biệt hiệu quả khi vẽ các hình ảnh phức tạp, chẳng hạn như vẽ các đối tượng game.
    // SDL_RENDERER_PRESENTVSYNC: Đảm bảo rằng hình ảnh được vẽ ra màn hình với tốc độ cập nhật tối đa của màn hình, từ đó giảm thiểu hiện tượng screen tearing và giúp cho chất lượng hình ảnh được cải thiện hơn.
    loadTextures();

    bird = new Bird(tex_playerDown, tex_playerMid, tex_playerUp, renderer);
    menu = new Menu(renderer,g_front_text);

    isRunning = true;
    gameover = false;

    ground1 = -WIDTH / 2;
    ground2 = WIDTH / 2;

    srand(static_cast<unsigned int>(time(NULL))); // tạo ra các giá trị khác nhau của ống nếu ko cần cx ko sao
}

void Game::init()
{
    bird->init(); // khởi taọ trạng thái ban đầu của trò chơi

    if(bird->hp==3) menu->show(); // có 3 mạng gọi ra menu
    if (TTF_Init() == -1){
        std::cout << "No load ttf" << std::endl;
    //    TTF_GetError();
    }
    if (g_front_text == NULL) {
        std::cout << "Could not load font ARCADE.ttf: " << TTF_GetError() << std::endl;
    }
    
    while (!pipes.empty()) { // nếu ống ko rỗng
        auto x = pipes.front(); // thêm cột đâu vào x
        std::cout << "Value x: "<< x << std::endl;
        pipes.pop_front(); // xoá ống cuối cúng
        // Do something with x
    }

    int lastPipeX = WIDTH / 1.2 + PIPE_DISTANCE*1.7; // lưu vị trí x của ống cuối cùng được thêm vào danh sách

    for (int i = 0; i < 3; i++) {
        int pipeY = rand() % 321 + 220; // lấy ngẫu nhiên pipeY
        pipes.push_back(new Pipe(lastPipeX, pipeY)); // thêm cặp ống mới vào danh sách        

        lastPipeX += PIPE_DISTANCE; // tính toán vị trí x của ống cuối cùng được thêm vào danh sách
    }

    gameStarted = false;
    gameover = false;

    render(); // khởi tạo hàm vẽ
}

void Game::Start()
{
    // initialize everything before the game starts
    init(); // Gọi hàm init để khởi tạo trò chơi bắt đầu
    
    auto t1 = std::chrono::system_clock::now(); // tính toán thời gian đã trôi qua
    auto t2 = t1;

    // main game loop
    
    while (isRunning)
    {
        t1 = t2;
        t2 = std::chrono::system_clock::now();

        std::chrono::duration<float> dt = t2 - t1; // đảm bảo game chạy với tốc độ ko đổi
        // Biến dt tính toán thời gian trôi qua giữa t2,t1

        bool jump = false;
        bool moveLeft = false;
        bool moveRight = false;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                isRunning = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                jump = gameStarted = true;

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_a)
                {
                    moveLeft = true;
                    bird->moveLeft();
                    jump = false;
                }
                else if (event.key.keysym.sym == SDLK_d)
                {
                    moveRight = true;
                    bird->moveRight();
                    jump = false;
                }
            }
        }

        if(frameDelay > dt.count()) // thời gian trôi qua có lớn hơn thời gian chờ khung hùng
            SDL_Delay(frameDelay - dt.count()); // giữ cho tốc độ khung hình ko quá nhanh
        
        if(gameStarted)
        {
            update(jump, moveLeft,moveRight, dt.count(), gameover,g_sound_background); // nếu ko có hàm update thì sẽ bị đứng hình
            if(gameover)
                gameOver();
        }
    }

    Close();
}

void Game::update(bool jump,bool moveLeft,bool moveRight, float elapsedTime, bool &gameover, Mix_Chunk* g_sound_background)
{
    const int MIN_PIPE_HEIGHT = 180;
    const int MAX_PIPE_HEIGHT = 350;
    const int GROUND_SPEED = 5;
    
    bird->update(jump,moveLeft,moveRight, elapsedTime); // cấph nhật giá trị mới nhảy và thời gian giữa 2 thời điểm

    for(auto& pipe : pipes)
    {
        pipe->bottom_dst.x -= PIPE_V + 0.7 ;
        pipe->top_dst.x = pipe->bottom_dst.x + 10  ;
    }

    if(!pipes.empty())
    {
        if(pipes.front()->bottom_dst.x + pipes.front()->bottom_dst.w < 0)
        {
            delete pipes.front();
            pipes.erase(pipes.begin());
            int offset = 0;
            if (bird->score >= 2 && bird->score < 5) {
                offset = PIPE_DISTANCE / 500;
            } else if (bird->score >= 5 && bird->score < 8) {
                offset = PIPE_DISTANCE * 2 / 500;
            } else if (bird->score >= 8) {
                offset = PIPE_DISTANCE * 3 / 500;
            }
            pipes.push_back(new Pipe(pipes.back()->bottom_dst.x + PIPE_DISTANCE + offset, rand() % (MAX_PIPE_HEIGHT - MIN_PIPE_HEIGHT) + MIN_PIPE_HEIGHT));
        }

    }
    else
    {
        pipes.push_back(new Pipe(WIDTH + PIPE_DISTANCE, rand() % (MAX_PIPE_HEIGHT - MIN_PIPE_HEIGHT) + MIN_PIPE_HEIGHT));
    }

    for(auto& pipe : pipes)
    {
        // kiểm tra tính điểm và chim vượt qua ống hay không
        if(!pipe->isScored() && bird->getX() > pipe->bottom_dst.x + pipe->bottom_dst.w)
        {
            pipe->setScored(true);
        }
        // nếu chim va chạm và ko có điểm
        else if(!gameover && !pipe->isScored() && bird->collisionDetector(pipe))
        {
            gameover = true;
            break;
        }
    }

    // nếu chim va chạm với cái đầu tiên
    if(bird->collisionDetector(pipes.front()))
        gameover = true;

    ground1 -= GROUND_SPEED;
    ground2 -= GROUND_SPEED;
    // kiểm tra mặt đất đi quá hay chưa
    if(ground1 + WIDTH < 0)
        ground1 = WIDTH - 10;
    if(ground2 + WIDTH < 0)
        ground2 = WIDTH - 10;
    render();
    
}

void Game::StopGame(){
    isRunning = false;
}

void Game::gameOver()
{
    bool in_gameover = false;

    if (bird->hp == 0) {
        // Render "GameOver" text
        SDL_RenderCopy(renderer, tex_gameover, NULL, new SDL_Rect{WIDTH / 2 - 96, HEIGHT / 3, 192, 42});
        SDL_RenderPresent(renderer);
        in_gameover = true;
    }

    while (in_gameover) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // Quit the game
                isRunning = false;
                in_gameover = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                // bắt đầu game
                bird->hp = 3;
                init();
                in_gameover = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // Kiểm tra vị trí nhấp chuột
                int x = event.button.x;
                int y = event.button.y;
                if (x >= WIDTH / 2 - 96 && x <= WIDTH / 2 + 96 && y >= HEIGHT / 3 && y <= HEIGHT / 3 + 42) {
                    // menu
                    bird->hp = 3;
                    menu->show();
                    in_gameover = false;
                }
            }
        }
    }

    SDL_Delay(100);
    bool wait = true, playagain = false;
    while(wait)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                wait = false;

            if((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) || (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT))
                static_cast<void>(wait = false), playagain = true;
        }
    }
    if (playagain && bird->hp >= 0) {
        bird->hp -= 1;
        Start();
    } else {
        isRunning = false;
    }

}

void Game::render()
{
    SDL_RenderClear(renderer);
    
    // background
    SDL_Texture* current_background = tex_background;

    // Trong mỗi vòng lặp render
    if ( bird->score >= 0 && bird->score< 23  ){
        current_background = tex_background3;
    }
    else if ( bird->score >= 23 && bird->score< 37  ){
        current_background = tex_background1;
    }
    else if ( bird->score >= 38 && bird->score < 55  ){
        current_background = tex_background2;
    }
    else if ( bird->score >= 55 && bird->score< 70  ){
        current_background = tex_background4;
    }
    else if (bird->score >= 70 && bird->score < 100 ){
        current_background = tex_background;
    }
    bool is_randomizing = true; // biến kiểm tra trạng thái "ngẫu nhiên"
    bool is_disappearing = false; // biến kiểm tra trạng thái "biến mất"
    float time_since_spawned = 0.0f; // biến tính thời gian đã trôi qua từ khi sinh ra cột mới
    float dt=0;
    float velocity=0;

    bool is_bird_score_above_10 = false;


    if (bird->score == 3) { // nếu chim đạt 3 điểm, trở lại trạng thái ban đầu của cột
        is_randomizing = true;
        is_disappearing = false;
    }

    if (bird->score == 10 && !is_bird_score_above_10) { // nếu chim đạt 10 điểm và chưa đạt 10 điểm trước đó
        is_randomizing = false;
        is_disappearing = false;
        is_bird_score_above_10 = true; // đánh dấu chim đã đạt 10 điểm
    }
    if (bird->score > 10){
        is_bird_score_above_10 = false;
    }

    if (!is_bird_score_above_10 && bird->score < 10) { // kiểm tra xem chim đã đạt được 10 điểm chưa trước khi thực hiện hàm này
        if (is_disappearing) {
            bird->dst.w -= 5; 
            bird->dst.h -= 5;
            if (bird->dst.w <= 0 || bird->dst.h <= 0) {
                // Khi kích thước của chim đạt 0, ta dừng trò chơi
                gameover = true;
            }
        } else if (is_randomizing && time_since_spawned > PIPE_SPAWN_INTERVAL) {
            // sinh ra cột mới theo cách ngẫu nhiên
            float x = WIDTH + PIPE_WIDTH + PIPE_DISTANCE;
            float gap = rand() % (PIPE_MAX_H - PIPE_MIN_H) + PIPE_MIN_H;
            pipes.push_back(new Pipe(x, 0));
            pipes.push_back(new Pipe(x, HEIGHT - gap));
            time_since_spawned = 0.0f;
        }

        for (auto it = pipes.begin(); it != pipes.end();) {
            Pipe* pipe = *it;
            if (pipe->bottom_dst.x < -PIPE_WIDTH) { // kiểm tra xem cột đã đi qua màn hình chưa
                it = pipes.erase(it);
                delete pipe;
            } else {
                pipe->update(dt, velocity);
                ++it;
            }
        }

        time_since_spawned += dt; // cộng thêm thời gian đã trôi qua từ lần sinh ra 2 cột mới trước đó.
    }
    
    if (bird->score >= 50 && bird->score <= 59) { // Kiểm tra xem điểm số của chim có nằm trong khoảng từ 50 đến 59 không
        if (pipes.size() > 1) { // Nếu đã có ít nhất 2 cột trong mảng pipes
            auto it = std::next(pipes.begin()); // Lấy con trỏ tới cột thứ 2 trong mảng pipes
            Pipe* second_pipe = *it; // Lấy con trỏ tới cột thứ 2

            // Tính toán vị trí mới của cột thứ 2 dựa trên điểm số của chim
            float delta_x = 0.5f * bird->score; // Độ chênh lệch tọa độ x giữa cột thứ 2 và cột thứ nhất
            float delta_y = -5.0f; // Độ chênh lệch tọa độ y giữa cột thứ 2 và cột thứ nhất

            // Nếu cột thứ 2 chưa đi quá màn hình, di chuyển sang trái
            if (second_pipe->bottom_dst.x + second_pipe->bottom_dst.w > 0) {
                // Kiểm tra khoảng cách giữa cột thứ 2 và cột trước đó
                auto prev_it = std::prev(it);
                Pipe* prev_pipe = *prev_it;
                float min_distance = 3 * PIPE_WIDTH; // Khoảng cách tối thiểu giữa 2 cột là 3 lần độ rộng của cột
                if (second_pipe->bottom_dst.x - (prev_pipe->bottom_dst.x + prev_pipe->bottom_dst.w) < min_distance) {
                    // Nếu khoảng cách quá gần, cột thứ 2 sẽ được đặt ở khoảng cách tối thiểu
                    second_pipe->bottom_dst.x = prev_pipe->bottom_dst.x + prev_pipe->bottom_dst.w + min_distance;
                    second_pipe->top_dst.x = second_pipe->bottom_dst.x;
                } else {
                    // Nếu khoảng cách đủ lớn, di chuyển cột thứ 2 sang trái
                    second_pipe->bottom_dst.x -= delta_x;
                    second_pipe->top_dst.x -= delta_x;
                }
            } else {
                delta_x = 0.0f;
            }

            // Tạo hiệu ứng xoay cho cột thứ 2
            static float direction = 1.0f; // Hướng xoay hiện tại
            static int count = 0; // Biến đếm số khung hình đã trôi qua
            if (count >= 40) { // Sau 40 khung hình, hướng xoay sẽ đổi chiều
                count = 0;
                direction = -direction;
            }
            float angle = sin(count * M_PI / 20.0f) * 10.0f * direction; // tính góc xoay và tăng độ nghiêng của cột
            second_pipe->angle = angle;

            second_pipe->bottom_dst.y += delta_y;
            second_pipe->top_dst.y += delta_y;
            second_pipe->top_src.y = second_pipe->top_src.h - second_pipe->top_dst.h;
            second_pipe->bottom_src.y = 0;

            count++;
        }
    }
    if (bird->score >= 60 && bird->score <= 99) {
        float pipes_speed = PIPE_V + 0.003 * bird->score;
        for (auto& pipe : pipes) {
            // Di chuyển ống
            pipe->bottom_dst.x -= pipes_speed;
            pipe->top_dst.x = pipe->bottom_dst.x;
            if (bird->score >= 60 && bird->score <= 69) {
                for (auto& pipe : pipes) {
                    // Di chuyển ống
                    pipe->bottom_dst.x -= pipes_speed;
                    pipe->top_dst.x = pipe->bottom_dst.x;

                    // Tăng độ nghiêng ống
                    pipe->angle = 5.0f;
                }
            }


            // Tăng độ nghiêng ống
            if (bird->score >= 70 && bird->score <= 79) {
                pipe->bottom_dst.x -= pipes_speed;
                pipe->angle = 25.0f;
            } else {
                pipe->bottom_dst.x -= pipes_speed;
                pipe->angle = 15.0f;
            }

            // Đặt ống gần nhau hơn
            if (bird->score >= 80 && bird->score <= 89) {
                // Giảm khoảng cách giữa các ống
                float gap = 180.0f - bird->score * 2.0f;
                if (gap < 50.0f) {
                    gap = 50.0f;
                }

                // Cho ống nghiêng về phía trên
                for (auto& pipe : pipes) {
                    pipe->angle = -10.0f;
                }
            }

            if (bird->score >= 90 && bird->score <= 99) {
                // Giảm tốc độ di chuyển của các ống
                for (auto& pipe : pipes) {
                    pipe->bottom_dst.x -= pipes_speed / 2.0f;
                    pipe->top_dst.x = pipe->bottom_dst.x;

                    // Cho ống nghiêng về phía dưới
                    pipe->angle = 10.0f;
                }
            }

        }
    }

    if (bird->score == 100 ){
        pipes.clear();
        current_background = tex_winner;
        SDL_Delay(100);

        Uint32 start_time = SDL_GetTicks();  // Lấy thời gian hiện tại

        while (SDL_GetTicks() - start_time < 2000) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    isRunning = false;
                    return;
                }
            }
        }

        menu->show();

    }


    // Render background hiện tại
    SDL_RenderCopy(renderer, current_background, NULL, NULL);
    
    // pipes

    if (bird->score >=25 && bird->score <= 33 ) {
        auto it = std::next(pipes.begin());
        Pipe* second_pipe = *it;
        float delta_y = 0.1f;
        float delta_x = 0.1f;  // Thêm delta_x vào
        second_pipe->angle = 15.0f;
        second_pipe->bottom_dst.y -= delta_y;
        second_pipe->bottom_dst.x -= delta_x * bird->score;  // Thay đổi delta_x
        for(auto& pipe : pipes)
        {
            pipe->render(renderer, tex_pipe);
            pipe->top_dst.x = pipe->bottom_dst.x;
        }
    }
    
   else{
        for(auto& pipe : pipes)
        {
            pipe->render(renderer, tex_pipe);
        }
    }

    // hiệu ứng pipes

    if (bird->score >= 11 && bird-> score <= 15) {
            auto it = std::next(pipes.begin());
            Pipe* second_pipe = *it;
            float delta_y = 0.14;
            second_pipe->bottom_dst.y -= delta_y;
            second_pipe->top_dst.y -= delta_y;
            for(auto& pipe : pipes)
            {
                pipe->bottom_dst.x -= PIPE_V+ 0.02 ;
                pipe->top_dst.x = pipe->bottom_dst.x;
            }
    }
    else if (bird->score >= 16 && bird-> score <= 20) {
        auto it = std::next(pipes.begin());
        Pipe* second_pipe = *it;
        float delta_y = 1.0;
        second_pipe->bottom_dst.y -= delta_y;
        second_pipe->bottom_dst.x -= delta_y;
        second_pipe->top_dst.y -= delta_y;
        for(auto& pipe : pipes)
        {
            pipe->bottom_dst.x -= PIPE_V+ 0.15 ;
            pipe->top_dst.x = pipe->bottom_dst.x;
        }
    }

    else if (bird->score > 21){
        for(auto& pipe : pipes)
        {
            pipe->bottom_dst.x -= PIPE_V+ 1.23 ;
            pipe->top_dst.x = pipe->bottom_dst.x;
        }
    }
    if (bird->score >= 35 && bird->score <= 45 ){
        auto it = std::next(pipes.begin(), 1); // lấy con trỏ tới cột thứ hai
        Pipe* second_pipe = *it;
        int first_pipe_x = pipes.front()->bottom_dst.x;
        float movement_speed = 3.0f; // tốc độ di chuyển của cột thứ hai
        movement_speed -= bird->score * 0.005f; // giảm tốc độ di chuyển theo điểm số
        if (movement_speed < 0.0f) {
            movement_speed = 0.0f;
        }
        int second_pipe_x = second_pipe->bottom_dst.x - movement_speed; // tính vị trí mới của cột thứ hai
        pipes.front()->bottom_dst.x = first_pipe_x;
        pipes.front()->top_dst.x = first_pipe_x;
        second_pipe->bottom_dst.x = second_pipe_x;
        second_pipe->top_dst.x = second_pipe_x;
    }
    
    // hp
    for(int i = 0; i < bird->hp; i++) {
           SDL_RenderCopy(renderer, tex_hp, NULL, new SDL_Rect{10 + 40*i, 10, 45, 30});
    }
    
    
    // score
    if(bird->score > 9)
    {
        SDL_RenderCopy(renderer, tex_numbers[bird->score / 10 % 10], NULL, new SDL_Rect{WIDTH / 2 - 20, 100, 40, 52});
        SDL_RenderCopy(renderer, tex_numbers[bird->score % 10], NULL, new SDL_Rect{WIDTH / 2 + 20, 100, 40, 52});
    }
    else
        SDL_RenderCopy(renderer, tex_numbers[bird->score], NULL, new SDL_Rect{WIDTH / 2, 100, 40, 52});

    // ground
    SDL_RenderCopy(renderer, tex_ground, NULL, new SDL_Rect{ground1, HEIGHT - GROUND_HEIGHT, WIDTH, GROUND_HEIGHT});
    SDL_RenderCopy(renderer, tex_ground, NULL, new SDL_Rect{ground2, HEIGHT - GROUND_HEIGHT, WIDTH, GROUND_HEIGHT});
    
    
    // player
    bird->render(); // vẽ hình ảnh chim

    SDL_RenderPresent(renderer); // load hết tất cả ảnh
}

// hàm load ảnh

void Game::loadTextures()
{
    tex_background = IMG_LoadTexture(renderer, "background-day.png");
    tex_background1 = IMG_LoadTexture(renderer, "BGBack.png");
    tex_background2 = IMG_LoadTexture(renderer, "BGFront.png");
    tex_background3 = IMG_LoadTexture(renderer, "CloudsBack.png");
    tex_background4 = IMG_LoadTexture(renderer, "CloudsFront.png");
    tex_pipe = IMG_LoadTexture(renderer, "pipe.png");
    tex_playerMid = IMG_LoadTexture(renderer, "yellowbird-midflap.png");
    tex_playerUp = IMG_LoadTexture(renderer, "yellowbird-upflap.png");
    tex_playerDown = IMG_LoadTexture(renderer, "yellowbird-downflap.png");
    tex_ground = IMG_LoadTexture(renderer, "base.png");
    tex_gameover = IMG_LoadTexture(renderer, "gameover.png");
    tex_hp = IMG_LoadTexture(renderer, "hp.png");
    tex_winner = IMG_LoadTexture(renderer, "winner.png");


    for(int i = 0; i < 10; i++)
    {
        std::string path = std::to_string(i) + ".png"; // load 9 điểm
        tex_numbers[i] = IMG_LoadTexture(renderer, path.c_str());
    }
    
}

void Game::Close()
{
    SDL_Quit(); // thoát trò chơi
}

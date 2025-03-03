#include <stdio.h>
#include <SDL3/SDL.h>
#define SCREEN_WIDTH 1206
#define SCREEN_HEIGHT 800
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define GRID_COLOR 0x2a2a2a2a
#define GRID_SIZE 14
#define GRID_WIDTH 2
#define ROWS SCREEN_HEIGHT/GRID_SIZE
#define COLS SCREEN_WIDTH/GRID_SIZE
#define SNAKE_COLOR 0xffffffff

struct Snake {
    double cell_x, cell_y;
    int size; //relative to cell size
    int width;
    double speed;
    int direction; //1, 2, 3, 4 (U, D, L, R)
};


void drawGrids(SDL_Surface* surface) {
    //rows    
    for(int i = 0; i <= ROWS; i++){
        SDL_Rect line = {0, GRID_SIZE * i, SCREEN_WIDTH, GRID_WIDTH};
        SDL_FillSurfaceRect(surface, &line, GRID_COLOR);
    }
    //cols    
    for(int i = 0; i <= COLS; i++){
        SDL_Rect line = {GRID_SIZE * i, 0, GRID_WIDTH, SCREEN_HEIGHT};
        SDL_FillSurfaceRect(surface, &line, GRID_COLOR);
    }
}

void drawSnake(SDL_Surface* surface, struct Snake* snake) {
    SDL_Rect snake_rect = {snake->cell_x * GRID_SIZE, snake->cell_y * GRID_SIZE, snake->width * GRID_SIZE, snake->size * GRID_SIZE};

    SDL_FillSurfaceRect(surface, &snake_rect, SNAKE_COLOR);
}

void moveSnake(struct Snake* snake) {

    switch(snake->direction) {
        case 1: { //up
                    snake->cell_y -= snake->speed;
                    break;
                }

        case 2: { //down
                    snake->cell_y += snake->speed;
                    break;
                }

        case 3: { //left
                    snake->cell_x -= snake->speed;
                    break;

                }

        case 4: { //right
                    snake->cell_x += snake->speed;
                    break;

                }
    }

}


void changeDirection(struct Snake* snake, int keyCode){

    switch(keyCode) {
        case SDLK_W:
            snake->direction = 1;
            break;
        case SDLK_S:
            snake->direction = 2;
            break;
        case SDLK_A:
            snake->direction = 3;
            break;
        case SDLK_D:
            snake->direction = 4;
            break;
        default:
        break;
    }


}


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Snake", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_Rect black_screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    struct Snake snake  = {COLS/2, ROWS/2, 3, 1, 0.1, 1};
    int running = 1; //1 for true and 0 for false
    SDL_Event event;

    while(running) {

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                running = 0;  
            }
            if(event.type == SDL_EVENT_KEY_DOWN){
                if(event.key.key == SDLK_W || event.key.key == SDLK_S || event.key.key == SDLK_A|| event.key.key == SDLK_D){

                    int keyCode = event.key.key;
                    changeDirection(&snake, keyCode);
                }
            }

        }

        SDL_FillSurfaceRect(surface, &black_screen, COLOR_BLACK);
        //draw grids
        drawGrids(surface);

        //draw snake
        drawSnake(surface, &snake);
        //move snake
        moveSnake(&snake);

        SDL_UpdateWindowSurface(window);

        SDL_Delay(10);
    }


    SDL_DestroyWindow(window);
    SDL_Quit();



    return 0;
}


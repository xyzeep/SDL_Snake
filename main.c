#include <stdio.h>
#include <SDL3/SDL.h>
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_GRID 0x2a2a2a2a
#define COLOR_APPLE 0x00ff0000
#define SNAKE_COLOR 0xffffffff
#define CELL_SIZE 40
#define GRID_WIDTH 2
#define FPS 30
#define ROWS SCREEN_HEIGHT/CELL_SIZE
#define COLS SCREEN_WIDTH/CELL_SIZE


#define SNAKE(x, y) fill_cell(surface, x, y, COLOR_WHITE)
#define APPLE(x, y) fill_cell(surface, x, y, COLOR_APPLE)

struct Snake {
    int x, y;
    int length; //relative to cell size
    double speed;
    int direction; //1, 2, 3, 4 (U, D, L, R)
};



void fill_cell(SDL_Surface* surface, int x, int y, Uint32 color) {

    SDL_Rect rect = {x * CELL_SIZE + GRID_WIDTH, y*CELL_SIZE+ GRID_WIDTH, CELL_SIZE - GRID_WIDTH, CELL_SIZE - GRID_WIDTH};
    SDL_FillSurfaceRect(surface, &rect, color);

}

void draw_snake (SDL_Surface* surface, struct Snake* snake) {
    switch (snake->direction) {
        case 1:
            snake->y --;
            break;
    }

}

void drawGrid(SDL_Surface* surface)  {
    //rows    
    for(int i = 0; i <= ROWS; i++){
        SDL_Rect line = {0, CELL_SIZE * i, SCREEN_WIDTH, GRID_WIDTH};
        SDL_FillSurfaceRect(surface, &line, COLOR_GRID);
    }
    //cols    
    for(int i = 0; i <= COLS; i++){
        SDL_Rect line = {CELL_SIZE * i, 0, GRID_WIDTH, SCREEN_HEIGHT};
        SDL_FillSurfaceRect(surface, &line, COLOR_GRID);
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Snake", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    int FPS_COUNTER = 0;

    SDL_Rect black_screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    struct Snake snake  = {10, 10, 3, 1, 1};
    int running = 1; //1 for true and 0 for false
    SDL_Event event;

    //////////
    int snake_x = 10;
    int snake_y = 10;
    int apple_x = 18;
    int apple_y = 10;
    //////////

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                running = 0;  
            }
            if(event.type == SDL_EVENT_KEY_DOWN){
                if (event.key.key == SDLK_W){
                    snake.direction = 1;
                }
                else if (event.key.key == SDLK_S) {
                    snake.direction  = 2;
                }

                else if (event.key.key == SDLK_A) {
                    snake.direction = 3;
                }
                else if (event.key.key == SDLK_D) {
                    snake.direction = 4;
                }

            }
        }


        SDL_FillSurfaceRect(surface, &black_screen, COLOR_BLACK);

        //draw grids
        drawGrid(surface);

        draw_snake(surface, &snake);
        SNAKE(snake.x, snake.y);
        APPLE(apple_x, apple_y);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(100);
    }






    SDL_DestroyWindow(window);
    SDL_Quit();



    return 0;
}


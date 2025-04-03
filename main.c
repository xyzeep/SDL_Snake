#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

#define COLOR_BLACK 0x00000000
#define COLOR_GRID 0x75757575
#define COLOR_APPLE 0x00ff0000
#define COLOR_SNAKE 0xffffffff

#define CELL_SIZE 40
#define GRIDLINE_WIDTH 2

#define INITIAL_LENGTH 4

#define ROWS SCREEN_HEIGHT / CELL_SIZE
#define COLS SCREEN_WIDTH / CELL_SIZE

#define SNAKE() draw_snake(surface, &snake)
#define APPLE(x, y) fill_cell(surface, x, y, COLOR_APPLE)

struct segment {
    int x, y; // relative to cells
};

struct Snake
{
    int length; // relative to cell size
    double speed;
    int direction; // 1, 2, 3, 4 (U, D, L, R)
    struct segment *segments;
};

void initialize_snake(struct Snake snake, int start_x, int start_y)
{
    for (int i = 0; i < snake.length; i++)
    {
        snake.segments[i].x = start_x;
        snake.segments[i].y = start_y;
    }
}

void update_segments(struct Snake *snake)
{

    switch (snake->direction)
    {
    case 1:
        snake->segments[snake->length - 1].y--;
        break;
    case 2:
        snake->segments[snake->length - 1].y++;
        break;
    case 3:
        snake->segments[snake->length - 1].x--;
        break;
    case 4:
        snake->segments[snake->length - 1].x++;
        break;
    }
    for (int i = 0; i < snake->length - 1; i++)
    {
        snake->segments[i] = snake->segments[i + 1];
    }
}

void draw_snake(SDL_Surface *surface, struct Snake *snake) {
    SDL_Rect rect = {snake->segments[0].x, snake->segments[0].y, CELL_SIZE - GRIDLINE_WIDTH, CELL_SIZE - GRIDLINE_WIDTH};

    for (int i = 0; i < snake->length; i++) {
        rect.x = snake->segments[i].x * CELL_SIZE + GRIDLINE_WIDTH;
        rect.y = snake->segments[i].y * CELL_SIZE + GRIDLINE_WIDTH;

        if (i == snake->length - 1) {
            SDL_FillSurfaceRect(surface, &rect, COLOR_GRID);
        }
        else {
            SDL_FillSurfaceRect(surface, &rect, COLOR_SNAKE);
        }
    }
}

void fill_cell(SDL_Surface *surface, int x, int y, Uint32 color) {
    SDL_Rect rect = {x * CELL_SIZE + GRIDLINE_WIDTH, y * CELL_SIZE + GRIDLINE_WIDTH, CELL_SIZE - GRIDLINE_WIDTH, CELL_SIZE - GRIDLINE_WIDTH};

    SDL_FillSurfaceRect(surface, &rect, color);
}

void drawGrid(SDL_Surface *surface)
{
    // rows
    for (int i = 0; i <= ROWS; i++)
    {
        SDL_Rect line = {0, CELL_SIZE * i, SCREEN_WIDTH, GRIDLINE_WIDTH};
        SDL_FillSurfaceRect(surface, &line, COLOR_GRID);
    }
    // cols
    for (int i = 0; i <= COLS; i++)
    {
        SDL_Rect line = {CELL_SIZE * i, 0, GRIDLINE_WIDTH, SCREEN_HEIGHT};
        SDL_FillSurfaceRect(surface, &line, COLOR_GRID);
    }
}

void new_apple(int* apple_x, int* apple_y) {

    *apple_x = 1 + rand() % COLS;
    *apple_y = 1 + rand() % ROWS;
}



int main()
{
    // SDL initialization part
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Snake", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Rect black_screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // to reset screen on every iteration
    
    
    int running = 1; // 1 for true and 0 for false
    
    
    //SNAKE
    struct Snake snake = {INITIAL_LENGTH, 1, 4}; // length, speed, direction

    // Snake segments
    snake.segments = malloc(sizeof(struct segment) * snake.length);
    if (snake.segments == NULL) {
        printf("malloc failed!\n");
        exit(1); // terminate game
    }
    
    //random seed
    srand(time(NULL)); 

    // initial values
    int snake_start_x = 3 + rand() % (COLS - 6 + 1);
    int snake_start_y = 3 + rand() % (ROWS - 6 + 1);
    
    int apple_x = 1 + rand() % (COLS);
    int apple_y = 1 + rand() % (ROWS);
   

    initialize_snake(snake, snake_start_x, snake_start_y);

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_W && snake.direction != 2)
                {
                    snake.direction = 1;
                    new_apple(&apple_x, &apple_y);
                }
                else if (event.key.key == SDLK_S && snake.direction != 1)
                {
                    snake.direction = 2;
                }

                else if (event.key.key == SDLK_A && snake.direction != 4)
                {
                    snake.direction = 3;
                }
                else if (event.key.key == SDLK_D && snake.direction != 3)
                {
                    snake.direction = 4;
                }
            }
        }

        SDL_FillSurfaceRect(surface, &black_screen, COLOR_BLACK);

        // draw grids
        drawGrid(surface);

        update_segments(&snake);
        
        SNAKE();
        APPLE(apple_x, apple_y);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(1000);
    }

    free(snake.segments);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

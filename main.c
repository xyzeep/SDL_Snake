#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 820

#define COLOR_BLACK 0x00000000
#define COLOR_GRID 0x1f1f1f1f
#define COLOR_APPLE 0x00ff0000
#define COLOR_SNAKE 0xffffffff

#define CELL_SIZE 60
#define GRIDLINE_WIDTH 2

#define INITIAL_LENGTH 3

#define ROWS SCREEN_HEIGHT / CELL_SIZE
#define COLS SCREEN_WIDTH / CELL_SIZE

#define SNAKE() draw_snake(surface, &snake)
#define APPLE() draw_apple(surface, apple_x, apple_y)
#define NEW_APPLE() new_apple(&apple_x, &apple_y)
#define NEW_GAME() new_game(surface, &snake, &apple_x, &apple_y)

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

void new_snake(struct Snake* snake)
{
    // Snake segments
    snake->segments = malloc(sizeof(struct segment) * 5); //change this later
    if (snake->segments == NULL) {
        printf("malloc failed!\n");
        exit(1); // terminate game
    }

    //random seed
    srand(time(NULL)); 

    // initial values
    int snake_start_x = 3 + rand() % (COLS - 6 + 1);
    int snake_start_y = 3 + rand() % (ROWS - 6 + 1);

    snake->length = INITIAL_LENGTH;
    snake->speed = 1;
    snake->direction = 1 + rand() % 4;

    for (int i = 0; i < snake->length; i++)
    {
        snake->segments[i].x = snake_start_x;
        snake->segments[i].y = snake_start_y;
    }
}

void reallocate_snake(struct Snake* snake) {

    (snake->segments) = realloc(snake->segments,(snake->length + 5) * sizeof(struct segment));
    
}

void update_segments(struct Snake *snake)
{
    for (int i = snake->length - 1; i > 0; i--)
    {
        snake->segments[i] = snake->segments[i - 1];
    }

    switch (snake->direction)
    {
        case 1:
            snake->segments[0].y--;
            break;
        case 2:
            snake->segments[0].y++;
            break;
        case 3:
            snake->segments[0].x--;
            break;
        case 4:
            snake->segments[0].x++;
            break;
    }
}

void draw_snake(SDL_Surface *surface, struct Snake *snake) {
    SDL_Rect rect = {snake->segments[0].x, snake->segments[0].y, CELL_SIZE - GRIDLINE_WIDTH, CELL_SIZE - GRIDLINE_WIDTH};

    for (int i = 0; i < snake->length; i++) {
        rect.x = snake->segments[i].x * CELL_SIZE + GRIDLINE_WIDTH;
        rect.y = snake->segments[i].y * CELL_SIZE + GRIDLINE_WIDTH;

        SDL_FillSurfaceRect(surface, &rect, COLOR_SNAKE);

    }
}

bool check_collision(struct Snake snake) {

    int head_x = snake.segments[0].x;
    int head_y = snake.segments[0].y;

    //border collision
    if ((head_x == 0 ||head_x == COLS) || (head_y == 0 || head_y == ROWS)) {
        return true;
    }

    // self collision
    for (int i = 1; i < snake.length; i++) {
        if(head_x == snake.segments[i].x  && head_y == snake.segments[i].y) {
            return true;
        }
    }

    return false;
}

void new_apple(int* apple_x, int* apple_y) {
    *apple_x = 3 + rand() % (COLS - 4);
    *apple_y = 3 + rand() % (ROWS - 4);
    printf("ROWS and COlS: %d, %d\n", *apple_y, *apple_x);
}
void new_game(SDL_Surface* surface, struct Snake* snake, int* apple_x, int* apple_y) {
    new_snake(snake);
    new_apple(apple_x, apple_y);
}


void draw_apple(SDL_Surface *surface, int x, int y) {
    SDL_Rect rect = {x * CELL_SIZE + GRIDLINE_WIDTH, y * CELL_SIZE + GRIDLINE_WIDTH, CELL_SIZE - GRIDLINE_WIDTH, CELL_SIZE - GRIDLINE_WIDTH};

    SDL_FillSurfaceRect(surface, &rect, COLOR_APPLE);
}

bool check_apple_collision(struct Snake snake, int apple_x, int apple_y) {
    if (snake.segments[0].x == apple_x && snake.segments[0].y == apple_y) return true;
    return false;
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




int main()
{
    // SDL initialization part
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Snake", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Rect black_screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // to reset screen on every iteration


    int running = 1; // 1 for true and 0 for false
    int current_max_length = 10;


    //intial apple values
    int apple_x, apple_y;
    //SNAKE
    struct Snake snake;

    new_snake(&snake);
    NEW_APPLE();

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

        //check if snake ate apple
        if (check_apple_collision(snake, apple_x, apple_y)) {
            NEW_APPLE();
            snake.length++;
            if (snake.length > current_max_length - 2) {
                reallocate_snake(&snake);
                current_max_length += 5;
                printf("just reallocted");
            }
            
            printf("snake size: %zu bytes\n", sizeof(snake.segments));
        }

        if (check_collision(snake)){
            free(snake.segments);
            NEW_GAME();
       } 
        SDL_FillSurfaceRect(surface, &black_screen, COLOR_BLACK);

        // draw grids
        drawGrid(surface);

        update_segments(&snake);

        SNAKE();
        APPLE();

        SDL_UpdateWindowSurface(window);
        SDL_Delay(200);
    }

    free(snake.segments);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

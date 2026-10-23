#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <stdio.h>

const int HEIGHT = 600;
const int WIDTH = 800;

int dx = 300;
int dy = 300;
const int PADDLE_SPEED = 300;

const int RECT_WIDTH = 20;
const int RECT_HEIGHT = 100;

const Uint64 FRAME_NS = 1000000000ULL / 200;

typedef struct {
  int x;
  int y;
  int w;
  int h;
} Ball;

typedef struct {
  int x;
  int y;
  int w;
  int h;
} Paddle;

bool collision(Ball b, Paddle r) {
  return ((b.x < r.x + r.w) && (b.x + b.w > r.x) && (b.y < r.y + r.h) && (b.y + b.h> r.y));
}

int get_press_direction() {
  const bool *key_states = SDL_GetKeyboardState(NULL);

  if (key_states[SDL_SCANCODE_UP] || key_states[SDL_SCANCODE_W]) {
    return -1;
  }

  if (key_states[SDL_SCANCODE_DOWN] || key_states[SDL_SCANCODE_S]) {
    return 1;
  }

  return 0;
}

int get_ai_paddle_dir(Ball b, Paddle r) {
  float ball_centre = b.y + b.h / 2;
  float paddle_centre = r.y + r.h / 2;
  float dead_zone = 10.0f;

  if (ball_centre < paddle_centre - dead_zone) {
    return -1;
  }

  if (ball_centre > paddle_centre + dead_zone) {
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log(SDL_GetError());
    return 1;
  }

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (!SDL_CreateWindowAndRenderer("PONG", WIDTH, HEIGHT, 0, &window, &renderer)) {
    SDL_Log("Window creation failed %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  int points_x = 0;
  int points_y = 0;

  Ball ball_xy = {x: WIDTH / 2, y: HEIGHT / 2, w: RECT_WIDTH, h: RECT_WIDTH};
  Paddle paddle_first = {x: 0, y: 100, w: RECT_WIDTH, h: RECT_HEIGHT};
  Paddle paddle_second = {x: 780, y: 100, w: RECT_WIDTH, h: RECT_HEIGHT};
      
  bool running = true;

  Uint64 last_time = SDL_GetTicksNS();
  
  while (running) {
    Uint64 frame_start = SDL_GetTicksNS();
    float dt = (frame_start - last_time) / 1e9f;
    last_time = frame_start;

    // INPUT
    
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
	running = false;
      }
    }

    // LOGIC

    paddle_first.y += get_press_direction() * dt * PADDLE_SPEED;

    if (paddle_first.y < 0) {
      paddle_first.y = 0;
    }

    if (paddle_first.y + paddle_first.h >= HEIGHT) {
      paddle_first.y = HEIGHT - paddle_first.h;
    }
    
    paddle_second.y += get_ai_paddle_dir(ball_xy, paddle_second) * dt * PADDLE_SPEED;

    if (paddle_second.y < 0) {
      paddle_second.y = 0;
    }

    if (paddle_second.y + paddle_second.h >= HEIGHT) {
      paddle_second.y = HEIGHT - paddle_second.h;
    }

    if (ball_xy.y + dy * dt < 0 || ball_xy.y + RECT_WIDTH + dy * dt > HEIGHT) {
      dy *= -1;
    }

    if (collision(ball_xy, paddle_first) || collision(ball_xy, paddle_second)) {
      dx *= -1;
    }

    if (ball_xy.x + dx * dt < 0) {
      ball_xy.x = WIDTH / 2;
      ball_xy.y = HEIGHT / 2;
      points_y++;
    }

    if (ball_xy.x + RECT_WIDTH + dx * dt > WIDTH) {
      ball_xy.x = WIDTH / 2;
      ball_xy.y = HEIGHT / 2;
      points_x++;
    }

    if (points_x > 10) {
      printf("Player 1 won");
      running = false;
    }

    if (points_y > 10) {
      printf("Player 2 won");
      running = false;
    }
      
    ball_xy.x += dx * dt;
    ball_xy.y += dy * dt;


    // RENDER
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_FRect left_plyr = {paddle_first.x, paddle_first.y, paddle_first.w, paddle_first.h};
    SDL_FRect right_plyr = {paddle_second.x, paddle_second.y, paddle_second.w, paddle_first.h};
    SDL_FRect ball = {ball_xy.x, ball_xy.y, ball_xy.w, ball_xy.h};
    
    SDL_RenderFillRect(renderer, &left_plyr);
    SDL_RenderFillRect(renderer, &right_plyr);
    SDL_RenderFillRect(renderer, &ball);
    
    SDL_RenderPresent(renderer);

    // FRAME CAP
    Uint64 elapsed = SDL_GetTicksNS() - frame_start;

    if (elapsed < FRAME_NS) {
      SDL_DelayNS(FRAME_NS - elapsed);
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  
  return 0;
}

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <stdio.h>

constexpr int height = 600;
constexpr int width = 800;

constexpr int rect_width = 5;
constexpr int rect_height = 20;

SDL_FRect* create_rect(int x, int y, int w, int h) {
  SDL_FRect *rect;
  rect = malloc(sizeof(SDL_FRect));

  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
}

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log(SDL_GetError());
    return 1;
  }

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (!SDL_CreateWindowAndRenderer("PONG", width, height, 0, &window, &renderer)) {
    SDL_Log("Window creation failed %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  bool running = true;
  while (running) {
    SDL_Event event;

    SDL_FRect* left_plyr = create_rect(100, 1, rect_width, rect_height);
    SDL_FRect* right_plyr = create_rect(100, 580, rect_width, rect_height);
    SDL_FRect* ball = create_rect(200, 200, rect_width, rect_width);

    SDL_FRect* rects[] = {left_plyr, right_plyr, ball};

    for (int i = 0; i < 3; i++) {
      bool success = SDL_RenderRect(renderer, rects[i]);
    }
    
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
	running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

float deltaTime(Uint32 lastTime) {
  return SDL_GetTicks() - lastTime;
}

void movePaddle(SDL_Rect* paddle, float amount, bool direction, unsigned int windowHeight, Uint32 lastTime) {
  paddle->y += (direction ? -amount : amount) * deltaTime(lastTime);

  // Clamp values to window boundaries
  if (paddle->y < 0)
    paddle->y = 0;
  if (paddle->y > windowHeight - paddle->h)
    paddle->y = windowHeight - paddle->h;
  // SDL_Log("%d", paddle->y);
}

int main() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initilized\n");
  } else {
    printf("SDL video sytem loaded\n");
  }

  const int TARGETFPS = 60;

  unsigned int windowWidth, windowHeight;
  windowWidth = 1200;
  windowHeight = 800;

  SDL_Window* window = SDL_CreateWindow("SDL Pong", 600, 200, windowWidth, windowHeight, SDL_WINDOW_SHOWN); // Create window
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); //Create renderer

  SDL_Rect paddle1, paddle2, ball;

  unsigned int paddleOffset = 100;

  //Set paddle coordinates and dimensions
  paddle1.x = paddleOffset;
  paddle2.x = windowWidth - paddleOffset;
  paddle1.y = paddle2.y = windowHeight / 2;
  paddle1.w = paddle2.w = 10;
  paddle1.h = paddle2.h = 100;

  //Set ball coordinates and dimensions
  ball.h = ball.w = 16;
  ball.x = windowWidth/2 - ball.w;
  ball.y = windowHeight/2;

  char gameIsRunning = true;

  while (gameIsRunning) {
    Uint32 lastTime = SDL_GetTicks();
    SDL_Event event;
    bool movingUp, movingDown;

    while (SDL_PollEvent(&event)) {
      SDL_KeyCode key = event.key.keysym.sym;
      switch (event.type) {
        case SDL_QUIT:
          gameIsRunning = false;
          break;

        case SDL_KEYDOWN:
          if (key == SDLK_w || key == SDLK_UP) {
            // SDL_Log("Up pressed");
            movingUp = true;
            movingDown = false;
          }
          if (key == SDLK_s || key == SDLK_DOWN) {
            // SDL_Log("Down pressed");
            movingDown = true;
            movingUp = false;
          }
          break;

        case SDL_KEYUP:
          if (key == SDLK_w || key == SDLK_UP) {
              // SDL_Log("Up released");
              movingUp = false;
            }
            if (key == SDLK_s || key == SDLK_DOWN) {
              // SDL_Log("Down released");
              movingDown = false;
            }
          break;
      }
    }
    
    if (movingDown) {
      movePaddle(&paddle1, 15.0f, 0, windowHeight, lastTime);
    }

    if (movingUp) {
      movePaddle(&paddle1, 15.0f, 1, windowHeight, lastTime);
    }

    //DRAW TO SCREEN
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //White
    SDL_RenderFillRect(renderer, &paddle1);
    SDL_RenderFillRect(renderer, &paddle2);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); //Gray
    // Draw dotted line
    SDL_Rect dot;
    dot.w = 16;
    dot.h = 16;
    dot.x = windowWidth/2 - dot.w;
    dot.y = 0;

    unsigned int numDots = 20;

    for (int i = 0; i <= numDots; ++i) {
      SDL_RenderFillRect(renderer, &dot);
      dot.y += (windowHeight - dot.h) / numDots;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //White
    SDL_RenderFillRect(renderer, &ball);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Black
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
  }

  SDL_DestroyWindow(window);

  SDL_Quit();


  return 0;
}
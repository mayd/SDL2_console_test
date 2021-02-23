/* hello_sdl2.c */

#include <stdio.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// This simple C program tests the operation of console input and output for an SDL2 graphics application.
// It tests reading from stdin, output to stdout and output to stderr.
// Refer to Makefile for correct linker options needed to make this work under Microsoft Windows.

static char *copyright = "This program uses TrueType font \"froufrou\" from <https://www.dafont.com/froufrou.font>.\n"
                         "See file froufrou.txt for information about copyright and conditions of use.";
static char text[82];

static int InputThread(void *ptr) {
  while(1) {
    printf("Enter text (max 80 chars): "); fflush(stdout);
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;
    if (strlen(text) > 80) {
      fprintf(stderr, "Warning: input text exceeds 80 characters and will be truncated.\n"); fflush(stderr);
      text[79] = 0;
    } else {
      fprintf(stderr, "Info: length of input text is %d characters.\n", strlen(text)); fflush(stderr);
    }
    fflush(stdin);
    // printf("Read text: '%s' from stdin.\n", text); fflush(stdout);
    SDL_PushEvent((SDL_Event *)ptr);
  }
}

int main(int argc, char *args[]) {
  Uint32 USER_EVENT_TYPE1;
  SDL_Event txt_event;
  SDL_Thread *thread;
  int width, height;

  printf("%s\n\n", copyright);  fflush(stdout);

  printf("Load TrueType font from file froufrou.ttf.\n"); fflush(stdout);
  TTF_Init();
  TTF_Font *font = TTF_OpenFont("froufrou.ttf", 20);
  printf("Create display for text '%s'.\n", text); fflush(stdout);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("SDL Console output example",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderClear(renderer);

  USER_EVENT_TYPE1 = SDL_RegisterEvents(1);
  SDL_memset(&txt_event, 0, sizeof(txt_event));
  txt_event.type = USER_EVENT_TYPE1;
  txt_event.user.data1 = NULL;
  txt_event.user.data1 = NULL;
  printf("Create user input thread.\n", text); fflush(stdout);
  thread = SDL_CreateThread(InputThread, "InputThread", (void *)&txt_event);

  printf("Start event loop.\n"); fflush(stdout);
  while (1) {
    SDL_Event event;

    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT) {
      printf("Event type SDL_QUIT occurred.\n"); fflush(stdout);
      break;
    }
    if (event.type == USER_EVENT_TYPE1) {
      printf("Event type USER_EVENT_TYPE1 occurred.\n"); fflush(stdout);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderClear(renderer);
      SDL_Color colour = {0, 0, 0};
      SDL_Surface *surface = TTF_RenderText_Solid(font, text, colour);
      SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_QueryTexture(texture, NULL, NULL, &width, &height);  
      SDL_Rect rect = {10, 10, width, height};
      SDL_RenderCopy(renderer, texture, NULL, &rect);
      SDL_RenderPresent(renderer);
      SDL_DestroyTexture(texture);
      SDL_FreeSurface(surface);
      SDL_RaiseWindow(window);
    }
  }
  printf("Exited event loop.\n"); fflush(stdout);

  printf("Release resources.\n"); fflush(stdout);
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  printf("Finished.\n"); fflush(stdout);
  return 0;
}

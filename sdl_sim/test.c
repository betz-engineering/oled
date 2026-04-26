// demo the `psu_board_gui` on a PC using SDL2
#include "demo.h"
#include "font.h"
#include "frame_buffer.h"
#include "ui_board.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define ZOOM 2

SDL_Renderer *rr = NULL;
SDL_Window *window = NULL;

// To show the display pixels in blue (persistent between frames)
SDL_Texture *layer_a;

void send_frame_buffer() {
  // Show pixel values in blue in background layer
  SDL_SetRenderTarget(rr, layer_a);
  for (int y = 0; y < FB_HEIGHT; y++) {
    for (int x = 0; x < FB_WIDTH; x++) {
      uint8_t p = get_pixel(x, y);
      SDL_SetRenderDrawColor(rr, p, p, p, 0xFF);
      SDL_RenderDrawPoint(rr, x, y);
    }
  }
}

static void init_sdl() {
  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize SDL2: %s\n", SDL_GetError());
    return;
  }

  if (SDL_CreateWindowAndRenderer(FB_WIDTH * ZOOM, FB_HEIGHT * ZOOM, 0, &window,
                                  &rr)) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return;
  };

  layer_a = SDL_CreateTexture(rr, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, FB_WIDTH, FB_HEIGHT);

  SDL_SetTextureBlendMode(layer_a, SDL_BLENDMODE_NONE);
}

// Mock user interface
int ticks = 0;
unsigned event_flags = 0;
int get_encoder_ticks(bool reset) { return ticks; }

unsigned get_event_flags(void) { return event_flags; }

void set_leda(unsigned rgb_value) { printf("set_leda(%x)\n", rgb_value); }
void set_ledb(unsigned rgb_value) { printf("set_ledb(%x)\n", rgb_value); }
void set_inverted(bool val) { printf("set_inverted(%x)\n", val); };
void ui_init(t_ui_board_type val) { printf("ui_init(%d)\n", val); }

// Get raw MCP23 GPIO input values
uint16_t get_gpios(void) { return 0; }

void _putchar(char c) { putc(c, stdout); }

int main(int argc, char *args[]) {
  init_sdl();

  while (1) {
    SDL_Event e;
    bool isExit = false;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        isExit = true;
        break;

      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
          ticks--;
          event_flags |= EV_ROT_CCW;
          break;

        case SDLK_RIGHT:
          ticks++;
          event_flags |= EV_ROT_CW;
          break;

        case SDLK_DOWN:
          event_flags |= EV_ENC_S;
          break;

        case SDLK_UP:
          event_flags |= EV_BACK_S;
          break;
        }
        break;
      }
    }
    if (isExit)
      break;

    demo();
    send_frame_buffer();

    // Compose the layers
    SDL_SetRenderTarget(rr, NULL); // default backbuffer
    SDL_RenderSetScale(rr, ZOOM, ZOOM);
    SDL_RenderClear(rr);
    SDL_RenderCopy(rr, layer_a, NULL, NULL);
    SDL_RenderPresent(rr);

    event_flags = 0;
    SDL_Delay(30);
  }

  SDL_DestroyRenderer(rr);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

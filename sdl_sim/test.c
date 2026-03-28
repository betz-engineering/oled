// demo the `psu_board_gui` on a PC using SDL2
#include "demo.h"
#include "frame_buffer.h"
#include "lv_font.h"
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
SDL_Texture *layer_bg;

// To show the update windows (cleared each frame)
SDL_Texture *layer_fg;

// x1, y1, x2, y2: the rectangle to update in [pixels]
// x1, y1, x2 and y2 are all inclusive!
// note that ssd1322 works with columns of 4 pixels horizontally
// so the lower 2 bits of x1 and x2 will be truncated
// data in 4 bits / pixel, 2 pixels / byte
bool send_window_4(unsigned x1, unsigned y1, unsigned x2, unsigned y2,
                   uint8_t *data) {
  // printf("send_window_4(%3d, %3d, %3d, %3d)\n", x1, y1, x2, y2);

  // truncate the 2 LSBs
  x1 = x1 & ~3;
  x2 = (x2 & ~3);

  // Show pixel values in blue in background layer
  SDL_SetRenderTarget(rr, layer_bg);
  for (int y = y1; y <= y2; y++) {
    for (int x = x1; x <= x2; x++) {
      int p = getPixel(x, y);
      SDL_SetRenderDrawColor(rr, 0, 0, (p << 4) | p, 0xFF);
      SDL_RenderDrawPoint(rr, x, y);
    }
  }

  // Show update windows in red in foreground layer
  SDL_SetRenderTarget(rr, layer_fg);
  SDL_Rect rect;
  SDL_SetRenderDrawColor(rr, 0xFF, 0x00, 0x00, 0x80);
  rect.x = x1;
  rect.y = y1;
  rect.w = x2 - x1 + 1;
  rect.h = y2 - y1 + 1;
  SDL_RenderDrawRect(rr, &rect);

  return true;
}

static void init_sdl() {
  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize SDL2: %s\n", SDL_GetError());
    return;
  }

  if (SDL_CreateWindowAndRenderer(DISPLAY_WIDTH * ZOOM, DISPLAY_HEIGHT * ZOOM,
                                  0, &window, &rr)) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return;
  };

  layer_bg =
      SDL_CreateTexture(rr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                        DISPLAY_WIDTH, DISPLAY_HEIGHT);

  layer_fg =
      SDL_CreateTexture(rr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                        DISPLAY_WIDTH, DISPLAY_HEIGHT);

  SDL_SetTextureBlendMode(layer_fg, SDL_BLENDMODE_BLEND);
  SDL_SetTextureBlendMode(layer_bg, SDL_BLENDMODE_NONE);
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
    send_partial_fb();

    // Compose the 2 layers
    SDL_SetRenderTarget(rr, NULL); // default backbuffer
    SDL_RenderSetScale(rr, ZOOM, ZOOM);
    SDL_RenderClear(rr);
    SDL_RenderCopy(rr, layer_bg, NULL, NULL);
    SDL_RenderCopy(rr, layer_fg, NULL, NULL);
    SDL_RenderPresent(rr);

    // Clear foreground layer for next frame
    SDL_SetRenderTarget(rr, layer_fg);
    SDL_SetRenderDrawColor(rr, 0, 0, 0, 0);
    SDL_RenderClear(rr);
    event_flags = 0;

    SDL_Delay(30);
  }

  SDL_DestroyRenderer(rr);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

#ifndef SGL2_HPP
#define SGL2_HPP

#include "sgl2.h"

namespace sgl2 {

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a):
	red(r < 0 ? 0 : (r > 255) ? 255 : r),
	green(g < 0 ? 0 : (g > 255) ? 255 : g),
	blue(b < 0 ? 0 : (b > 255) ? 255 : b),
	alpha(a < 0 ? 0 : (a > 255) ? 255 : a){};

Window::Window(const string& title, int width, int height)
    : background_color_(Color(0, 0, 0, 0)) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) return;
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window_,
                                    &renderer_))
        return;
    SDL_SetWindowTitle(window_, title.c_str());
}

void Window::Run() {
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key);
                    break;
            }
        }

        // TODO: break drawing code into other methods
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer_);

        SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer_, 1, 1);

        SDL_RenderPresent(renderer_);
    }
}

void Window::KeyPressed(KeyboardEvent& key) {}
}
#endif
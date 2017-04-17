#ifndef SGL2_HPP
#define SGL2_HPP

#include "sgl2.h"

namespace sgl2 {

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a):
	red(r < 0 ? 0 : (r > 255) ? 255 : r),
	green(g < 0 ? 0 : (g > 255) ? 255 : g),
	blue(b < 0 ? 0 : (b > 255) ? 255 : b),
	alpha(a < 0 ? 0 : (a > 255) ? 255 : a){};


//--------------BASE WINDOW CLASS-----------------------//
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

Window::Window(const string& title, int width, int height, const Color& background)
    : background_color_(background) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) return;
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window_,
                                    &renderer_))
        return;
    SDL_SetWindowTitle(window_, title.c_str());
}

void Window::SetColor(const Color& color)
{
    background_color_ = color;
}

void Window::PrePaint() {
    SetColor(background_color_);
    SDL_RenderClear(renderer_);
}

void Window::PostPaint() {
    SDL_RenderPresent(renderer_);
    invalid_ = false;
}

void Window::PaintAll()
{
    PrePaint();
    Paint();
    PostPaint();
}

void Window::Repaint()
{
    invalid_ = true;
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

        if (invalid_)
        {
            PaintAll();
        }
    }
}

void Window::KeyPressed(KeyboardEvent& key) {}

//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//


//--------------OBJECT WINDOW CLASS-----------------------//
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//



}
#endif
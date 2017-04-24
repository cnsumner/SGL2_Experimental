#ifndef SGL2_HPP
#define SGL2_HPP

#include "sgl2.h"

namespace sgl2 {

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    : red(r < 0 ? 0 : (r > 255) ? 255 : r),
      green(g < 0 ? 0 : (g > 255) ? 255 : g),
      blue(b < 0 ? 0 : (b > 255) ? 255 : b),
      alpha(a < 0 ? 0 : (a > 255) ? 255 : a){};

//--------------BASE WINDOW CLASS-----------------------//
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

Window::Window(const string& title, int width, int height,
               const Color& background)
    : invalid_(true), background_color_(background) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) return;
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window_,
                                    &renderer_))
        return;
    SDL_SetWindowTitle(window_, title.c_str());
}

void Window::SetColor(const Color& color) { background_color_ = color; }

void Window::PrePaint() {
    SetColor(background_color_);
    SDL_RenderClear(renderer_);
}

void Window::PostPaint() {
    SDL_RenderPresent(renderer_);
    invalid_ = false;
}

void Window::PaintAll() {
    PrePaint();
    Paint();
    PostPaint();
}

void Window::Repaint() { invalid_ = true; }

void Window::DrawPoint(Color color, int x, int y) {
    SDL_SetRenderDrawColor(renderer_, color.red, color.green, color.blue,
                           color.alpha);
    SDL_RenderDrawPoint(renderer_, x, y);
}

void Window::DrawRectangle(Color color, const SDL_Rect& rect) {
    SDL_SetRenderDrawColor(renderer_, color.red, color.green, color.blue,
                           color.alpha);
    SDL_RenderDrawRect(renderer_, &rect);
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

        if (invalid_) {
            PaintAll();
        }
    }
}

void Window::KeyPressed(KeyboardEvent& key) {}

//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

//--------------GRAPHICAL OBJECT CLASS------------------//
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

GraphicalObject::GraphicalObject(ObjectWindow* window, Color color)
    : window_(window), color_(color), id_(id_source_++) {}

GraphicalObject::GraphicalObject(ObjectWindow* window, Color color, int x,
                                 int y)
    : window_(window), color_(color), posx_(x), posy_(y), id_(id_source_++) {}

int GraphicalObject::GetPosX() { return posx_; }

int GraphicalObject::GetPosY() { return posy_; }

void GraphicalObject::SetPosX(int x) { posx_ = x; }

void GraphicalObject::SetPosY(int y) { posx_ = y; }

unsigned GraphicalObject::id_source_ = 0;

//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

//--------------POINT CLASS-----------------------------//
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

Point::Point(ObjectWindow* window, Color color, int x, int y)
    : GraphicalObject(window, color, x, y) {}

void Point::Paint() { window_->DrawPoint(color_, posx_, posy_); }

//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

//--------------RECTANGLE CLASS-------------------------//
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

Rectangle::Rectangle(ObjectWindow* window, Color color, int x, int y, int width,
                     int height)
    : GraphicalObject(window, color) {
        rect_ = SDL_Rect{x, y, width, height};
    }

void Rectangle::Paint() {
    window_->DrawRectangle(color_, rect_);
}

int Rectangle::GetPosX() {
    return rect_.x;
}

int Rectangle::GetPosY() {
    return rect_.y;
}

void Rectangle::SetPosX(int x) {
    rect_.x = x;
}

void Rectangle::SetPosY(int y) {
    rect_.y = y;
}

//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

//--------------OBJECT WINDOW CLASS---------------------//
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//

ObjectWindow::ObjectWindow(const string& title, int width, int height,
                           const Color& background)
    : Window(title, width, height, background) {}

void ObjectWindow::PrePaint() { Window::PrePaint(); }

void ObjectWindow::Paint() {}

void ObjectWindow::PostPaint() {
    for (auto& o : objects_) o->Paint();
    Window::PostPaint();
}

void ObjectWindow::Add(GraphicalObject* obj) { objects_.push_back(obj); }

//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//
}
#endif
#ifndef SGL2_HPP
#define SGL2_HPP

#include "sgl2.h"
#include <algorithm>

namespace sgl2 {

	Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
		: red(r < 0 ? 0 : (r > 255) ? 255 : r),
		green(g < 0 ? 0 : (g > 255) ? 255 : g),
		blue(b < 0 ? 0 : (b > 255) ? 255 : b),
		alpha(a < 0 ? 0 : (a > 255) ? 255 : a) {};

	//--------------BASE WINDOW CLASS-----------------------//
	//------------------------------------------------------//
	//------------------------------------------------------//
	//------------------------------------------------------//

	Window::Window(const string& title, int width, int height,
		const Color& background)
		: invalid_(true), background_color_(background), running_(false) {
		if (SDL_Init(SDL_INIT_EVERYTHING)) return;
		if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window_,
			&renderer_))
			return;
		SDL_SetWindowTitle(window_, title.c_str());
	}

	void Window::SetBackgroundColor(const Color& color) { background_color_ = color; }

	void Window::PrePaint() {
		SDL_SetRenderDrawColor(renderer_, background_color_.red, background_color_.green, background_color_.blue, background_color_.alpha);
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

	void Window::Update() {}

	void Window::Run() {
		running_ = true;
		while (running_) {
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					running_ = false;
					break;
				case SDL_KEYDOWN:
					KeyPressed(event.key);
					break;
				case SDL_MOUSEMOTION:
					MouseMoved(event.motion);
					break;
				case SDL_MOUSEBUTTONDOWN:
					MousePressed(event.button);
					break;
				case SDL_MOUSEBUTTONUP:
					MouseReleased(event.button);
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
					case SDL_WINDOWEVENT_ENTER:
						MouseEntered();
						break;
					case SDL_WINDOWEVENT_LEAVE:
						MouseExited();
						break;
					}
					break;
				}
			}

			Update();

			if (invalid_) {
				PaintAll();
			}
		}
	}

	void Window::Quit()
	{
		running_ = false;
	}

	void Window::KeyPressed(const KeyboardEvent & event) {}

	inline void Window::MouseMoved(const MouseMotionEvent & event) {}

	inline void Window::MousePressed(const MouseButtonEvent & event) {}

	inline void Window::MouseReleased(const MouseButtonEvent & event) {}

	inline void Window::MouseEntered() {}

	inline void Window::MouseExited() {}

	//------------------------------------------------------//
	//------------------------------------------------------//
	//------------------------------------------------------//

	//--------------GRAPHICAL OBJECT CLASS------------------//
	//------------------------------------------------------//
	//------------------------------------------------------//
	//------------------------------------------------------//

	GraphicalObject::GraphicalObject(ObjectWindow* window, Color color)
		: window_(window), color_(color), id_(id_source_++) {
		rect_ = SDL_Rect{ 0, 0, 0, 0 };
	}

	GraphicalObject::GraphicalObject(ObjectWindow* window, Color color, int x,
		int y)
		: window_(window), color_(color), id_(id_source_++) {
		rect_ = SDL_Rect{ x, y, 0, 0 };
	}

	int GraphicalObject::GetPosX() { return rect_.x; }

	int GraphicalObject::GetPosY() { return rect_.y; }

	void GraphicalObject::SetPosX(int x) { rect_.x = x; }

	void GraphicalObject::SetPosY(int y) { rect_.y = y; }

	int GraphicalObject::GetWidth()
	{
		return rect_.w;
	}

	int GraphicalObject::GetHeight()
	{
		return rect_.h;
	}

	void GraphicalObject::SetWidth(int width)
	{
		rect_.w = width;
	}

	void GraphicalObject::SetHeight(int height)
	{
		rect_.h = height;
	}

	bool GraphicalObject::Hit(int x, int y)
	{
		//TODO: check if coords are in bounds
		return true;
	}

	//TODO: add docs
	//collision code written from scratch (no outside sources consulted, so it may have a bug)
	bool GraphicalObject::Collision(GraphicalObject* obj)
	{
		GraphicalObject* rect1 = this;
		GraphicalObject* rect2 = obj;

		if (rect1->GetPosX() + rect1->GetWidth() >= rect2->GetPosX() &&
			rect1->GetPosX() <= rect2->GetPosX() + rect2->GetWidth() &&
			rect1->GetPosY() + rect1->GetHeight() >= rect2->GetPosY() &&
			rect1->GetPosY() <= rect2->GetPosY() + rect2->GetHeight()) {
			return true;
		}

		return false;
	}

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

	void Point::Paint() { window_->DrawPoint(color_, rect_.x, rect_.y); }

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
		rect_ = SDL_Rect{ x, y, width, height };
	}

	void Rectangle::Paint() {
		window_->DrawRectangle(color_, rect_);
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

	void ObjectWindow::Remove(GraphicalObject * obj)
	{
		objects_.erase(std::remove(objects_.begin(), objects_.end(), obj), objects_.end());
	}

	GraphicalObject* ObjectWindow::GetFirstHit(int x, int y)
	{
		for (GraphicalObject* o : objects_)
		{
			if (o->Hit(x, y))
				return o;
		}
		return nullptr;
	}

	//------------------------------------------------------//
	//------------------------------------------------------//
	//------------------------------------------------------//
}
#endif
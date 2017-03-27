#ifndef SGL2_HPP_INCLUDED_
#define SGL2_HPP_INCLUDED_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <exception>

// Include the SGL2 declarations
#include "sgl2.h"

// Provide the SGL2 implementation
namespace sgl2 {

//--------------------------------------------------------------
// WindowInitializationException method implementations
WindowInitializationException::WindowInitializationException(const std::string& msg): message(msg){}

const char *WindowInitializationException::what() const throw() { 
    return message.c_str(); 
}


//--------------------------------------------------------------
// Color struct constructor implementation
Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a): 
      red(r < 0 ? 0 : (r > 255) ? 255 : r), 
      green(g < 0 ? 0 : (g > 255) ? 255 : g), 
      blue(b < 0 ? 0 : (b > 255) ? 255 : b), 
      alpha(a < 0 ? 0 : (a > 255) ? 255 : a) {};


//--------------------------------------------------------------
// Definitions of the global Color objects
const Color BLACK(0, 0, 0);
const Color RED(255, 0, 0);
const Color GREEN(0, 255, 0);
const Color BLUE(0, 0, 255);
const Color YELLOW(255, 255, 0);
const Color MAGENTA(255, 0, 255);
const Color CYAN(0, 255, 255);
const Color DARK_RED(191, 0, 0);
const Color DARK_GREEN(0, 191, 0);
const Color DARK_BLUE(0, 0, 191);
const Color LIGHT_RED(255, 191, 191);
const Color LIGHT_GREEN(191, 255, 191);
const Color LIGHT_BLUE(191, 191, 255);
const Color GRAY(128, 128, 128);
const Color LIGHT_GRAY(204, 204, 204);
const Color DARK_GRAY(51, 51, 51);
const Color WHITE(255, 255, 255);


//--------------------------------------------------------------
// Function that returns the keyboard state (shift, ctrl, alt)
KeyModifier get_keyboard_state() {
    return static_cast<KeyModifier>(SDL_GetModState());
}


static Uint32 sgl_timer_expired(Uint32 interval, void *param) {
    //std::cout << "In sgl_timer_expired: " << param << '\n';
    Window *win = reinterpret_cast<Window *>(param);
	return win->timer_expired();
}


//--------------------------------------------------------------
// Window class methods

Window::Window(const std::string& title, int x, int y, int width, int height,
       const Color& background):
       background_color(background), repaint_flag(true) {
    //std::cout << "Making a Window" << std::endl;
    // Initialize SDL system
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        throw WindowInitializationException("Failed to initialize SDL : "s + SDL_GetError());
    // Create window
    window = SDL_CreateWindow(title.c_str(), x, y, width, height, 0);
    if (!window)
        throw WindowInitializationException("Failed to create window : "s + SDL_GetError());
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
        throw WindowInitializationException("Failed to create renderer : "s + SDL_GetError());
    // Set size of renderer to the same as window
    SDL_RenderSetLogicalSize(renderer, width, height);
    // Set color of renderer to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //std::cout << "Made a Window" << std::endl;
}

Window::Window(): Window("Program"s, 100, 100, 800, 600, WHITE) {}

int Window::get_x() const {
    int x, y;
    SDL_GetWindowPosition(window, &x, &y);
    return x;
}

int Window::get_y() const {
    int x, y;
    SDL_GetWindowPosition(window, &x, &y);
    return y;
}

int Window::get_width() const {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return w;
}

int Window::get_height() const {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return h;
}

void Window::mouse_moved(const MouseMotionEvent& ev) {}

void Window::mouse_pressed(const MouseButtonEvent& ev) {}

void Window::mouse_released(const MouseButtonEvent& ev) {}

void Window::mouse_entered() {}

void Window::mouse_exited() {}

void Window::key_pressed(const KeyboardEvent& key) {}

void Window::pre_paint() {
    //std::cout << "Painting" << std::endl;
    // Set background color
    set_color(background_color);
    // Clear the window and set its background color
    SDL_RenderClear(renderer);
}

void Window::post_paint() {
    // Render the changes from the painting
    SDL_RenderPresent(renderer);
    repaint_flag = false;
}

void Window::paint_all() {
    if (repaint_flag) {
        //std::cout << "Painting ALL" << std::endl;
        pre_paint();
        paint();
        post_paint();
    }
}

void Window::repaint() {
    repaint_flag = true;
}

void Window::run() {
    bool loop = true;
    while (loop) {
        //std::cout << "In event loop" << std::endl;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:    
                    loop = false;
                    break;
                case SDL_KEYDOWN: 
                    key_pressed(event.key); 
                    break;
                case SDL_MOUSEMOTION: 
                    mouse_moved(event.motion); 
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    mouse_pressed(event.button); 
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_released(event.button);
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_ENTER:
                            mouse_entered();
                            break;
                        case SDL_WINDOWEVENT_LEAVE:
                            mouse_exited();
                            break;
                    }
                    break;
            }
        }
        if (repaint_flag)
            paint_all();
    }
}

void Window::draw_point(int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

void Window::fill_rectangle(const Rectangle& rect) {
    //set_color(color);
    SDL_RenderFillRect(renderer, &rect);
}

void Window::fill_rectangle(int x, int y, int width, int height) {
    Rectangle rect {x, y, width, height};
    SDL_RenderFillRect(renderer, &rect);
}

void Window::draw_rectangle(const Rectangle& rect) {
    //set_color(color);
    SDL_RenderDrawRect(renderer, &rect);
}

void Window::draw_rectangle(int x, int y, int width, int height) {
    Rectangle rect {x, y, width, height};
    SDL_RenderDrawRect(renderer, &rect);
}

// Adapted from 
// http://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles 
void Window::fill_circle(int x, int y, int radius) {
    for (int edge_y = -radius; edge_y <= radius; edge_y++)
        for (int edge_x = -radius; edge_x <= radius; edge_x++)
            if (edge_x*edge_x + edge_y*edge_y <= radius*radius)
                draw_point(x + edge_x, y + edge_y); 
}

// Draws eight points on the rim of a circle reflected from
// one point across eight octants.
// (edge_x, edge_y) is a point on the rim.
// (center_x, center_y) is the circle's center.
// Adapted from 
// Computer Graphics: The Principles behind the Art and Science
// Cornel Pokorny and Curtis Gerald (c) 1989
inline void Window::eight_octs(int edge_x, int edge_y, 
                               int center_x, int center_y) {
    draw_point(edge_x + center_x, edge_y + center_y);
    draw_point(-edge_x + center_x, edge_y + center_y);
    draw_point(edge_x + center_x, -edge_y + center_y);
    draw_point(-edge_x + center_x, -edge_y + center_y);
    draw_point(edge_y + center_x, edge_x + center_y);
    draw_point(-edge_y + center_x, edge_x + center_y);
    draw_point(edge_y + center_x, -edge_x + center_y);
    draw_point(-edge_y + center_x, -edge_x + center_y);
}

// Implements Bresenham's circle generation algorithm to draw
// the an arc on a circle from 90 degrees to 45 degrees.  Calls
// eight_octs function to render the complete circle.
// (x, y) is the circle's center.
// radius is the circle's radius.
// Adapted from 
// Computer Graphics: The Principles behind the Art and Science
// Cornel Pokorny and Curtis Gerald (c) 1989
void Window::draw_circle(int x, int y, int radius) {
    // Start at 90 degrees
    int current_x = 0, current_y =radius, s = 3 - 2*radius;
    while (current_x < current_y) {  // Stop at 45 degrees
        eight_octs(current_x, current_y, x, y);
        if (s <= 0)
            s = s + 4*current_x + 6;
        else {
            s = s + 4*(current_x - current_y) + 10;
            current_y--;
        }
        current_x++;
    }
    if (current_x == current_y)
        eight_octs(current_x, current_y, x, y);
}

void Window::draw_line(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Window::set_color(const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.red, color.green,
                           color.blue, color.alpha);
}
void Window::set_color(unsigned r, unsigned g, unsigned b, unsigned a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a); 
}

void Window::start_timer(int msec) {
    //SDL_TimerID timer_id = SDL_AddTimer(msec, sgl_timer_expired, this);
    //std::cout << "Starting timer.  window pointer = " << this << '\n';
    timer_id = SDL_AddTimer(msec, sgl_timer_expired, this);
}

void Window::stop_timer() {
    //std::cout << "Stopping timer." << '\n';
    SDL_RemoveTimer(timer_id);
}

void Window::delay(int msec) {
    SDL_Delay(msec);
}

int Window::timer_expired() { return 0; }


//--------------------------------------------------------------
// Global message box function
void show_message_box(const std::string& title, 
                        const std::string& message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             title.c_str(), message.c_str(), 0);
}


//--------------------------------------------------------------
// ObjectWindow class method implementations


/*
 *  Constructor
 *      Creates a graphics window with a its left-top corner at (left, top) in 
 *      screen coordinates.  Its width is width, and its height is height.  The viewport
 *      it represents extends from min_x to max_y horizontally and from min_y to max_y vertically.
 */
ObjectWindow::ObjectWindow(const std::string& title, 
                                int left, int top, 
                                int width, int height, 
				                int min_x, int max_x, 
                                int min_y, int max_y):
                Window(title, left, top, width, height),
                       //min_x, max_x, min_y, max_y),
                active_object(nullptr) {} 

ObjectWindow::ObjectWindow(const std::string& title, 
                                int width, int height):
                Window(title, 10, 10, width, height),
                active_object(nullptr) {}

ObjectWindow::ObjectWindow(const std::string& title, 
                                int min_x, int max_x, 
                                int min_y, int max_y):
                Window(title, min_x, max_x, min_y, max_y),
                active_object(nullptr) {}

ObjectWindow::ObjectWindow(): Window() {}


ObjectWindow::~ObjectWindow() {
	//window_list[glutGetWindow()] = nullptr;
	object_list.clear();
}

void ObjectWindow::pre_paint() {
    Window::pre_paint();
}

/**
 *  post_paint paints all the graphical objects
 *  this window manages
 */
void ObjectWindow::post_paint() {
    for (auto& p : object_list)
        p->paint();
    Window::post_paint();
}

void ObjectWindow::mouse_pressed(const MouseButtonEvent& ev) {
    if (active_object) {
        active_object->mouse_pressed(ev);
        repaint();
    }
    Window::mouse_pressed(ev);
}

void ObjectWindow::mouse_released(const MouseButtonEvent& ev) {
    if (active_object) {
        active_object->mouse_released(ev);
    }
    Window::mouse_released(ev);
    repaint();
}

void ObjectWindow::mouse_moved(const MouseMotionEvent& ev) {
    //std::cout << "In ObjectWindow::mouse_moved" << std::endl;
    if (active_object && ev.state != NO_BUTTON_DOWN) {
        active_object->mouse_dragged(ev);
    }
    else {
        GraphicalObject *prev_active = active_object;
        active_object = hit(ev.x, ev.y);
        if (prev_active != active_object) {
            if (prev_active) {
                prev_active->set_mouse_over(false);
                prev_active->set_cursor(CURSOR_ARROW);
            }
            if (active_object) {
                active_object->set_mouse_over(true);
                active_object->mouse_moved(ev);
                active_object->set_cursor(active_object->get_cursor());
            }
        }
    }
    repaint();
}

//void ObjectWindow::mouse_dragged(const MouseMotionEvent& ev) {
//    if (active_object) {
//        active_object->mouse_dragged(ev);
//    }
//    repaint();
//}

void ObjectWindow::key_pressed(const KeyboardEvent& ev) {
    if (active_object) {
        active_object->key_pressed(ev);
        repaint();
    }
    Window::key_pressed(ev);
}


// Add the given graphical object obj to the display list
void ObjectWindow::add(GraphicalObject *obj) {
    //std::cout << "In add" << std::endl;
    object_list.push_back(obj);
	obj->set_window(this);
}

//  Remove the given graphical object obj from the display list
void ObjectWindow::remove(GraphicalObject *obj) {
    std::cout << "remove not implemented" << std::endl;
    //object_list.remove(obj);
    //object_list.erase(std::remove_if(object_list.begin(), 
    //                   object_list.end(), 
    //                   [](GraphicalObject *elem) { return elem == obj; }),
    //                   object_list.end());
	repaint();
}

//  Remove all the graphical objects from the display list
//  Frees up the memory allocated for the object
void ObjectWindow::remove_all() {
	for (auto& p : object_list)
        delete p;
    object_list.clear();
	repaint();
}

//  Return the first graphical object in the display list that is hit
//  by the given point (x,y).  If no objects in the list intersect the 
//  given point, return null.
GraphicalObject *ObjectWindow::hit(int x, int y) const {
    for (auto p = rbegin(object_list); p != rend(object_list); p++)
        if ((*p)->hit(x, y))
            return *p;
    return nullptr;   //  No object hit
}

/**
 *  Returns an iterator to the begining of the vector of 
 *  graphical objects this window contains.
 *  @return  an iterator to the begining of the vector of 
 *           graphical objects this window contains.
 */
//std::vector<GraphicalObject *>::iterator ObjectWindow::begin()
auto ObjectWindow::begin() -> decltype(std::begin(object_list)) {
    //return object_list.begin();
    return std::begin(object_list);
}

/**
 *  Returns an iterator just past the end of the vector of 
 *  graphical objects this window contains.
 *  @return  an iterator just past the end of the vector of 
 *           graphical objects this window contains.
 */
//std::vector<GraphicalObject *>::iterator ObjectWindow::end()
auto ObjectWindow::end() -> decltype(std::end(object_list)) {
    //return object_list.end();
    return std::end(object_list);
}


//--------------------------------------------------------------
// Graphical object implementation code

//  Constructor initializes the bounding box of the graphical object and
//  assigns a unique ID number to the object.
GraphicalObject::GraphicalObject(int lf, int bm, 
                                       int wd, int ht):
                           window(nullptr), left(lf), top(bm), 
						   width(wd), height(ht), 
                           cursor(CURSOR_CROSSHAIR),
                           mouse_over(false), id(id_source++) {
	//cout << "Making a graphical object: ";
	//cout << "left = " << left << ", top = " << top << endl;
}

//  Copy constructor makes a new graphical object with the same
//  characteristics except it has a unique ID
GraphicalObject::GraphicalObject(const GraphicalObject &go):
                 left(go.left), top(go.top),
                 width(go.width), height(go.height),
                 cursor(CURSOR_CROSSHAIR),
                 mouse_over(false), id(id_source++) {}

//  Assignment operator copies all the graphical object
//  characteristics but does not affect the object's ID
GraphicalObject& GraphicalObject::operator=
                  (const GraphicalObject &go) {
    left = go.left;
    top = go.top;
    width = go.width;
    height = go.height;
    cursor = go.cursor;
    return *this;
}

//  Destructor currently does nothing
GraphicalObject::~GraphicalObject() {
	//cout << "Destroying graphical object #" << id << endl;
}



int GraphicalObject::get_left() const {
	return left;
}

int GraphicalObject::get_top() const {
	return top;
}

int GraphicalObject::get_width() const {
	return width;
}

int GraphicalObject::get_height() const {
	return height;
}

/**
 *  Sets the location and size of the graphical object.
 *  Values given are in the containing viewport's coordinates.
 *  @x the x coordinate of the left-top corner of the 
 *  graphical object's bounding rectangle.
 *  @y the y coordinate of the left-top corner of the 
 *  graphical object's bounding rectangle.
 *  @width the graphical object's width in viewport coordinates.
 *  @height the graphical object's height in viewport coordinates.
 *  @return nothing
 */
void GraphicalObject::set(int x, int y, 
                               int width, int height) {
    left = x;
    top = y;
    this->width = width;
    this->height = height;
}



//  Determines if the point (x,y) falls within the bounding box of this 
//  graphical object?
bool GraphicalObject::hit(int x, int y) {
    if (x >= left && x <= left + width 
            && y >= top && y <= top + height) {
        x_hit_offset = x - left;  y_hit_offset = y - top;
        return true;
    }
    return false;
}

/**
 *  Called by the containing window's mouse_dragged method
 *  when the user drags the mouse
 *  when the mouse pointer is over the graphical object.
 *  @param x the x coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param y the y coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @return nothing
 */

void GraphicalObject::mouse_dragged(const MouseMotionEvent& ev) {
    //left = x - x_hit_offset;   
    //top = y - y_hit_offset;
    std::cout << "Moving a graphical object to "
              << ev.x << "," << ev.y << std::endl;
    move_to(ev.x - x_hit_offset, ev.y - y_hit_offset);
    //move_to(ev.x, ev.y);
}

void GraphicalObject::move_to(int left, int top) {
    this->left = left;   
    this->top = top;
    //std::cout << "***" << std::endl;
}

void GraphicalObject::set_mouse_over(bool flag) {
    mouse_over = flag;
}


/**
 *  Called by the containing window's mouse_pressed method
 *  when the user depresses any mouse
 *  button when the mouse pointer is over the graphical object.
 *  @param x the x coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param y the y coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param button the button that was depressed, left (LEFT_BUTTON) 
 *         or right (RIGHT_BUTTON) during the current mouse event.
 *  @return nothing
 */
void GraphicalObject::mouse_pressed(const MouseButtonEvent&) {}

/**
 *  Called by the containing window's mouse_released method
 *  when the user releases any mouse
 *  button when the mouse pointer is over the graphical object.
 *  @param x the x coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param y the y coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param button the button that was depressed, left (LEFT_BUTTON) 
 *         or right (RIGHT_BUTTON) during the current mouse event.
 *  @return nothing
 */
void GraphicalObject::mouse_released(const MouseButtonEvent&) {}

/**
 *  Called by the containing window's mouse_moved method
 *  when the user moves the mouse
 *  when the mouse pointer is over the graphical object.
 *  @param x the x coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param y the y coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @return nothing
 */
void GraphicalObject::mouse_moved(const MouseMotionEvent& ev) {}
//    if (ev.state & LEFT_MOUSE_DOWN)
//        //move_to(ev.x, ev.y);
//        move_to(ev.x - x_hit_offset, ev.y - y_hit_offset);
//
//}


/**
 *  Called by the containing window when the user types a key
 *  when the window has keyboard focus and the mouse pointer
 *  is over the graphical object.  The key pressed and
 *  the location of the mouse pointer in viewport coordinates 
 *  is reported.  
 *  @param k the key typed by the user 
 *  @param x the x coordinate in viewport coordinates of the 
 *           location of the mouse pointer within the window
 *  @param y the y coordinate in viewport coordinates of the 
 *           location of the mouse pointer within the window
 *  @return nothing
 */
void GraphicalObject::key_pressed(const KeyboardEvent&) {}



Window *GraphicalObject::set_window(ObjectWindow *win) {
	Window *previous = window;
    window = win;
	return previous;
}

Window *GraphicalObject::get_window() const {
	return window;
}

 /**
  *  Sets the graphical object's cursor shape.
  *  @param cursor the object's cursor shape
  *  @return the previous cursor shape
  */
CursorShape GraphicalObject::set_cursor(CursorShape cursor) {
    CursorShape prev_cursor = this->cursor;
    this->cursor = cursor;
    return prev_cursor;
}

/**
 *  Returns the cursor shape associated with this graphical
 *  object. 
 *  @return the cursor shape associated with this graphical
 *          object
 */
CursorShape GraphicalObject::get_cursor() {
    return cursor;
}

void GraphicalObject::draw_point(int x, int y) {
    window->draw_point(x, y);
}

void GraphicalObject::fill_rectangle(const Rectangle& rect) {
    window->fill_rectangle(rect);
}

void GraphicalObject::fill_rectangle(int x, int y, int width, int height) {
    window->fill_rectangle(x, y, width, height);
}

void GraphicalObject::draw_rectangle(const Rectangle& rect) {
    window->draw_rectangle(rect);
}

void GraphicalObject::draw_rectangle(int x, int y, int width, int height) {
    window->draw_rectangle(x, y, width, height);
}

// Adapted from 
// http://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles 
void GraphicalObject::fill_circle(int x, int y, int radius) {
    window->fill_circle(x, y, radius);
}

void GraphicalObject::draw_circle(int x, int y, int radius) {
    window->draw_circle(x, y, radius);
}

void GraphicalObject::draw_line(int x1, int y1, int x2, int y2) {
    window->draw_line(x1, y1, x2, y2);
}

void GraphicalObject::set_color(const Color& color) {
    window->set_color(color);
}
void GraphicalObject::set_color(unsigned r, unsigned g, unsigned b, unsigned a) {
    window->set_color(r, g, b, a);
}


unsigned GraphicalObject::id_source = 0;


} // End of sgl2 namespace

#endif

#ifndef SGL2_WINDOW_HPP_
#define SGL2_WINDOW_HPP_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <exception>

using namespace std::string_literals;

namespace sgl2 {

class WindowInitializationException : public std::exception {
    std::string message;
public:
    WindowInitializationException(const std::string& msg): message(msg){}

    const char *what() const throw() { return message.c_str(); }
};

using Point = SDL_Point;
using Rectangle = SDL_Rect;
using MouseMotionEvent = SDL_MouseMotionEvent;
using MouseButtonEvent = SDL_MouseButtonEvent;
using KeyboardEvent = SDL_KeyboardEvent;

/**
 *  A Color object represents an RGB color.
 *  Color objects are immutable.
 */
class Color {
public:
	/**  The red component of the color  */
	Uint8 red;
	/**  The green component of the color  */
	Uint8 green;
	/**  The blue component of the color  */
	Uint8 blue;
	/**  The alpha channel of the color  */
	Uint8 alpha;

	/**  Initializes a Color object.  The constructor
	 *   enforces clamped RGB values (0...255). 
     *   @param r the red component of the color
	 *   @param g the green component of the color
	 *   @param b the blue component of the color
	 *   @param a the alpha channel of the color
	 */
	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255): 
	           red(r < 0 ? 0 : (r > 255) ? 255 : r), 
			   green(g < 0 ? 0 : (g > 255) ? 255 : g), 
			   blue(b < 0 ? 0 : (b > 255) ? 255 : b), 
			   alpha(a < 0 ? 0 : (a > 255) ? 255 : a) {};
};

/*****************************************************
 *  Global constant Color objects
 *****************************************************/

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

enum MouseModifier { 
    NO_BUTTON_DOWN    = 0, 
    LEFT_MOUSE_DOWN   = SDL_BUTTON_LMASK,
    RIGHT_MOUSE_DOWN  = SDL_BUTTON_RMASK,
    MIDDLE_MOUSE_DOWN = SDL_BUTTON_MMASK
};


enum KeyCodes { 
    A_KEY = SDLK_a,
    B_KEY,
    C_KEY,
    D_KEY,
    E_KEY,
    F_KEY,
    G_KEY,
    H_KEY,
    I_KEY,
    J_KEY,
    K_KEY,
    L_KEY,
    M_KEY,
    N_KEY,
    O_KEY,
    P_KEY,
    Q_KEY,
    R_KEY,
    S_KEY,
    T_KEY,
    U_KEY,
    V_KEY,
    W_KEY,
    X_KEY,
    Y_KEY,
    Z_KEY,
    F1_KEY = SDLK_F1, 
    //F1_KEY = 10001, 
    F2_KEY, 
    F3_KEY, 
    F4_KEY, 
    F5_KEY, 
    F6_KEY, 
    F7_KEY, 
    F8_KEY, 
    F9_KEY, 
    F10_KEY, 
    F11_KEY, 
    F12_KEY,
    LEFT_KEY = SDLK_LEFT, 
    UP_KEY = SDLK_UP, 
    RIGHT_KEY = SDLK_RIGHT, 
    DOWN_KEY = SDLK_DOWN, 
    PAGE_UP_KEY = SDLK_PAGEUP, 
    PAGE_DOWN_KEY = SDLK_PAGEDOWN, 
    HOME_KEY = SDLK_HOME, 
    END_KEY = SDLK_END, 
    INSERT_KEY = SDLK_INSERT 
};

/**
 *  KeyModifer is an enumerated type that maps directly to the
 *  SDL2 equivalents for the corresponding key modifers: 
 *  SHIFT, CRTL, and ALT.
 *  These can be combined with bitwise OR to represent combinations 
 *  such as CTRL-ALT.
 */
enum KeyModifier { 
    NONE_DOWN        = KMOD_NONE, //0, 
    LEFT_SHIFT_DOWN  = KMOD_LSHIFT, //1,
    RIGHT_SHIFT_DOWN = KMOD_RSHIFT, //2,
    SHIFT_DOWN       = LEFT_SHIFT_DOWN | RIGHT_SHIFT_DOWN,
    LEFT_CTRL_DOWN   = KMOD_LCTRL, //64,
    RIGHT_CTRL_DOWN  = KMOD_RCTRL, //128,
    CTRL_DOWN        = LEFT_CTRL_DOWN | RIGHT_CTRL_DOWN, 
    LEFT_ALT_DOWN    = KMOD_LALT, //256,
    RIGHT_ALT_DOWN   = KMOD_RALT, //512,
    ALT_DOWN         = LEFT_ALT_DOWN | RIGHT_ALT_DOWN, 
};

/**
 *  Cursor shapes map directly to the SDL2 equivalents.
 */
enum CursorShape { 
    CURSOR_ARROW     = SDL_SYSTEM_CURSOR_ARROW,
    CURSOR_IBEAM     = SDL_SYSTEM_CURSOR_IBEAM,
    CURSOR_WAIT      = SDL_SYSTEM_CURSOR_WAIT,
    CURSOR_CROSSHAIR = SDL_SYSTEM_CURSOR_CROSSHAIR,
    CURSOR_SIZEALL   = SDL_SYSTEM_CURSOR_SIZEALL,
    CURSOR_HAND      = SDL_SYSTEM_CURSOR_HAND
};

KeyModifier get_keyboard_state() {
    return static_cast<KeyModifier>(SDL_GetModState());
}


class Window {
protected:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Color background_color;
    bool repaint_flag;
public:
    Window(const std::string& title, int x, int y, int width, int height,
           const Color& background=WHITE):
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

    Window(): Window("Program"s, 100, 100, 800, 600, WHITE) {}

    virtual void mouse_moved(const MouseMotionEvent& ev) {}

    virtual void mouse_pressed(const MouseButtonEvent& ev) {}

    virtual void mouse_released(const MouseButtonEvent& ev) {}

    virtual void mouse_entered() {}

    virtual void mouse_exited() {}

    virtual void key_pressed(const KeyboardEvent& key) {}

    virtual void pre_paint() {
        //std::cout << "Painting" << std::endl;
        // Set background color
        set_color(background_color);
        // Clear the window and set its background color
        SDL_RenderClear(renderer);
    }

    virtual void post_paint() {
        // Render the changes from the painting
        SDL_RenderPresent(renderer);
        repaint_flag = false;
    }

    void paint_all() {
        if (repaint_flag) {
            //std::cout << "Painting ALL" << std::endl;
            pre_paint();
            paint();
            post_paint();
        }
    }

    virtual void paint() = 0;

    void repaint() {
        repaint_flag = true;
    }

    void run() {
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

    void fill_rectangle(const Rectangle& rect) {
        //set_color(color);
        SDL_RenderFillRect(renderer, &rect);
    }

    void fill_rectangle(int x, int y, int width, int height) {
        Rectangle rect {x, y, width, height};
        SDL_RenderFillRect(renderer, &rect);
    }

    void draw_rectangle(const Rectangle& rect) {
        //set_color(color);
        SDL_RenderDrawRect(renderer, &rect);
    }

    void draw_rectangle(int x, int y, int width, int height) {
        Rectangle rect {x, y, width, height};
        SDL_RenderDrawRect(renderer, &rect);
    }

    void draw_line(int x1, int y1, int x2, int y2) {
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    void set_color(const Color& color) {
        SDL_SetRenderDrawColor(renderer, color.red, color.green,
                               color.blue, color.alpha);
    }
    void set_color(unsigned r, unsigned g, unsigned b, unsigned a) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a); 
    }
};

// Forward reference
class ObjectWindow;



/**
 *   Represents a persistent graphical object used within a window.
 *   A graphical object is owned by a particular window.
 */
class GraphicalObject {
private:
    /**  Used to generate a unique ID for each graphical object  */
    static unsigned id_source;

protected:
    /**  The window in which this graphical object is a component  */
    ObjectWindow *window;

    /**  Location of the mouse pointer's x coordinate relative to the graphical
      *  object's bounding box  */
    int x_hit_offset;
	
	/**  Location of the mouse pointer's y coordinate relative to the graphical
      *  object's bounding box  */
	int y_hit_offset;

	/**  x coordinate of the left-top corner of the graphical 
	 *   object's bounding box  */
    int left;
	
    /**  y coordinate of the left-top corner of the graphical 
	 *   object's bounding box  */
	int top;

	/**  Width of the graphical object's bounding box in
	 *   the viewport coordinate system  */
    int width;
	
	/**  Height of the graphical object's bounding box in
	 *   the viewport coordinate system  */
	int height;

    /**  Shape that the cursor should assume when the mouse
     *   hovers over this graphical object. */
    CursorShape cursor;

    /**  Indicates if this object is the one that the mouse
     *   is currently hovering over.  */
    bool mouse_over;

public:
    /**  This graphical object's unique ID  */
    const unsigned id;

	/**  Constructor specifies the bounding box extent.  
	 *   @param left the x coordinate in viewport coordinates
	 *               of the left-top corner of the
	 *               graphical object's bounding box
     *   @param left the y coordinate in viewport coordinates 
	 *               of the left-top corner of the
	 *               graphical object's bounding box
	 *   @param width the width in viewport coordinates of the graphical 
	 *                object's bounding box
	 *   @param height the height in viewport coordinates of the graphical 
	 *                object's bounding box
     *   \pre{The left value must be greater than 0.}
     *   \pre{The top value must be greater than 0.}
     *   \pre{The width must be greater than 0.}
     *   \pre{The height must be greater than 0.}
     *   \post{A GraphicalObject will be created with the given parameters.}
     *   \code{.cpp}
     *      GraphicalObject *obj = new GraphicalObject(0, 0, 1920, 1080);
     *   \endcode
	 */
    GraphicalObject(int left, int top, int width, 
                    int height);

	/**  Copy construction creates a new graphical object with
	 *   identical location.  The new graphical object has the same
	 *   containing window but has a unique id.  The pre-existing 
	 *   graphical object is not affected by the copy.
	 *   @param other the graphical object from which this new graphical
	 *   object is being created.
     *   \pre{The object the method is being called on must be initialized to a non-null value.}
     *   \post{A GraphicalObject will be created with the given parameters.}
     *   \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 0, 1920, 1080);
     *
     *      GraphicalObject *obj1 = new GraphicalObject(obj0);
     *   \endcode  	 
     */
    GraphicalObject(const GraphicalObject& other);

	/**  Assignment copies the location, size, and window container from 
	 *   another graphical object.  The id of the window being assigned 
	 *   is not affected.  The source graphical object is not affected 
	 *   by the copy.
	 *   @param other the source graphical object from which this graphical
	 *   object is being assigned.
	 *   @return a reference to this graphical object as per standard
	 *           assignment operator sementics
     *   \post{A GraphicalObject will be assigned from the given parameters.}
     *   \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 0, 1920, 1080);
     *
     *      GraphicalObject obj1 = *obj0;
     *   \endcode
	 */
    GraphicalObject& operator=(const GraphicalObject& other);

	/**
	 *  Destroys a graphical object.
     *  \pre{The declared object must be non-null or previously deleted.}
     *  \post{A GraphicalObject will be created with the given default parameters.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 0, 1920, 1080);
     *
     *      delete obj0;
     *  \endcode
	 */
    virtual ~GraphicalObject();

	/**
	 *  Returns the x value of the left-top corner of the 
	 *  graphical object's bounding box.  The value is given 
	 *  in the viewport coordinate system.
	 *  @return the x coordinate of the graphical object's
	 *          left-top corner
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      
     *      int left = obj0->get_left();
     *      // left should be 0;
     *  \endcode
	 */
	virtual int get_left() const;

	/**
	 *  Returns the y value of the left-top corner of the 
	 *  graphical object's bounding box.  The value is given 
	 *  in the viewport coordinate system.
	 *  @return the y coordinate of the graphical object's
	 *          left-top corner
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      
     *      int top = obj0->get_top();
     *      // top should be 5;
     *  \endcode
	 */
	virtual int get_top() const;

	/**
	 *  Returns the width in viewport coordinates of the 
	 *  graphical object's bounding box.
	 *  @return the width of the graphical object
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      
     *      int width = obj0->get_width();
     *      // width should be 1920;
     *  \endcode
	 */
	virtual int get_width() const;

	/**
	 *  Returns the height in viewport coordinates of the 
	 *  graphical object's bounding box.
	 *  @return the height of the graphical object
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      
     *      int height = obj0->get_height();
     *      // height should be 1080;
     *  \endcode
	 */
	virtual int get_height() const;

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
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \pre{The left value must be greater than 0.}
     *  \pre{The top value must be greater than 0.}
     *  \pre{The width must be greater than 0.}
     *  \pre{The height must be greater than 0.}
     *  \post{The object will be set to the given x and y coordinate with the given width and height.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      
     *      obj0->set(50, 80, 1080, 720);
     *  \endcode
	 */
	virtual void set(int x, int y, int width, int height);


	/**
	 *  The paint method is called by the containing window's 
	 *  paint method when it renders this graphical object.
	 *  This method is abstract, so concrete derived classes must
     *  provide a concrete implementation.
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      
     *      obj0->paint();
     *  \endcode
	 */
    virtual void paint() const = 0;


	/**
	 *  Relocates the left-top corner of the graphical object's
	 *  bounding box to the specified location.  The graphical object's
	 *  width and height are unaffected.
	 *  @param left the x coordinate in viewport coordinates
	 *               of the left-top corner of the
	 *               graphical object's bounding box
	 *  @param top the y coordinate in viewport coordinates
	 *               of the left-top corner of the
	 *               graphical object's bounding box
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \pre{The left value must be greater than 0.}
     *  \pre{The top value must be greater than 0.}
     *  \post{The object will be moved to the given x and y coordinates.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      
     *      obj0->move_to(50, 80);
     *  \endcode
	 */  
    virtual void move_to(int left, int top);

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
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	virtual void mouse_pressed(const MouseButtonEvent& ev);

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
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	virtual void mouse_released(const MouseButtonEvent& ev);

	/**
	 *  Called by the containing window's mouse_moved method
     *  when the user moves the mouse
	 *  when the mouse pointer is over the graphical object.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	virtual void mouse_moved(const MouseMotionEvent& ev);

	/**
	 *  Called by the containing window's mouse_dragged method
     *  when the user drags the mouse
	 *  By default it relocates the (left,top) location of 
     *  the graphical object's bounding box relative to 
     *  a mouse drag event.  
     *  The move is relative to the location of the last
	 *  mouse event on the object.  This allows the user to drag the
	 *  graphical object without it "jumping" slightly at first movement.
	 *  The graphical object's width and height are unaffected.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	virtual void mouse_dragged(const MouseMotionEvent& ev);

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
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	virtual void key_pressed(const KeyboardEvent& key);



	/**
	 *  Returns true if the coordinate provided is within the graphical
	 *  object's bounding box; otherwise, it returns false.
	 *  @param x the x coordinate in viewport coordinates of the
	 *           location to check
	 *  @param y the y coordinate in viewport coordinates of the
	 *           location to check
	 *  @return true, if (x,y) intersects the bounding box of the graphical
	 *          object; otherwise it returns false
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      
     *      bool hit = obj0->hit(1000, 80);
     *      // hit will be true
     *
     *      hit = obj0->hit(2, 4);
     *      // hit will be false
     *  \endcode
	 */
    virtual bool hit(int x, int y);

	/**
	 *  Set the containing window.
	 *  @param win a pointer to the window to make the containing 
	 *             window for this graphical object.
	 *  @return a pointer to the previous owner of this window
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \pre{The window to be set to the object must be initialized to a non-null value.}
     *  \post{The objects containing window will be set to the window passed in.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *      ObjectWindow *window = ObjectWindow("Window", 200, 300, 1080, 720, 200, 1280, 300, 1020);
     *      
     *      obj0->set_window(window);
     *  \endcode
	 */
	virtual Window *set_window(ObjectWindow *win);

	/**
	 *  Get the containing window of this graphical object.
	 *  @return a pointer to the window that contains this 
	 *          graphical object.
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *
     *      ObjectWindow *window = obj0->get_window();
     *  \endcode
	 */
	virtual Window *get_window() const;

	/**
	 *  Activates or deactivates the graphical object
     *  @param flag if true, activates the object; otherwise,
                    deactivates the object
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \post{The object will be activated or deactivated, depending on the input.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *
     *      // activate the object
     *      obj0->set_mouse_over(true);
     *
     *      // deactivate the object
     *      obj0->set_mouse_over(false);
     *  \endcode
	 */
	virtual void set_mouse_over(bool flag);

    /**
     *  Sets the graphical object's cursor shape.
     *  @param cursor the object's cursor shape
     *  @return the previous cursor shape
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \pre{The cursor must be an initialized to a non-null value.}
     *  \post{The object's cursor will be set to the given shape.}
     */
    virtual CursorShape set_cursor(CursorShape cursor);

    /**
     *  Returns the cursor shape associated with this graphical
     *  object. 
     *  @return the cursor shape associated with this graphical
     *          object
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *
     *      CursorShape shape = obj0->get_cursor();
     *  \endcode
     */
    virtual CursorShape get_cursor();
};

/**
 *  The class representing window objects that can contain
 *  graphical objects that the user can manipulate.
 */
class ObjectWindow: public Window {
protected:
	/**  The list of graphical objects contained in this window.  */
    std::vector<GraphicalObject *> object_list;
    GraphicalObject *active_object;

public:
	/**
	 *  Constructor that provides the most control to the client.
	 *  Creates a window with detailed information.
     *  @param title the text to appear within the window's titlebar
	 *  @param left the x coordinate in screen coordinates of the window's 
	 *         left-top corner 
	 *  @param top the y coordinate in screen coordinates of the window's 
	 *         left-top corner  
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 *  @param min_x the smallest x value representing the left-most position
	 *               within the viewport
	 *  @param max_x the largest x value representing the right-most position
	 *               within the viewport
	 *  @param min_y the smallest y value representing the top-most position
	 *               within the viewport
	 *  @param max_y the largest y value representing the top-most position
	 *               within the viewport
     *  \pre{The left value must be greater than 0.}
     *  \pre{The top value must be greater than 0.}
     *  \pre{The width must be greater than 0.}
     *  \pre{The height must be greater than 0.}
     *  \pre{The min_x must be greater than 0.}
     *  \pre{The min_y must be greater than 0.}
     *  \pre{The max_x must be greater than min_x.}
     *  \pre{The max_y must be greater than min_y.}
     *  \post{An ObjectWindow will be created with the given parameters.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow("Window", 0, 5, 1920, 1080, 0, 1920, 5, 1085);
     *  \endcode
	 */
	ObjectWindow(const std::string& title, int left, int top, int width, 
                 int height, int min_x, int max_x, 
                 int min_y, int max_y);

	/**
	 *  Creates a window of a specified width and height
     *  @param title the text to appear within the window's titlebar
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
     *  \pre{The width must be greater than 0.}
     *  \pre{The height must be greater than 0.}
     *  \post{An ObjectWindow will be created with the given parameters.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow("Window", 1920, 1080);
     *  \endcode
	 */
	ObjectWindow(const std::string& title, int width, int height);


	/**
	 *  Creates a window with a default position and size.
     *  @param title the text to appear within the window's titlebar
	 *  @param min_x the smallest x value representing the left-most position
	 *               within the viewport
	 *  @param max_x the largest x value representing the right-most position
	 *               within the viewport
	 *  @param min_y the smallest y value representing the top-most position
	 *               within the viewport
	 *  @param max_y the largest y value representing the top-most position
	 *               within the viewport
     *  \pre{The min_x must be greater than 0.}
     *  \pre{The min_y must be greater than 0.}
     *  \pre{The max_x must be greater than min_x.}
     *  \pre{The max_y must be greater than min_y.}
     *  \post{An ObjectWindow will be created with the given parameters.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow("Window", 0, 1920, 5, 1085);
     *  \endcode
	 */
	ObjectWindow(const std::string& title, int min_x, int max_x, 
                 int min_y, int max_y);

	/**
	 *  Create a default, plain window
     *  \post{An ObjectWindow will be created with the default parameters.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *  \endcode
	 */
	ObjectWindow();

	/**
	 *  Destroys a graphical window object.
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \post{The object will be deleted.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *
     *      delete window;
     *  \endcode
	 */
	virtual ~ObjectWindow();

	/**  
	 *  Code executed before the call to paint
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *
     *      window->pre_paint();
     *  \endcode
	 */
	void pre_paint() override;

	/**
	 *  Code executed after the call to paint
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *
     *      window->post_paint();
     *  \endcode
	 */
	void post_paint() override;

	/**
	 *  Called by the event loop when the user depresses any mouse
	 *  button when the mouse pointer is within the window.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param button the button that was depressed, left (LEFT_BUTTON) 
	 *         or right (RIGHT_BUTTON) during the current mouse event.
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	void mouse_pressed(const MouseButtonEvent& ev) override;

	/**
	 *  Called by the event manager when the user releases any mouse
	 *  button when the mouse pointer is within the window.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param button the button that was released, left (LEFT_BUTTON) 
	 *         or right (RIGHT_BUTTON) during the current mouse event.
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	void mouse_released(const MouseButtonEvent& ev) override;

	/**
	 *  Called by the event manager when the user moves the mouse
	 *  when the mouse pointer is within the window.  The location
	 *  reported is given in viewport coordinates.
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	void mouse_moved(const MouseMotionEvent& ev) override;

	// --> /**
	// -->  *  Called by the event manager when the user drags the mouse
	// -->  *  (moves the mouse while holding down a mouse button)
	// -->  *  when the mouse pointer is within the window.  The location
	// -->  *  reported is given in viewport coordinates.
	// -->  *  @param x the x coordinate in viewport coordinates of the 
	// -->  *           location of the mouse pointer within the window during
	// -->  *           the most recent mouse event.
	// -->  *  @param y the y coordinate in viewport coordinates of the 
	// -->  *           location of the mouse pointer within the window during
	// -->  *           the most recent mouse event.
	// -->  *  @return nothing
	// -->  */
	// --> virtual void mouse_dragged(const MouseMotionEvent& ev);

	/**
	 *  Called by the event manager when the user types a key
	 *  when the window has keyboard focus.  The key pressed and
	 *  the location of the mouse pointer in viewport coordinates 
	 *  is reported.  
	 *  @param k the key typed by the user 
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
	 */
	void key_pressed(const KeyboardEvent& ev) override;

	/**
	 *  Adds a graphical object to the window.
	 *  @param obj the graphical object to add to this window
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \pre{The object being passed in must be initialized to a non-null value.}
     *  \post{The object will be added to the given window.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *
     *      window->add(obj0);
     *  \endcode
	 */
	void add(GraphicalObject *obj);

	/**  
	 *  Removes a graphical object from the window.
	 *  @param obj the graphical object to remove from the window.
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \pre{The object being passed in must be initialized to a non-null value.}
     *  \post{The object will be removed from the given window.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *
     *      window->add(obj0);
     *      window->remove(obj0);
     *  \endcode
	 */
    void remove(GraphicalObject *obj);

	/**
	 *  Removes all the graphical objects from the list of
	 *  contained graphical objects.  Frees up the memory 
	 *  allocated for the object.
	 *  @return nothing
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \post{All objects in the given window will be removed.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *      GraphicalObject *obj0 = new GraphicalObject(0, 5, 1920, 1080);
     *
     *      window->add(obj0);
     *      window->remove_all();
     *  \endcode
	 */
	void remove_all();

	/**
	 *  Returns the first graphical object in the list of
	 *  graphical objects with a bounding box that intersects the 
	 *  given viewport coordinates.
	 *  @param x the x coordinate in viewport coordinates of a
	 *           location within the viewport
	 *  @param y the y coordinate in viewport coordinates of a
	 *           location within the viewport
	 *  @return a contained graphical object with a bounding box 
	 *          that intersects the given position within the 
	 *          viewport.  Returns null if no contained graphical
	 *          object intersects the given point
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *
     *      bool hit = window->hit(100, 300);
     *      // hit will be true
     *
     *      bool hit = window->hit(100, 4);
     *      // hit will be false
     *  \endcode
	 */
    GraphicalObject *hit(int x, int y) const;

    /**
     *  Returns an iterator to the begining of the vector of 
     *  graphical objects this window contains.
     *  @return  an iterator to the begining of the vector of 
     *           graphical objects this window contains.
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *
     *      std::vector<GraphicalObject *>::iterator ittr = window->begin();
     *  \endcode
     */
    std::vector<GraphicalObject *>::iterator begin();

    /**
     *  Returns an iterator just past the end of the vector of 
     *  graphical objects this window contains.
     *  @return  an iterator just past the end of the vector of 
     *           graphical objects this window contains.
     *  \pre{The object the method is being called on must be initialized to a non-null value.}
     *  \code{.cpp}
     *      ObjectWindow *window = new ObjectWindow();
     *
     *      std::vector<GraphicalObject *>::iterator ittr = window->end();
     *  \endcode
     */
    std::vector<GraphicalObject *>::iterator end();
};


// Global functions

void show_message_box(const std::string& title, 
                        const std::string& message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             title.c_str(), message.c_str(), 0);

}



} // End of sgl2 namespace


/*****************************************************
 *  ObjectWindow code
 *****************************************************/

/*
 *  Constructor
 *      Creates a graphics window with a its left-top corner at (left, top) in 
 *      screen coordinates.  Its width is width, and its height is height.  The viewport
 *      it represents extends from min_x to max_y horizontally and from min_y to max_y vertically.
 */
sgl2::ObjectWindow::ObjectWindow(const std::string& title, 
                                int left, int top, 
                                int width, int height, 
				                int min_x, int max_x, 
                                int min_y, int max_y):
                sgl2::Window(title, left, top, width, height),
                       //min_x, max_x, min_y, max_y),
                active_object(nullptr) {} 

sgl2::ObjectWindow::ObjectWindow(const std::string& title, 
                                int width, int height):
                sgl2::Window(title, 10, 10, width, height),
                active_object(nullptr) {}

sgl2::ObjectWindow::ObjectWindow(const std::string& title, 
                                int min_x, int max_x, 
                                int min_y, int max_y):
                sgl2::Window(title, min_x, max_x, min_y, max_y),
                active_object(nullptr) {}

sgl2::ObjectWindow::ObjectWindow(): sgl2::Window() {}


sgl2::ObjectWindow::~ObjectWindow() {
	//window_list[glutGetWindow()] = nullptr;
	object_list.clear();
}

void sgl2::ObjectWindow::pre_paint() {
    Window::pre_paint();
}

/**
 *  post_paint paints all the graphical objects
 *  this window manages
 */
void sgl2::ObjectWindow::post_paint() {
    for (auto& p : object_list)
        p->paint();
    Window::post_paint();
}

void sgl2::ObjectWindow::mouse_pressed(const MouseButtonEvent& ev) {
    if (active_object) {
        active_object->mouse_pressed(ev);
        repaint();
    }
    sgl2::Window::mouse_pressed(ev);
}

void sgl2::ObjectWindow::mouse_released(const MouseButtonEvent& ev) {
    if (active_object) {
        active_object->mouse_released(ev);
    }
    sgl2::Window::mouse_released(ev);
    repaint();
}

void sgl2::ObjectWindow::mouse_moved(const MouseMotionEvent& ev) {
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

//void sgl2::ObjectWindow::mouse_dragged(const MouseMotionEvent& ev) {
//    if (active_object) {
//        active_object->mouse_dragged(ev);
//    }
//    repaint();
//}

void sgl2::ObjectWindow::key_pressed(const KeyboardEvent& ev) {
    if (active_object) {
        active_object->key_pressed(ev);
        repaint();
    }
    sgl2::Window::key_pressed(ev);
}


// Add the given graphical object obj to the display list
void sgl2::ObjectWindow::add(GraphicalObject *obj) {
    //std::cout << "In add" << std::endl;
    object_list.push_back(obj);
	obj->set_window(this);
}

//  Remove the given graphical object obj from the display list
void sgl2::ObjectWindow::remove(GraphicalObject *obj) {
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
void sgl2::ObjectWindow::remove_all() {
	for (auto& p : object_list)
        delete p;
    object_list.clear();
	repaint();
}

//  Return the first graphical object in the display list that is hit
//  by the given point (x,y).  If no objects in the list intersect the 
//  given point, return null.
sgl2::GraphicalObject *sgl2::ObjectWindow::hit(int x, int y) const {
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
//std::vector<sgl2::GraphicalObject *>::iterator sgl2::ObjectWindow::begin()
auto sgl2::ObjectWindow::begin() -> decltype(std::begin(object_list)) {
    //return object_list.begin();
    return std::begin(object_list);
}

/**
 *  Returns an iterator just past the end of the vector of 
 *  graphical objects this window contains.
 *  @return  an iterator just past the end of the vector of 
 *           graphical objects this window contains.
 */
//std::vector<sgl2::GraphicalObject *>::iterator sgl2::ObjectWindow::end()
auto sgl2::ObjectWindow::end() -> decltype(std::end(object_list)) {
    //return object_list.end();
    return std::end(object_list);
}

/*****************************************************
 *  Graphical object code
 *****************************************************/

//  Constructor initializes the bounding box of the graphical object and
//  assigns a unique ID number to the object.
sgl2::GraphicalObject::GraphicalObject(int lf, int bm, 
                                       int wd, int ht):
                           window(0), left(lf), top(bm), 
						   width(wd), height(ht), 
                           cursor(sgl2::CURSOR_CROSSHAIR),
                           mouse_over(false), id(id_source++) {
	//cout << "Making a graphical object: ";
	//cout << "left = " << left << ", top = " << top << endl;
}

//  Copy constructor makes a new graphical object with the same
//  characteristics except it has a unique ID
sgl2::GraphicalObject::GraphicalObject(const GraphicalObject &go):
                 left(go.left), top(go.top),
                 width(go.width), height(go.height),
                 cursor(sgl2::CURSOR_CROSSHAIR),
                 mouse_over(false), id(id_source++) {}

//  Assignment operator copies all the graphical object
//  characteristics but does not affect the object's ID
sgl2::GraphicalObject& sgl2::GraphicalObject::operator=
                  (const sgl2::GraphicalObject &go) {
    left = go.left;
    top = go.top;
    width = go.width;
    height = go.height;
    cursor = go.cursor;
    return *this;
}

//  Destructor currently does nothing
sgl2::GraphicalObject::~GraphicalObject() {
	//cout << "Destroying graphical object #" << id << endl;
}



int sgl2::GraphicalObject::get_left() const {
	return left;
}

int sgl2::GraphicalObject::get_top() const {
	return top;
}

int sgl2::GraphicalObject::get_width() const {
	return width;
}

int sgl2::GraphicalObject::get_height() const {
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
void sgl2::GraphicalObject::set(int x, int y, 
                               int width, int height) {
    left = x;
    top = y;
    this->width = width;
    this->height = height;
}



//  Determines if the point (x,y) falls within the bounding box of this 
//  graphical object?
bool sgl2::GraphicalObject::hit(int x, int y) {
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

void sgl2::GraphicalObject::mouse_dragged(const MouseMotionEvent& ev) {
    //left = x - x_hit_offset;   
    //top = y - y_hit_offset;
    std::cout << "Moving a graphical object to "
              << ev.x << "," << ev.y << std::endl;
    move_to(ev.x - x_hit_offset, ev.y - y_hit_offset);
    //move_to(ev.x, ev.y);
}

void sgl2::GraphicalObject::move_to(int left, int top) {
    this->left = left;   
    this->top = top;
    std::cout << "***" << std::endl;
}

void sgl2::GraphicalObject::set_mouse_over(bool flag) {
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
void sgl2::GraphicalObject::mouse_pressed(const sgl2::MouseButtonEvent&) {}

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
void sgl2::GraphicalObject::mouse_released(const sgl2::MouseButtonEvent&) {}

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
void sgl2::GraphicalObject::mouse_moved(const MouseMotionEvent& ev) {}
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
void sgl2::GraphicalObject::key_pressed(const sgl2::KeyboardEvent&) {}



sgl2::Window *sgl2::GraphicalObject::set_window(sgl2::ObjectWindow *win) {
	Window *previous = window;
    window = win;
	return previous;
}

sgl2::Window *sgl2::GraphicalObject::get_window() const {
	return window;
}

 /**
  *  Sets the graphical object's cursor shape.
  *  @param cursor the object's cursor shape
  *  @return the previous cursor shape
  */
sgl2::CursorShape sgl2::GraphicalObject::set_cursor(sgl2::CursorShape cursor) {
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
sgl2::CursorShape sgl2::GraphicalObject::get_cursor() {
    return cursor;
}


unsigned sgl2::GraphicalObject::id_source = 0;


#undef main

#endif

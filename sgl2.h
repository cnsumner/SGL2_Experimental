#ifndef SGL2_H_INCLUDED_
#define SGL2_H_INCLUDED_

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
    WindowInitializationException(const std::string& msg);

    const char *what() const throw();
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
	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
};


/*****************************************************
 *  Global constant Color objects
 *****************************************************/

extern const Color BLACK;
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color YELLOW;
extern const Color MAGENTA;
extern const Color CYAN;
extern const Color DARK_RED;
extern const Color DARK_GREEN;
extern const Color DARK_BLUE;
extern const Color LIGHT_RED;
extern const Color LIGHT_GREEN;
extern const Color LIGHT_BLUE;
extern const Color GRAY;
extern const Color LIGHT_GRAY;
extern const Color DARK_GRAY;
extern const Color WHITE;


enum MouseModifier { 
    NO_BUTTON_DOWN    = 0, 
    LEFT_MOUSE_DOWN   = SDL_BUTTON_LMASK,
    RIGHT_MOUSE_DOWN  = SDL_BUTTON_RMASK,
    MIDDLE_MOUSE_DOWN = SDL_BUTTON_MMASK
};


enum KeyCodes { 
    ESC_KEY = SDLK_ESCAPE,
    SPACE_KEY = SDLK_SPACE,
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

KeyModifier get_keyboard_state();


class Window {
protected:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Color background_color;
    bool repaint_flag;
    inline void eight_octs(int edge_x, int edge_y, 
                           int center_x, int center_y);
    SDL_TimerID timer_id;
public:
    Window(const std::string& title, int x, int y, int width, int height,
           const Color& background=WHITE);
    Window();

    int get_x() const;

    int get_y() const;

    int get_width() const;

    int get_height() const;

    virtual void mouse_moved(const MouseMotionEvent& ev);

    virtual void mouse_pressed(const MouseButtonEvent& ev);

    virtual void mouse_released(const MouseButtonEvent& ev);

    virtual void mouse_entered();

    virtual void mouse_exited();

    virtual void key_pressed(const KeyboardEvent& key);

    virtual void pre_paint();

    virtual void post_paint();

    void paint_all();

    virtual void paint() = 0;

    void repaint();

    void run(); 

    void draw_point(int x, int y);

    void fill_rectangle(const Rectangle& rect);

    void fill_rectangle(int x, int y, int width, int height);

    void draw_rectangle(const Rectangle& rect);

    void draw_rectangle(int x, int y, int width, int height);

    void fill_circle(int x, int y, int radius);

    void draw_circle(int x, int y, int radius);

    void draw_line(int x1, int y1, int x2, int y2);

    void set_color(const Color& color);
    
    void set_color(unsigned r, unsigned g, unsigned b, unsigned a);

    void start_timer(int msec);

    void stop_timer();

    void delay(int msec);

    virtual int timer_expired();
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

    void draw_point(int x, int y);

    void fill_rectangle(const Rectangle& rect);

    void fill_rectangle(int x, int y, int width, int height);

    void draw_rectangle(const Rectangle& rect);

    void draw_rectangle(int x, int y, int width, int height);

    void fill_circle(int x, int y, int radius);

    void draw_circle(int x, int y, int radius);

    void draw_line(int x1, int y1, int x2, int y2);

    void set_color(const Color& color);
    
    void set_color(unsigned r, unsigned g, unsigned b, unsigned a);

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
	 */
    GraphicalObject(int left, int top, int width, 
                    int height);

	/**  Copy construction creates a new graphical object with
	 *   identical location.  The new graphical object has the same
	 *   containing window but has a unique id.  The pre-existing 
	 *   graphical object is not affected by the copy.
	 *   @param other the graphical object from which this new graphical
	 *   object is being created.  	 */
    GraphicalObject(const GraphicalObject& other);

	/**  Assignment copies the location, size, and window container from 
	 *   another graphical object.  The id of the window being assigned 
	 *   is not affected.  The source graphical object is not affected 
	 *   by the copy.
	 *   @param other the source graphical object from which this graphical
	 *   object is being assigned.
	 *   @return a reference to this graphical object as per standard
	 *           assignment operator sementics
	 */
    GraphicalObject& operator=(const GraphicalObject& other);

	/**
	 *  Destroys a graphical object.
	 */
    virtual ~GraphicalObject();

	/**
	 *  Returns the x value of the left-top corner of the 
	 *  graphical object's bounding box.  The value is given 
	 *  in the viewport coordinate system.
	 *  @return the x coordinate of the graphical object's
	 *          left-top corner
	 */
	virtual int get_left() const;

	/**
	 *  Returns the y value of the left-top corner of the 
	 *  graphical object's bounding box.  The value is given 
	 *  in the viewport coordinate system.
	 *  @return the y coordinate of the graphical object's
	 *          left-top corner
	 */
	virtual int get_top() const;

	/**
	 *  Returns the width in viewport coordinates of the 
	 *  graphical object's bounding box.
	 *  @return the width of the graphical object
	 */
	virtual int get_width() const;

	/**
	 *  Returns the height in viewport coordinates of the 
	 *  graphical object's bounding box.
	 *  @return the height of the graphical object
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
	 */
	virtual void set(int x, int y, int width, int height);


	/**
	 *  The paint method is called by the containing window's 
	 *  paint method when it renders this graphical object.
	 *  This method is abstract, so concrete derived classes must
     *  provide a concrete imppementation.
	 *  @return nothing
	 */
    virtual void paint() = 0;


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
	 */
    virtual bool hit(int x, int y);

	/**
	 *  Set the containing window.
	 *  @param win a pointer to the window to make the containing 
	 *             window for this graphical object.
	 *  @return a pointer to the previous owner of this window
	 */
	virtual Window *set_window(ObjectWindow *win);

	/**
	 *  Get the containing window of this graphical object.
	 *  @return a pointer to the window that contains this 
	 *          graphical object.
	 */
	virtual Window *get_window() const;

	/**
	 *  Activates or deactivates the graphical object
     *  @param flag if true, activates the object; otherwise,
                    deactivates the object
	 *  @return nothing
	 */
	virtual void set_mouse_over(bool flag);

    /**
     *  Sets the graphical object's cursor shape.
     *  @param cursor the object's cursor shape
     *  @return the previous cursor shape
     */
    virtual CursorShape set_cursor(CursorShape cursor);

    /**
     *  Returns the cursor shape associated with this graphical
     *  object. 
     *  @return the cursor shape associated with this graphical
     *          object
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
	 */
	ObjectWindow(const std::string& title, int left, int top, int width, 
                 int height, int min_x, int max_x, 
                 int min_y, int max_y);

	/**
	 *  Creates a window of a specified width and height
     *  @param title the text to appear within the window's titlebar
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
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
	 */
	ObjectWindow(const std::string& title, int min_x, int max_x, 
                 int min_y, int max_y);

	/**
	 *  Create a default, plain window
	 */
	ObjectWindow();

	/**
	 *  Destroys a graphical window object.
	 */
	virtual ~ObjectWindow();

	/**  
	 *  Code executed before the call to paint
	 *  @return nothing
	 */
	void pre_paint() override;

	/**
	 *  Code executed after the call to paint
	 *  @return nothing
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
	 */
	void key_pressed(const KeyboardEvent& ev) override;

	/**
	 *  Adds a graphical object to the window.
	 *  @param obj the graphical object to add to this window
	 *  @return nothing
	 */
	void add(GraphicalObject *obj);

	/**  
	 *  Removes a graphical object from the window.
	 *  @param obj the graphical object to remove from the window.
	 *  @return nothing
	 */
    void remove(GraphicalObject *obj);

	/**
	 *  Removes all the graphical objects from the list of
	 *  contained graphical objects.  Frees up the memory 
	 *  allocated for the object.
	 *  @return nothing
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
	 */
    GraphicalObject *hit(int x, int y) const;

    /**
     *  Returns an iterator to the begining of the vector of 
     *  graphical objects this window contains.
     *  @return  an iterator to the begining of the vector of 
     *           graphical objects this window contains.
     */
    std::vector<GraphicalObject *>::iterator begin();

    /**
     *  Returns an iterator just past the end of the vector of 
     *  graphical objects this window contains.
     *  @return  an iterator just past the end of the vector of 
     *           graphical objects this window contains.
     */
    std::vector<GraphicalObject *>::iterator end();
};


// Global functions

void show_message_box(const std::string& title, 
                        const std::string& message);


} // End of sgl2 namespace


#undef main

#endif

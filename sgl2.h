#ifndef SGL2_H
#define SGL2_H

#include <string>

// TODO: change to SDL2/SDL.h
#include <C:/SDL2-2.0.4/include/SDL.h>

using std::string;

namespace sgl2 {

using KeyboardEvent = SDL_KeyboardEvent;

class Color {
   public:
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;

    /** Initializes a Color object.  The constructor
    *   enforces clamped RGB values (0...255).
    *   @param r the red component of the color
    *   @param g the green component of the color
    *   @param b the blue component of the color
    *   @param a the alpha channel of the color
    */
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
};

class Window {
   protected:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    Color background_color_;

   public:
    /** Basic window constructor.
        *
        *   Creates a basic window with title, dimensions.
        */
    Window(const string& title, int width, int height);

    /**
    *
    */
    Window(const string& title, int width, int height, const Color& background);

    /** Main loop.
        *
        *   Runs the main loop, which handles rendering, events, etc.
        */
    void Run();

    /**	Key press event handler.
        *
        *	Override this to handle key presses.
        */
    virtual void KeyPressed(KeyboardEvent& key);
};
}

#endif
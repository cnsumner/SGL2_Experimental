#ifndef SGL2_H
#define SGL2_H

#include <string>
#include <vector>

// TODO: change to SDL2/SDL.h
#include <C:/SDL2-2.0.4/include/SDL.h>

using std::string;
using std::vector;

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
    bool invalid_;  // calling this invalid to match up with what most graphics
                    // libraries call it

   public:
    /** Basic Window constructor.
    *   Creates a basic window with title, dimensions, and background color.
    *
    */
    Window(const string& title, int width, int height, const Color& background);

    /** Basic Window constructor.
    *	Creates a basic window with title, origin, dimensions, and background
    *color.
    *
    */
    Window(const string& title, int x, int y, int width, int height,
           const Color& background);

    void SetColor(const Color& color);

    /**	Key press event handler.
    *	Override this to handle key presses.
    */
    virtual void KeyPressed(KeyboardEvent& key);

    virtual void PrePaint();

    virtual void Paint() = 0;

    virtual void PostPaint();

    void PaintAll();

    void Repaint();

    void DrawPoint(Color color, int x, int y);

    void DrawRectangle(Color color, const SDL_Rect& rect);

    /** Main loop.
    *	Runs the main loop, which handles rendering, events, etc.
    */
    void Run();
};

/** forward declaration */
class ObjectWindow;

class GraphicalObject {
   protected:
    ObjectWindow* window_;
    int posx_;
    int posy_;
    Color color_;

   public:
    /** Unique identifier for object */
    const unsigned id_;
    GraphicalObject(ObjectWindow* window, Color color);
    GraphicalObject(ObjectWindow* window, Color color, int x, int y);
    virtual void Paint() = 0;
    virtual int GetPosX();
    virtual int GetPosY();
    virtual void SetPosX(int x);
    virtual void SetPosY(int y);

   private:
    static unsigned id_source_;
};

class Point : public GraphicalObject {
   public:
    Point(ObjectWindow* window, Color color, int x, int y);
    void Paint();
};

class Rectangle : public GraphicalObject {
   public:
    Rectangle(ObjectWindow* window, Color color, int x, int y, int width,
              int height);
    void Paint();
    int GetPosX();
    int GetPosY();
    void SetPosX(int x);
    void SetPosY(int y);

   private:
    SDL_Rect rect_;

};

class ObjectWindow : public Window {
   protected:
    vector<GraphicalObject*> objects_;

   public:
    ObjectWindow(const string& title, int width, int height,
                 const Color& background);

    void PrePaint() override;
    void Paint() override;
    void PostPaint() override;

    void Add(GraphicalObject* obj);
};
}

#undef main

#endif
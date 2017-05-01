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
	using MouseMotionEvent = SDL_MouseMotionEvent;
	using MouseButtonEvent = SDL_MouseButtonEvent;
	using WindowEvent = SDL_WindowEvent;

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
		bool invalid_;  // calling this invalid to match up with what most graphics libraries call it
		bool running_;

	public:
		/*!
		Creates a window with title, dimensions, and background color.
		*/
		Window(const string& title, int width, int height, const Color& background);

		/*!
		Sets the background color of the window.
		*/
		void SetBackgroundColor(const Color& color);

		/*!
		Key press event handler. Override this to handle key presses.
		@param key 
		*/
		virtual void KeyPressed(const KeyboardEvent & event);

		//TODO: add docs
		virtual void MouseMoved(const MouseMotionEvent & event);

		//TODO: add docs
		virtual void MousePressed(const MouseButtonEvent& event);

		//TODO: add docs
		virtual void MouseReleased(const MouseButtonEvent& event);

		//TODO: add docs
		virtual void MouseEntered();

		//TODO: add docs
		virtual void MouseExited();

		//TODO: add docs
		virtual void PrePaint();

		/*!
		Must override. Handles drawing objects, called when the window needs to be repainted.
		*/
		virtual void Paint() = 0;

		//TODO: add docs
		virtual void PostPaint();

		//TODO: add docs
		void PaintAll();

		//TODO: add docs
		void Repaint();

		//TODO: add docs
		void DrawPoint(Color color, int x, int y);

		//TODO: add docs
		void DrawPoint(int x, int y);

		//TODO: add docs
		void DrawRectangle(Color color, const SDL_Rect& rect);

		//TODO: add docs
		void DrawFilledRectangle(Color color, const SDL_Rect& rect);

		void EightOcts(int edge_x, int edge_y, int center_x, int center_y);

		//TODO: add docs
		void DrawCircle(Color, int x, int y, int radius);

		//TODO: add docs
		void DrawFilledCircle(Color color, int x, int y, int radius);

		//TODO: add docs
		virtual void Update();

		/*!
		Runs the main loop, which handles rendering, events, and updates.
		*/
		void Run();

		/*!
		Closes the window.
		*/
		void Quit();
	};

	/* forward declaration */
	class ObjectWindow;

	class GraphicalObject {
	protected:
		ObjectWindow* window_;
		SDL_Rect rect_;
		Color color_;
		bool filled_;

	public:
		/* Unique identifier for object */
		const unsigned id_;

		//TODO: add docs
		GraphicalObject(ObjectWindow* window, Color color);

		//TODO: add docs
		GraphicalObject(ObjectWindow* window, Color color, bool filled);

		//TODO: add docs
		GraphicalObject(ObjectWindow* window, Color color, int x, int y);

		//TODO: add docs
		virtual void Paint() = 0;

		//TODO: add docs
		int GetPosX();

		//TODO: add docs
		int GetPosY();

		//TODO: add docs
		void SetPosX(int x);

		//TODO: add docs
		void SetPosY(int y);

		//TODO: add docs
		int GetWidth();

		//TODO: add docs
		int GetHeight();

		//TODO: add docs
		void SetWidth(int width);

		//TODO: add docs
		void SetHeight(int height);

		//TODO: add docs
		void SetFilled(bool filled);

		//TODO: add docs
		bool Hit(int x, int y);

		//TODO: add docs
		bool Collision(GraphicalObject* obj);

	private:
		static unsigned id_source_;
	};

	class Point : public GraphicalObject {
	public:
		//TODO: add docs
		Point(ObjectWindow* window, Color color, int x, int y);

		//TODO: add docs
		void Paint() override;
	};

	class Rectangle : public GraphicalObject {
	public:
		//TODO: add docs
		Rectangle(ObjectWindow* window, Color color, int x, int y, int width,
			int height);

		//TODO: add docs
		Rectangle(ObjectWindow* window, Color color, int x, int y, int width,
			int height, bool filled);

		//TODO: add docs
		void Paint() override;
	};

	class Circle : public GraphicalObject {
	public:
		//TODO: add docs
		Circle(ObjectWindow* window, Color color, int x, int y, int radius);

		//TODO: add docs
		Circle(ObjectWindow* window, Color color, int x, int y, int radius, bool filled);

		//TODO: add docs
		void SetRadius(int radius);

		//TODO: add docs
		void Paint() override;
	};

	class ObjectWindow : public Window {
	protected:
		vector<GraphicalObject*> objects_;

	public:
		//TODO: add docs
		ObjectWindow(const string& title, int width, int height,
			const Color& background);

		//TODO: add docs
		void PrePaint() override;
		
		//TODO: add docs
		void Paint() override;
		
		//TODO: add docs
		void PostPaint() override;

		//TODO: add docs
		void Add(GraphicalObject* obj);

		//TODO: add docs
		void Remove(GraphicalObject* obj);

		/*!
		Returns a pointer to the first GraphicalObject found that intersects with the point (x, y). Returns nullptr if no object found. 
		*/
		GraphicalObject* GetFirstHit(int x, int y);
	};
}

#undef main

#endif
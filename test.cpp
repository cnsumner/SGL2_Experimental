#include "sgl2.hpp"

using namespace sgl2;

class MyWindow : public Window {
public:
	MyWindow(const string& title, int width, int height, const Color& background) : Window(title, width, height, background)
	{
		x = 1;
		y = 1;
	}

private:
	int x;
	int y;

	void Paint()
	{
		// TODO: break drawing code into other methods
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer_);

        SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer_, x, 1);

        SDL_RenderPresent(renderer_);
	}

	void KeyPressed(KeyboardEvent& key)
	{
		x++;
		Repaint();
	}

};

int main(int argc, char *argv[]) {
	MyWindow window = MyWindow("test", 800, 600, Color(0,0,0));
	window.Repaint();
	window.Run();

	return 0;
}
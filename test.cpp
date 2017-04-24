#include "sgl2.hpp"

using namespace sgl2;

class MyObjectWindow : public ObjectWindow {
   public:
    MyObjectWindow(const string& title, int width, int height,
                   const Color& background)
        : ObjectWindow(title, width, height, background) {}
};

int main() {
    MyObjectWindow window = MyObjectWindow("test", 800, 600, Color(0, 0, 0));
	Point p(&window, Color(255,0,0), 1, 1);
    Rectangle r(&window, Color(255,255,255), 20, 20, 50, 50);

	window.Add(&p);
    window.Add(&r);
    window.Run();

    return 0;
}
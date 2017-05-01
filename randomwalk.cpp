#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "sgl2.hpp"

using namespace sgl2;
using namespace std;

/**
 * A struct to hold the parameters to draw a circle on a window.
 */
struct Circle {
    Window *win;  // The window containing this circle
    double x;     // The location of the circle's center:
    double y;     // point (x,y)
    double r;     // The circle's radius
    Color color;  // The circle's color
    //  Constructor requires location, radius, and color information
    Circle(Window *win, double x, double y, double r, const Color& color): 
              win(win), x(x), y(y), r(r), color(color) {}
    void draw() {
        win->set_color(color);
        win->draw_circle(x, y, r + 1);
        //win->draw_rectangle(x, y, r + 1, r + 1);
        //win->set_color(WHITE);
        //win->fill_circle(x, y, r);
        //win->fill_rectangle(x, y, r, r);
        //std::cout << "Drawing rectangle" << '\n';
    }
};


//  A window that traces the movements of a circle
//  on a random walk.  The trace leaves behind a multicolor
//  tube-like structure of varying diameter
class RandomCircleWindow : public Window {
    double x;              //  Current position of the
    double y;              //  circle's center: (x, y)
    double z;              //  Circle's radius

    int dx;                //  Change in x
    int dy;                //  Change in y
    double dz;             //  Change in radius

    int run_x;             //  Length of run for dx
    int run_y;             //  Length of run for dy
    int run_r;             //  Length of run for radius change

    bool animate;          //  Are we animating?

    vector<Circle> track;  //  Record of all the circle locations

    Color current_color;   //  The current color for a circle 
    int color_run;

    const int delay = 10;

    //  Save the current locations to a text file
    void save() {
        string filename;
        cout << "Enter file name: ";        //  Get file name from user
        cin >> filename;
        ofstream fout(filename);            //  Open file for writing, if possible
        if (fout.good()) {
            fout << track.size() << endl;   //  Store number of circles
            //  Store each circle's component data about its location, size, and color
            for (auto& circ : track)
                fout << circ.x << ' ' << circ.y << ' ' << circ.r << ' '
                     << circ.color.red << ' ' << circ.color.green << ' ' 
                     << circ.color.blue << endl;
        }
        else
            cout << "Could not save the file" << endl;
    }

    //  Load circle locations from a text file
    void load() {
        string filename;
        cout << "Enter file name: ";  //  Get file name from user
        cin >> filename;
        ifstream fin(filename);       //  Oprn the file for reading, if possible
        if (fin.good()) {
            track.clear();            //  Empty the circle vector
            int size;
            fin >> size;              //  Read number of circles
            //  Obtain the location, size, and color information for each circle
            for (int i = 0; i < size; i++) {
                double x, y, z, red, green, blue;
                fin >> x >> y >> z;
                fin >> red >> green >> blue;
                //  Add the circle to the collection of circles
                track.push_back(Circle(this, x, y, z, Color(red, green, blue)));
            }
        }
        else
            cout << "Could not load the file" << endl;

    }
public:
    //  Create a window of sufficient size
    RandomCircleWindow(double min_x, double max_x, double min_y, double max_y) :
                         Window("Tubes", min_x, max_x, min_y, max_y),
                         x((min_x + max_x) / 2), y((min_y + max_y) / 2), z(10),
                         dx(0), dy(0), dz(0.01), run_x(0), run_y(0), run_r(0),
                         animate(true), current_color(200, 200, 200), 
                         color_run(0) {
        start_timer(delay);   //  Enable animation
    }

    //  Draw the track of all the multicolor circles
    void paint() override {
        for (auto& circ : track)
            circ.draw();
    }

    ////  Do not erase image between repaints
    //void prepaint() override {
    //    glPushMatrix();
    //    glColor3d(0.0, 0.2, 0.0);
    //}

    //  Respond to user's key strokes
    void key_pressed(const KeyboardEvent& ev) override {
        animate = !animate;   //  Pause or resume
        char k = ev.keysym.sym;
        switch (k) {
            case 'S':  //  Save the circle track to a text file
            case 's':
                save();
                break;
            case 'L':  //  Load a circle track from a text file
            case 'l':
                load();
                break;
            case 'M':  //  Load a circle track from a text file
            case 'm':
                show_message_box("Sample message box", 
                                 "You typed the 'L' key");
                break;
        }
        //cout << "# circles: " << track.size() << endl;
        repaint();
        Window::key_pressed(ev);  //  Defer to base class for further processing
    }

    int timer_expired() override {
        //  Determine if a run of each component has run its course
        //  and update if necessary
        //std::cout << "In timer_expired" << '\n';
        if (run_x == 0) {
            run_x = rand() % 100 + 1; //random(1, 100);  //  Determine length of next run for x
            dx = rand() % 7 - 3; //random(-3, 3);      //  New x direction and speed
        }
        if (run_y == 0) {
            run_y = rand() % 100 + 1; //random(1, 100);  //  Determine length of next run for y
            dy = rand() % 7 - 3; //random(-3, 3);      //  New y direction and speed
        }
        if (run_r == 0) {
            run_r = rand() % 100 + 1; //random(1, 100);  //  Determine length of next run for the radius
            dz = 0.1*(rand() % 7 - 3); //random(-3, 3); //  New radius
            //dz = 0.1;   //  Fixed radius
        }
        x += dx;   //  Update x
        y += dy;   //  Update y
        z += dz;   //  Update radius
        //  Limit the range of the circle's random walk
        int min_x = 0, max_x = get_width(), 
            min_y = 0, max_y = get_height();
        x = (x < min_x - z) ? min_x - z : x;
        x = (x > max_x + z) ? max_x + z : x;
        y = (y < min_y - z) ? min_y - z : y;
        y = (y > max_y + z) ? max_y + z : y;
        //  Bound the circle's size
        z = (z < 10) ? 10 : z;
        z = (z > 30) ? 30 : z;

        run_x--;   //  Completed one more step of the run for x
        run_y--;   //  Completed one more step of the run for y 
        run_r--;   //  Completed one more step of the run for radius

        //  Determine if the color has run its course, and a new 
        //  color should be generated
        if (color_run == 0) {
            color_run = rand() % 900 + 100; //random(100, 1000);  //  New run length
            current_color.red = rand() % 256; //random(1, 1000) / 1000.0;    //  New red component
            current_color.green = rand() % 256; //random(1, 1000) / 1000.0;  //  New green component
            current_color.blue = rand() % 256; //random(1, 1000) / 1000.0;   //  New blue component
        }

        color_run--;  //  Completed one more step of the run for this color

        //repaint();
        //  Record the information for this circle
        track.push_back(Circle(this, x, y, z, current_color));


        // if (animate)   //  Should we continue to track circles?
        //     //start_timer(delay);
        //     return delay;
        // std::cout << "Returning zero" << '\n';
        // return 0;  // Stop animation
        // //return delay;  // Stop animation

        return delay;
    }
};

int main() {
    //set_random_seed(42);   //  Set the random number generator seed
    srand(42);   //  Set the random number generator seed
    //  Make and run a window for displaying the circles
    //make_shared<RandomCircleWindow>(0, 1900, 0, 1000)->run();
    make_shared<RandomCircleWindow>(100, 100, 800, 600)->run();
}

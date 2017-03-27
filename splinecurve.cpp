#include <iostream>
#include <fstream>
#include "sgl2.hpp"
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;
using namespace sgl2;

class SplinePoint: public GraphicalObject {
    static const int offset = 4;
public:
    bool active;
    bool visible;
    int x, y;
    SplinePoint(int x, int y): 
               GraphicalObject(x - offset, y - offset, 2*offset, 2*offset), 
                               visible(true), x(x), y(y) {
        set_cursor(CURSOR_CROSSHAIR);
    }

    //  Plot a point in a graphical window
    void paint() override {
        window->set_color(DARK_BLUE);
        if (visible) {
            int width = 2*offset;
            window->fill_rectangle(x - offset, y - offset, width, width); 
            window->set_color(WHITE);
            window->fill_rectangle(x - 1, y - 1, 2, 2); 
            if (mouse_over) {
                int offset = 3*SplinePoint::offset;
                int width = 2*offset;
                window->set_color(DARK_BLUE);
                window->draw_rectangle(x - offset, y - offset, width, width); 
            }
        }
    }

    //void move_to(int x, int y) override {
    //    //GraphicalObject::move_to(left + width/2.0, top + height/2.0);
    //    GraphicalObject::move_to(x - width/2.0, y - height/2.0);
    //}

    void move_to(int left, int top) override {
        //GraphicalObject::move_to(left + width/2.0, top + height/2.0);
        x = left + width/2;
        y = top + height/2;
        GraphicalObject::move_to(left, top);
    }

    //void mouse_moved(const MouseMotionEvent& ev) override {
    //    //std::cout << "State " << ev.state << "  " 
    //    //          << LEFT_MOUSE_DOWN << "  " 
    //    //          << (ev.state & LEFT_MOUSE_DOWN) << std::endl;
    //    if (ev.state & LEFT_MOUSE_DOWN) {
    //        //GraphicalObject::mouse_moved(ev);
    //        //this->x = left + width/2.0;
    //        //this->y = top + height/2.0;
    //        x += ev.xrel;
    //        y += ev.yrel;
    //        //move_to(left + width/2.0, top + height/2.0);
    //        std::cout << "Moving object to " 
    //                  << x << "," << y << std::endl;

    //    }
    //}

    //void mouse_dragged(const MouseMotionEvent& ev) override {
    //    GraphicalObject::mouse_dragged(ev);
    //    mouse_moved(ev);
    //    //if (dragging) {
    //    //    GraphicalObject::mouse_moved(ev);
    //    //    this->x = left + width/2.0;
    //    //    this->y = top + height/2.0;
    //    //}
    //}

    void make_active(bool flag) {
        active = flag;
    }
};

const double SPLINE_MATRIX[4][4] 
                    = { { -1.0,  3.0, -3.0, 1.0 },
                        {  3.0, -6.0,  3.0, 0.0 },
                        { -3.0,  0.0,  3.0, 0.0 },
                        {  1.0,  4.0,  1.0, 0.0 } };


class SplineWindow: public ObjectWindow {
protected:

    //class SplinePopup;

    //  A global vector that stores the locations of mouse presses
    vector<SplinePoint *> mouse_points;

    //SplinePopup *popup;

    //void dispose_points(vector<SplinePoint *>& pts)
    //{   //  Free up each individual heap allocated point
    //    for ( unsigned i = 0; i < pts.size(); i++ )
    //        delete pts[i];
    //    pts.clear();
    //}



    void spline_mult(double *vec, const double m[4][4]) {
        double spline_result[4];
        for (int col = 0;  col < 4;  col++) {
            double sum = 0.0;
            for (int row = 0;  row < 4;  row++)
                sum += vec[row]*m[row][col];
            spline_result[col] = sum;
        }
        vec[0] = spline_result[0];  vec[1] = spline_result[1];
        vec[2] = spline_result[2];  vec[3] = spline_result[3];
    }


    double spline_function(double t, double p1, double p2, 
                           double p3, double p4) {
        double spline_vector[4];
        spline_vector[3] = 1.0; 
        spline_vector[2] = spline_vector[3]*t;
        spline_vector[1] = spline_vector[2]*t;
        spline_vector[0] = spline_vector[1]*t;
        spline_mult(spline_vector, SPLINE_MATRIX);
        return 1.0/6.0*(spline_vector[0]*p1 + spline_vector[1]*p2 +
            spline_vector[2]*p3 + spline_vector[3]*p4);
    }


    void draw_spline_curve() {
        int num_points = mouse_points.size();
        if (num_points > 3) {
            set_color(DARK_RED);
            double oldX, oldY, newX, newY;
            oldX = spline_function(0.0, 
                                   mouse_points[0]->x,
                                   mouse_points[1]->x,
                                   mouse_points[2]->x,
                                   mouse_points[3]->x);
            oldY = spline_function(0.0, 
                                   mouse_points[0]->y,
                                   mouse_points[1]->y,
                                   mouse_points[2]->y,
                                   mouse_points[3]->y);
            for (int i = 1;  i < num_points - 2;  i++) {
                for (double param = 0.0;  param <= 1.0;  param += 0.05) {
                    newX = spline_function(param, 
                        mouse_points[i - 1]->x,
                        mouse_points[  i  ]->x,
                        mouse_points[i + 1]->x,
                        mouse_points[i + 2]->x);
                    newY = spline_function(param, 
                        mouse_points[i - 1]->y,
                        mouse_points[  i  ]->y,
                        mouse_points[i + 1]->y,
                        mouse_points[i + 2]->y);
                    draw_line(oldX, oldY, newX, newY);
                    oldX = newX;  oldY = newY;
                }
            }
        }
    }

    bool draw_control_points,
         draw_control_lines;

    void draw_lines() {
        int num_points = mouse_points.size();
        if (num_points > 1) {
            double old_x = mouse_points[0]->x,
                   old_y = mouse_points[0]->y;
            set_color(Color(0.3, 0.3, 1.0));
            for (int i = 1;  i < num_points;  i++) {
                //draw_dashed_line(old_x, old_y, mouse_points[i]->x,
                //                 mouse_points[i]->y, 0x00ff);
                draw_line(old_x, old_y, mouse_points[i]->x,
                                 mouse_points[i]->y);
                old_x = mouse_points[i]->x;
                old_y = mouse_points[i]->y;
            }
        }
    }


    SplinePoint *active_point = nullptr;

    void remove_active_point() {
        //for (vector<SplinePoint *>::iterator iter = mouse_points.begin();
        //    iter != mouse_points.end(); iter++ )
        for (auto iter = std::begin(mouse_points); 
             iter != std::end(mouse_points); iter++) {
            if (*iter == active_object) {
                SplinePoint *temp = *iter;
                mouse_points.erase(iter);
                remove(temp);
                delete temp;
                active_object = nullptr;  //  No active point now
                //cout << "Deleting a point" << endl;
                break;
            }
        }
        repaint();
    }

    void insert_point() {
        for (auto iter = ::begin(mouse_points) + 1;
             iter != ::end(mouse_points); iter++ ) {
            if ( *iter == active_object) {
                SplinePoint *pt = dynamic_cast<SplinePoint *>(active_object);
                double x = (pt->x + (*(iter - 1))->x)/2,
                       y = (pt->y + (*(iter - 1))->y)/2;
                SplinePoint *new_point = new SplinePoint(x, y);
                mouse_points.insert(iter, new_point);
                add(new_point);
                active_point = nullptr;  //  No active point now
                break;
            }
        }
        repaint();
    }

    //class SplinePopup: public PopupMenu
    //{
    //public:
    //    SplinePopup(SplineWindow *win): PopupMenu(win)
    //    {
    //        cout << "making a popup menu" << endl;
    //        add_menu_item("Hide Control Points", &SplineWindow::control_points_off);
    //        add_menu_item("Show Control Lines", &SplineWindow::control_lines_on);
    //        add_menu_item("Remove Last Point", &SplineWindow::pop_point);
    //        add_menu_item("Clear", &SplineWindow::clear);
    //        add_menu_item("Quit", &SplineWindow::quit);
    //    }
    //};


public:

    SplineWindow(int width, int height): ObjectWindow("Splines", 100, 100, 
                                                      width, height,
                                                      0.0, 640.0, 0.0, 480.0),
        draw_control_points(true), draw_control_lines(false) {
        //popup = new SplinePopup(this);
    }


    void paint() override {
        //set_color(Color(0.0, 0.2, 0.0));

        int num_points = mouse_points.size();
        /*if ( draw_control_points )
            for ( int i = 0;  i < num_points;  i++ ) 
                mouse_points[i]->draw();
        else if ( active_point )
            active_point->draw();*/

        if (draw_control_lines)
            draw_lines();
        if (num_points > 3)
            draw_spline_curve();
    }


    void mouse_moved(const MouseMotionEvent& ev) override {
        ObjectWindow::mouse_moved(ev);
        SplinePoint *old_active = active_point;
        active_point = dynamic_cast<SplinePoint *>(hit(ev.x, ev.y));
        if (old_active) {
            old_active->make_active(false);
            old_active->set_cursor(CURSOR_ARROW);
        }
        if (active_point) {
            active_point->make_active(true);
            if (ev.state & LEFT_MOUSE_DOWN)
                //active_point->move_to(ev.x, ev.y);
                active_point->mouse_moved(ev);
            active_point->set_cursor(CURSOR_CROSSHAIR);
        }
        repaint();
    }

    //void mouse_dragged(double x, double y) {
    //    if (active_point) {
    //        active_point->move_to(x, y);
    //        repaint();
    //    }
    //}


   /*
    *  Releasing the mouse button with the shift key down
    *  adds a control point to the set of spline control points.
    */
    void mouse_released(const MouseButtonEvent& ev) override {
        //if (dragging) {
        //    dragging = false;
        //    repaint();
        //    return;
        //}
        KeyModifier state = get_keyboard_state();
        //std::cout << "In mouse_released state = " << state << std::endl;
        //std::cout << SHIFT_DOWN << std::endl;
        if (state & SHIFT_DOWN) {
            //std::cout << "Adding a point in mouse_released" << std::endl;
            SplinePoint *pt = new SplinePoint(ev.x, ev.y);
            mouse_points.push_back(pt);
            add(pt);
        }
        repaint();
    }


   /*
    *  Pressing the spacebar toggles the display of the 
    *  the control points.
    */
    void key_pressed(const KeyboardEvent& ev) override {
        switch (ev.keysym.sym) {
        case 'P':
        case 'p':
            if ( draw_control_points )
                control_points_off();
            else
                control_points_on();
            break;
        case 'L':
        case 'l':
            if ( draw_control_lines )
                control_lines_off();
            else 
                control_lines_on();
            break;
        case 'E':
        case 'e':
            clear(); //dispose_points(mouse_points);
            break;
        case 'X':
        case 'x':
            remove_active_point();
            //set_cursor(CURSOR_ARROW);
            break;
        case 'A':
        case 'a':
            insert_point();
            break;
        }
        ObjectWindow::key_pressed(ev);
        repaint();
    }


    void control_points_off() {
        int num_points = mouse_points.size();
        for (int i = 0;  i < num_points;  i++) 
            mouse_points[i]->visible = false;
        //popup->replace_menu_item("Hide Control Points",
        //                         "Show Control Points", 
        //                         &SplineWindow::control_points_on);
        draw_control_points = false;
        repaint();
    }

    void control_points_on() {
        int num_points = mouse_points.size();
        for (int i = 0;  i < num_points;  i++) 
            mouse_points[i]->visible = true;
        //popup->replace_menu_item("Show Control Points",
        //                         "Hide Control Points", 
        //                         &SplineWindow::control_points_off);
        draw_control_points = true;
        repaint();
    }

    void control_lines_off() {
        if (draw_control_lines) {
            //popup->replace_menu_item("Hide Control Lines",
            //                         "Show Control Lines", 
            //                         &SplineWindow::control_lines_on);
            draw_control_lines = false;
            repaint();
        }
    }

    void control_lines_on() {
        if (!draw_control_lines) {
            //popup->replace_menu_item("Show Control Lines",
            //                         "Hide Control Lines", 
            //                         &SplineWindow::control_lines_off);
            draw_control_lines = true;
            repaint();
        }
    }

    void pop_point() {
        if (mouse_points.size() > 0) {
            SplinePoint *last = mouse_points[mouse_points.size() - 1];
            mouse_points.pop_back();
            remove(last);
            //delete last;
        }
        repaint();
    }

    void clear() {
        //dispose_points(mouse_points);
        mouse_points.clear();
        remove_all();
        repaint();
    }

    void quit() {
        //dispose_points(mouse_points);
        //delete popup;
        exit(0);
    }
};

int main() {
    make_shared<SplineWindow>(640, 480)->run();
}

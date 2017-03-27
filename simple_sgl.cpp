#include <iostream>
#include <string>
#include <memory>

#include "sgl2_window.hpp"

using namespace sgl2;

class SimpleGraphicsWindow: public Window {
    Rectangle player_position;
public:
    SimpleGraphicsWindow(): Window("Simple Graphics", 100, 200, 
                                   300, 400, LIGHT_GREEN) {
        //std::cout << "Making a SimpleGraphicsWindow" << std::endl;
        player_position.x = 20;
        player_position.y = 20;
        player_position.w = 20;
        player_position.h = 20;
    }

    void paint() override {
        // Change color to dark blue
        set_color(DARK_BLUE);
        // Render player (a simple rectangle)
        fill_rectangle(player_position);
    }

    void mouse_moved(const MouseMotionEvent& motion) override {
        std::cout << "(" << motion.x << "," 
                  << motion.y << ")" << std::endl;
        repaint();
    }

    void mouse_pressed(const MouseButtonEvent& button) override {
        std::cout << button.x << " " 
                  << button.y << std::endl;
        player_position.x = button.x - player_position.w/2;
        player_position.y = button.y - player_position.h/2;
        repaint();
    }

    void mouse_released(const MouseButtonEvent& button) override {
        std::cout << " *** UP ***" << std::endl;
        repaint();
    }

    void mouse_entered() override {
        std::cout << "I'm in!" << std::endl;
        //repaint();
    }

    void mouse_exited() override {
        std::cout << "I'm out!" << std::endl;
        //repaint();
    }

    void key_pressed(const KeyboardEvent& key) override {
        std::cout << "<<<" << key.keysym.sym << ">>>" << std::endl;
        switch (key.keysym.sym) {
            case RIGHT_KEY:
                player_position.x += 5;
                break;
            case LEFT_KEY:
                player_position.x -= 5;
                break;
            case DOWN_KEY:
                player_position.y += 5;
                break;
            case UP_KEY:
                player_position.y -= 5;
                break;
            case F1_KEY:
                std::cout << "You pressed the F1 key" << std::endl;
                break;
            case M_KEY:
                show_message_box("Player Position", 
                                 "("s 
                                  + std::to_string(player_position.x) 
                                  + ", "s 
                                  + std::to_string(player_position.y)
                                  + ")"s);
                break;
            default :
                break;
        }
        repaint();
    }

};


int main() {
    std::make_shared<SimpleGraphicsWindow>()->run();
}

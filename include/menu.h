#pragma once

#include <vector>
#include "game.h"

class Menu {
   private:
    double width;
    double height;
    size_t action_count{3};
    size_t selectedOptionIndex{0};
    sf::Font font;
    std::vector<sf::Text> m_menu;

   public:
    Menu(double width_, double height_, sf::Font font_);
    ~Menu() = default;

    enum class Action {
        PLAY,
        INFO,
        QUIT,
    };

<<<<<<< HEAD
    void open_info(sf::RenderWindow& window);
=======
    void open_info(sf::RenderWindow& window) const;
>>>>>>> 3ec9479c56848fd0335ee8f16ea82edb180c81f9
    void draw_text(float x,
                   float y,
                   const std::string& text,
                   sf::RenderWindow& window) const;

    void draw(sf::RenderWindow& window) const;
    void move_up();
    void move_down();

    Action get_selected_action() const {
        return static_cast<Action>(selectedOptionIndex);
    }
};
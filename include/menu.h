#pragma once

#include <vector>
#include "game.h"

class Menu {
   private:
    size_t numberOfOptions{3};
    size_t selectedOptionIndex{0};
    sf::Font font;
    std::vector<sf::Text> m_menu;

   public:
    Menu(double width, double height);
    ~Menu();

    enum class Options {
        PLAY,
        INFO,
        QUIT,
    };

    void open_info(float width, float height, sf::RenderWindow& window);
    void draw_text(float x,
                   float y,
                   const std::string& text,
                   sf::RenderWindow& window) const;

    void draw(sf::RenderWindow& window) const;
    void MoveUp();
    void MoveDown();

    Options getPressedItem() const {
        return static_cast<Options>(selectedOptionIndex);
    }
};
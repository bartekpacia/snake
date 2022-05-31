#include "../include/menu.h"

Menu::Menu(double width_, double height_, sf::Font font_)
    : width(width_), height(height_), font(font_) {
    m_menu.resize(action_count);
    const std::vector<std::string> options = {"Play", "Info", "Quit"};

    for (size_t i = 0; i < action_count; i++) {
        double action_width = width / 2 - m_menu[i].getLocalBounds().width;
        double action_height = height / (action_count + 1) * (i + 1);

        m_menu[i].setFont(font);
        m_menu[i].setString(options[i]);
        m_menu[i].setPosition(sf::Vector2f(action_width, action_height));
        m_menu[i].setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
    }
}

void Menu::open_info(sf::RenderWindow& window) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Q:
                        case sf::Keyboard::Escape:
                        case ::sf::Keyboard::Enter:
                            return;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        sf::Color midnight_blue(1, 3, 50);
        window.clear(midnight_blue);
        draw_text(0 * width, 0 * height,
                  "no i ja sie pytam czlowieku dumny ty \njestes z siebie "
                  "zdajesz sobie \nsprawe z tego co robisz?\nmasz ty wogole "
                  "rozum i godnosc czlowieka?",
                  window);
        window.display();
    }
}

void Menu::draw_text(float x,
                     float y,
                     const std::string& text,
                     sf::RenderWindow& window) const {
    sf::Text T;
    T.setString(text);
    T.setFont(font);
    T.setPosition(x, y);
    T.setFillColor(sf::Color::White);
    window.draw(T);
}

void Menu::draw(sf::RenderWindow& window) const {
    for (size_t i = 0; i < action_count; i++)
        window.draw(m_menu[i]);
}

void Menu::move_up() {
    if (selectedOptionIndex > 0) {
        m_menu[selectedOptionIndex].setFillColor(sf::Color::White);
        selectedOptionIndex--;
        m_menu[selectedOptionIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::move_down() {
    if (selectedOptionIndex + 1 < action_count) {
        m_menu[selectedOptionIndex].setFillColor(sf::Color::White);
        selectedOptionIndex++;
        m_menu[selectedOptionIndex].setFillColor(sf::Color::Red);
    }
}

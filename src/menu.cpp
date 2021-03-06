#include "../include/menu.h"

Menu::Menu(double width_, double height_, sf::Font font_)
    : width(width_), height(height_), font(font_) {
    m_menu.resize(action_count);
    const std::vector<std::string> options = {"Play", "Info", "Quit"};

    for (size_t i = 0; i < action_count; i++) {
        m_menu[i].setFont(font);
        m_menu[i].setString(options[i]);

        double action_width = (width - m_menu[i].getLocalBounds().width) / 2;
        double action_height = height / (action_count + 1) * (i + 1);

        m_menu[i].setPosition(sf::Vector2f(action_width, action_height));
        m_menu[i].setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
    }
}

void Menu::open_info(sf::RenderWindow& window) const {
    sf::Color midnight_blue(1, 3, 50);
    window.clear(midnight_blue);
    draw_text(
        0 * width, 0 * height,
        L"CONTROLS:\n\t[ ← ] - Turn snake's head West\n\t[ → ] - Turn snake's "
        L"head East \n\t[ ↑ ] - Turn snake's head North\n\t[ ↓ ] - Turn "
        L"snake's "
        L"head South\n\t[ ENTER ] - Select an option\n\t[ "
        L"P ] - Pause the game\n\t[ R ] - Restart the game\n\t[ ESC/Q ] - Quit",
        window);
    window.display();

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
    }
}

void Menu::draw_text(float x,
                     float y,
                     const std::wstring& text,
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

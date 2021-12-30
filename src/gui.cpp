#include "../include/gui.h"

GUI::GUI(const GUISettings& settings) {
    settings_ = settings;

    sf::ContextSettings window_settings;
    window_settings.antialiasingLevel = settings_.antialiasing;

    if (!settings_.font.loadFromFile("arial.ttf")) {
        std::clog << "snake: error loading font file" << std::endl;
    }

    window_.create(sf::VideoMode(settings_.width, settings_.height), "snake",
                   sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize,
                   window_settings);

    for (int i = 0; i < int(StatusKey::COUNT); i++) {
        status_keys_[i] = false;
    }

    screenshots_cnt_ = 0;
}

bool GUI::update() {
    if (!settings_.running) {
        window_.close();
        return false;
    }

    window_.clear(settings_.color_background);

    handle_input();
    render_grid();

    window_.display();
    sf::sleep(sf::milliseconds(settings_.sleep_time_ms));
    return true;
}

void GUI::handle_input() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Resized) {
            settings_.width = event.size.width;
            settings_.height = event.size.height;
            sf::FloatRect visible_area(0, 0, event.size.width,
                                       event.size.height);
            window_.setView(sf::View(visible_area));
            std::cout << "snake: window resized: " << event.size.width << "x"
                      << event.size.height << std::endl;
        }
        if (event.type == sf::Event::Closed)
            settings_.running = false;
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up)
                status_keys_[int(StatusKey::UP)] = true;
            if (event.key.code == sf::Keyboard::Down)
                status_keys_[int(StatusKey::DOWN)] = true;
            if (event.key.code == sf::Keyboard::Left)
                status_keys_[int(StatusKey::LEFT)] = true;
            if (event.key.code == sf::Keyboard::Right)
                status_keys_[int(StatusKey::RIGHT)] = true;
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Up)
                status_keys_[int(StatusKey::UP)] = false;
            if (event.key.code == sf::Keyboard::Down)
                status_keys_[int(StatusKey::DOWN)] = false;
            if (event.key.code == sf::Keyboard::Left)
                status_keys_[int(StatusKey::LEFT)] = false;
            if (event.key.code == sf::Keyboard::Right)
                status_keys_[int(StatusKey::RIGHT)] = false;
        }
    }
}

void GUI::render_grid() {
    double size = settings_.grid_size;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            auto x = (settings_.width / size) * j;
            auto y = (settings_.height / size) * i;

            auto point = sf::Vertex(sf::Vector2f(x, y), Color::Red);
            window_.draw(&point, 1, sf::Points);
        }
    }
}

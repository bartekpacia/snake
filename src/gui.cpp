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

    render_grid();

    window_.display();
    window_.setTitle("snake");
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

            if (event.key.code == sf::Keyboard::S)
                save_screenshot();
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
    std::cout << "render grid" << std::endl;
    for (int i = 0; i < 100; i++) {
        auto y = (settings_.width / 100) * i;
        for (int j = 0; j < 100; j++) {
            auto x = (settings_.height / 100) * j;

            auto point = sf::Vertex(sf::Vector2f(x, y), Color::Black);
            window_.draw(&point, 1, sf::Points);
        }
    }
}

bool GUI::save_screenshot() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d.%m.%Y-%H.%M.%S");

    auto filename = settings_.output_dir + "/screenshot-" +
                    std::to_string(++screenshots_cnt_) + "-" + oss.str() +
                    ".png";

    sf::Texture texture;
    texture.create(settings_.width, settings_.height);
    texture.update(window_);

    if (!texture.copyToImage().saveToFile(filename)) {
        std::cerr << "snake: error: unable to save screenshot" << std::endl;
        return false;
    } else {
        std::cout << "snake: screenshot saved to " << filename << std::endl;
        return true;
    }
}

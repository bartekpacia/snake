#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef sf::Color Color;

struct GUISettings {
    bool running = true;
    unsigned int width = 1280;
    unsigned int height = 720;
    sf::Font font;
    float sleep_time_ms = 1000.0f / 30.0f;

    Color color_background = Color(16, 16, 24);
    Color color_grid = Color(40, 40, 50);

    unsigned int antialiasing = 8;

    std::string output_dir = ".";
};

/**
 * GUI is responsible for the visual layer of the application and interacting
 * with the user.
 */
class GUI {
   public:
    explicit GUI(const GUISettings& settings);
    virtual bool update();

    bool save_screenshot();

   private:
    enum class StatusKey {
        UP,
        RIGHT,
        DOWN,
        LEFT,
        COUNT,
    };

    void handle_input();
    void render_grid();

    GUISettings settings_;
    sf::RenderWindow window_;
    bool status_keys_[int(StatusKey::COUNT)];
    size_t screenshots_cnt_;
};

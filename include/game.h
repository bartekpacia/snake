#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

typedef sf::Color Color;

struct GameSettings {
    bool running = true;
    unsigned int width = 720;
    unsigned int height = 720;
    unsigned int grid_size = 16;
    sf::Font font;
    float sleep_time_ms = 1000.0f / 30.0f;

    Color color_background = Color::Black;
    Color color_grid = Color(40, 40, 50);

    unsigned int antialiasing = 8;
};

enum class TileObject { Empty, Snake, Point };

/**
 * GUI is responsible for the visual layer of the application and interacting
 * with the user.
 */
class Game {
   public:
    explicit Game(const GameSettings& settings);
    virtual bool update();
    virtual ~Game() = default;

   private:
    enum class MoveStatus {
        LEFT,
        UP,
        RIGHT,
        DOWN,
    };

    void handle_logic();
    void handle_input();
    void render_grid();

    std::vector<std::vector<TileObject>> state_;
    sf::Vector2i snake_head_pos_;
    sf::Vector2i point_pos_;

    GameSettings settings_;
    sf::RenderWindow window_;
    MoveStatus move_status_;
    std::chrono::seconds::rep timestamp_;
};

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <vector>
#include "menu.h"

using Color = sf::Color;
class Menu;

struct GameSettings {
    bool running = true;
    unsigned int width = 720;
    unsigned int height = 720;
    unsigned int grid_size = 16;
    sf::Font font;
    unsigned int interval_ms = 500.0F;

    Color color_background = Color::Black;
    Color color_snake = Color::Green;
    Color color_pickup = Color::Yellow;
    Color color_ui = Color::White;

    unsigned int antialiasing = 8;
};

enum class TileObject { Empty, Snake, Pickup };

/**
 * Game is responsible for the visual layer of the application and interacting
 * with the user.
 */
class Game {
   public:
    explicit Game(const GameSettings& settings);
    Game(const Game&) = delete;
    Game(const Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(const Game&&) = delete;
    ~Game() = default;

    bool update();

    unsigned int high_score() const { return high_score_; }

   private:
    enum class MoveStatus {
        LEFT,
        UP,
        RIGHT,
        DOWN,
    };

    // menu
    void open_menu();
    void handle_menu_input(const sf::Event::KeyPressed& event,
                           Menu& menu,
                           bool& leave_menu);

    // event loop methods
    void handle_logic();
    void handle_input();
    void render_grid();
    void render_ui();
    void handle_game_over();
    void reset_game();
    void pause_game();

    // game event methods
    void move_snake();

    // Generates a new crystal.
    //
    // This method assumes that there is no crystal is present when it is
    // called. It takes snake's position into account and guarantees that new
    // crystal's position will not overlap with snake's position at the time of
    // invocation.
    void new_crystal();

    unsigned int score_;
    unsigned int high_score_;
    bool game_over;
    bool paused;

    std::vector<std::vector<TileObject>> state_;
    std::vector<sf::Vector2u> snake_positions_;
    sf::Vector2u point_pos_;

    GameSettings settings_;
    sf::RenderWindow window_;
    MoveStatus move_status_;
    std::chrono::seconds::rep timestamp_;
};

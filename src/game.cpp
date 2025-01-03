#include "../include/game.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include "../include/menu.h"

Game::Game(const GameSettings& settings)
    : score_(0), game_over(false), paused(false), settings_(settings) {
    sf::ContextSettings window_settings;
    window_settings.antiAliasingLevel = settings_.antialiasing;

    if (!settings_.font.openFromFile("fonts/arial.ttf")) {
        std::clog << "snake: error loading font file" << std::endl;
    }

    window_.create(
        sf::VideoMode(sf::Vector2u(settings_.width, settings_.height)), "snake",
        sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize,
        sf::State::Windowed, window_settings);

    // open menu screen
    open_menu();

    move_status_ = MoveStatus::LEFT;

    timestamp_ = std::chrono::system_clock::now().time_since_epoch() /
                 std::chrono::seconds(1);

    // fill the board with empty tiles
    for (size_t i = 0; i < settings.grid_size; i++) {
        std::vector<TileObject> row;
        for (size_t j = 0; j < settings.grid_size; j++) {
            row.push_back(TileObject::Empty);
        }
        state_.push_back(row);
    }

    // place snake head on the board
    auto snake_head_position =
        sf::Vector2u(state_.size() / 2, state_.size() / 2);

    // Always update these two state variables when updating snake position.
    snake_positions_.push_back(snake_head_position);
    state_[snake_positions_.front().x][snake_positions_.front().y] =
        TileObject::Snake;

    // place the first point on the board
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> random(0, settings.grid_size - 1);
    point_pos_ = sf::Vector2u(random(generator), random(generator));
    state_[point_pos_.x][point_pos_.y] = TileObject::Pickup;

    // load high score
    if (std::ifstream input_score_file("high_score.txt");
        input_score_file.fail()) {
        high_score_ = 0;
    } else {
        std::stringstream buffer;
        buffer << input_score_file.rdbuf();
        std::string high_score = buffer.str();
        try {
            high_score_ = std::stoi(high_score);
        } catch (const std::invalid_argument& e) {
            high_score_ = 0;
        }
    }
}

void Game::move_snake() {
    auto new_snake_positions = std::vector(snake_positions_);

    switch (move_status_) {
        case MoveStatus::LEFT: {
            new_snake_positions[0] =
                sf::Vector2u(snake_positions_[0].x - 1, snake_positions_[0].y);

            for (size_t i = 1; i < snake_positions_.size(); i++) {
                new_snake_positions[i] = snake_positions_[i - 1];
            }
            break;
        }
        case MoveStatus::UP: {
            new_snake_positions[0] =
                sf::Vector2u(snake_positions_[0].x, snake_positions_[0].y - 1);
            for (size_t i = 1; i < snake_positions_.size(); i++) {
                new_snake_positions[i] = snake_positions_[i - 1];
            }
            break;
        }
        case MoveStatus::RIGHT: {
            new_snake_positions[0] =
                sf::Vector2u(snake_positions_[0].x + 1, snake_positions_[0].y);
            for (size_t i = 1; i < snake_positions_.size(); i++) {
                new_snake_positions[i] = snake_positions_[i - 1];
            }
            break;
        }
        case MoveStatus::DOWN: {
            new_snake_positions[0] =
                sf::Vector2u(snake_positions_[0].x, snake_positions_[0].y + 1);
            for (size_t i = 1; i < snake_positions_.size(); i++) {
                new_snake_positions[i] = snake_positions_[i - 1];
            }
            break;
        }
    }

    // LOST: out of bounds
    if (new_snake_positions[0].x < 0 || new_snake_positions[0].y < 0 ||
        new_snake_positions[0].x > settings_.grid_size - 1 ||
        new_snake_positions[0].y > settings_.grid_size - 1) {
        std::cout << "LOST (OUT OF BOUNDS)" << std::endl;
        game_over = true;
        return;
    }

    // LOST: snake hit itself
    for (size_t i = 1; i < new_snake_positions.size(); i++) {
        auto segment = new_snake_positions[i];
        auto head = new_snake_positions.front();

        if (head.x == segment.x && head.y == segment.y) {
            std::cout << "LOST (SNAKE HIT ITSELF)" << std::endl;
            game_over = true;
            return;
        }
    }

    // collected crystal
    if (new_snake_positions[0].x == point_pos_.x &&
        new_snake_positions[0].y == point_pos_.y) {
        std::cout << "COLLECT" << std::endl;

        score_++;
        if (score_ > high_score_) {
            high_score_ = score_;
        }

        // add new segment in place of the last one in the old snake_positions
        sf::Vector2u new_segment_position = snake_positions_.back();

        new_snake_positions.push_back(new_segment_position);

        new_crystal();
    }

    // clear old snake
    for (const auto& old_snake_pos : snake_positions_) {
        state_[old_snake_pos.x][old_snake_pos.y] = TileObject::Empty;
    }

    // update state with new snake
    snake_positions_ = new_snake_positions;
    for (const auto& snake_pos : snake_positions_) {
        state_[snake_pos.x][snake_pos.y] = TileObject::Snake;
    }
}

void Game::new_crystal() {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> random(0, settings_.grid_size - 1);

    unsigned int x;
    unsigned int y;

    do {
        x = random(generator);
        y = random(generator);
    } while (std::find(snake_positions_.begin(), snake_positions_.end(),
                       sf::Vector2u(x, y)) != snake_positions_.end());

    state_[x][y] = TileObject::Pickup;
    point_pos_ = sf::Vector2u(x, y);
}

bool Game::update() {
    if (!settings_.running) {
        window_.close();
        return false;
    }

    window_.clear(settings_.color_background);

    handle_logic();
    handle_input();
    render_grid();
    render_ui();

    if (game_over) {
        handle_game_over();
    } else if (paused) {
        pause_game();
        paused = false;
    }
    window_.display();

    return true;
}

void Game::reset_game() {
    /* reset game progress */
    score_ = 0;
    game_over = false;
    move_status_ = MoveStatus::LEFT;

    // fill the board with empty tiles
    for (size_t i = 0; i < settings_.grid_size; i++)
        std::fill(state_[i].begin(), state_[i].end(), TileObject::Empty);

    // place snake head on the board
    const auto snake_head_position =
        sf::Vector2u(state_.size() / 2, state_.size() / 2);

    // Always update these two state variables when updating snake position.
    snake_positions_.clear();
    snake_positions_.push_back(snake_head_position);
    state_[snake_positions_.front().x][snake_positions_.front().y] =
        TileObject::Snake;

    // place the first point on the board
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> random(0, settings_.grid_size - 1);
    point_pos_ = sf::Vector2u(random(generator), random(generator));
    state_[point_pos_.x][point_pos_.y] = TileObject::Pickup;
}

void Game::pause_game() {
    sf::Text pause_text(settings_.font);
    pause_text.setFillColor(settings_.color_ui);
    pause_text.setString("PAUSED!\n[P] to unpause.");
    pause_text.setOutlineColor(sf::Color::Black);
    pause_text.setOutlineThickness(1);
    pause_text.setStyle(sf::Text::Bold);
    pause_text.setPosition({
        (settings_.width - pause_text.getLocalBounds().position.x) / 2,
        (settings_.height - pause_text.getLocalBounds().position.y) / 2,
    });
    window_.draw(pause_text);
    window_.display();

    while (window_.isOpen()) {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
                settings_.running = false;
                return;
            } else if (const auto* keyPressed =
                           event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->scancode) {
                    case sf::Keyboard::Scan::P:
                        return;
                    default:
                        break;
                }
            } else {
                break;
            }
        }
    }
}

void Game::handle_game_over() {
    // prepare the "game over" message
    sf::Text game_over_text(settings_.font);
    game_over_text.setFillColor(settings_.color_ui);
    game_over_text.setString("GAME OVER!\n[R] to restart,\n[ESC/Q] to quit.");
    game_over_text.setOutlineColor(sf::Color::Black);
    game_over_text.setOutlineThickness(1);
    game_over_text.setStyle(sf::Text::Bold);
    game_over_text.setPosition({
        (settings_.width - game_over_text.getLocalBounds().position.x) / 2,
        (settings_.height - game_over_text.getLocalBounds().position.x) / 2,
    });
    window_.draw(game_over_text);
    window_.display();

    /* make user stuck in the game over screen until
     * they press a key
     */
    while (window_.isOpen()) {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
                settings_.running = false;
                return;
            }
            if (const auto* keyPressed =
                    event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->scancode) {
                    case sf::Keyboard::Scan::Q:
                    case sf::Keyboard::Scan::Escape:
                    case sf::Keyboard::Scan::Enter:
                        reset_game();
                        open_menu();
                        return;
                    case sf::Keyboard::Scan::R:
                        reset_game();
                        return;
                    default:
                        break;
                }
            } else {
                break;
            }
        }
    }
}

void Game::open_menu() {
    Menu menu(settings_.width, settings_.height, settings_.font);
    while (window_.isOpen()) {
        while (const std::optional event = window_.pollEvent()) {
            if (const auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                bool leave_menu = false;
                handle_menu_input(*keyPressed, menu, leave_menu);
                if (leave_menu) {
                    return;
                }
            } else {
                break;
            }
        }
        sf::Color midnight_blue(1, 3, 50);
        window_.clear(midnight_blue);
        menu.draw(window_);
        window_.display();
    }
}

void Game::handle_menu_input(const sf::Event::KeyPressed& event,
                             Menu& menu,
                             bool& leave_menu) {
    switch (event.scancode) {
        case sf::Keyboard::Scan::Up:
            menu.move_up();
            break;
        case sf::Keyboard::Scan::Down:
            menu.move_down();
            break;
        case sf::Keyboard::Scan::Q:
            settings_.running = false;
            leave_menu = true;
            return;
        case sf::Keyboard::Scan::Enter:
            switch (menu.get_selected_action()) {
                case Menu::Action::PLAY:
                    leave_menu = true;
                    return;
                case Menu::Action::INFO:
                    menu.open_info(window_);
                    break;
                case Menu::Action::QUIT:
                    settings_.running = false;
                    leave_menu = true;
                default:
                    break;
            }
        default:
            break;
    }
}

void Game::handle_logic() {
    const auto new_timestamp =
        std::chrono::system_clock::now().time_since_epoch() /
        std::chrono::milliseconds(settings_.interval_ms);

    if (new_timestamp > timestamp_) {
        move_snake();
    }

    // post-update
    timestamp_ = new_timestamp;
}

void Game::handle_input() {
    while (const std::optional event = window_.pollEvent()) {
        if (const auto resized = event->getIf<sf::Event::Resized>()) {
            settings_.width = resized->size.x;
            settings_.height = resized->size.y;
            sf::FloatRect visible_area({0, 0},
                                       {
                                           static_cast<float>(resized->size.x),
                                           static_cast<float>(resized->size.y),
                                       });
            window_.setView(sf::View(visible_area));
            std::cout << "snake: window resized: " << resized->size.x << "x"
                      << resized->size.y << std::endl;
        } else if (event->is<sf::Event::Closed>()) {
            settings_.running = false;
        } else if (const auto keyPressed =
                       event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scan::Left) {
                move_status_ = MoveStatus::LEFT;
            } else if (keyPressed->scancode == sf::Keyboard::Scan::Up) {
                move_status_ = MoveStatus::UP;
            } else if (keyPressed->scancode == sf::Keyboard::Scan::Right) {
                move_status_ = MoveStatus::RIGHT;
            } else if (keyPressed->scancode == sf::Keyboard::Scan::Down) {
                move_status_ = MoveStatus::DOWN;
            } else if (keyPressed->scancode == sf::Keyboard::Scan::P) {
                paused = true;
            }
        }
    }
}

void Game::render_grid() {
    auto width = static_cast<float>(settings_.width / settings_.grid_size);
    auto height = static_cast<float>(settings_.height / settings_.grid_size);

    for (size_t i = 0; i < settings_.grid_size; i++) {
        for (size_t j = 0; j < settings_.grid_size; j++) {
            float x = width * i;
            float y = height * j;
            const auto tileObject = state_[i][j];

            Color color = settings_.color_background;
            if (tileObject == TileObject::Snake) {
                color = settings_.color_snake;
            }
            if (tileObject == TileObject::Pickup) {
                color = settings_.color_pickup;
            }

            auto tile = sf::RectangleShape(sf::Vector2f(width, height));
            tile.setPosition({x, y});
            tile.setFillColor(color);
            window_.draw(tile);
        }
    }
}

void Game::render_ui() {
    // render score text
    sf::Text score_text(settings_.font);

    std::string score;
    score.append("Score: ");
    score.append(std::to_string(score_));
    score_text.setString(score);

    score_text.setCharacterSize(24);
    score_text.setFillColor(settings_.color_ui);
    score_text.setOutlineColor(sf::Color::Black);
    score_text.setOutlineThickness(1);
    score_text.setStyle(sf::Text::Bold);
    score_text.setPosition(sf::Vector2f(32, 32));
    window_.draw(score_text);

    // render high score text
    sf::Text high_score_text(settings_.font);

    std::string high_score;
    high_score.append("High score: ");
    high_score.append(std::to_string(high_score_));
    high_score_text.setString(high_score);

    high_score_text.setCharacterSize(24);
    high_score_text.setFillColor(settings_.color_ui);
    high_score_text.setOutlineColor(sf::Color::Black);
    high_score_text.setOutlineThickness(1);
    high_score_text.setStyle(sf::Text::Bold);
    high_score_text.setPosition(sf::Vector2f(
        (settings_.width - high_score_text.getLocalBounds().size.x) / 2, 32));
    window_.draw(high_score_text);
}

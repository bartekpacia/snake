#include "../include/game.h"

Game::Game(const GameSettings& settings) {
    settings_ = settings;

    sf::ContextSettings window_settings;
    window_settings.antialiasingLevel = settings_.antialiasing;

    if (!settings_.font.loadFromFile("arial.ttf")) {
        std::clog << "snake: error loading font file" << std::endl;
    }

    window_.create(sf::VideoMode(settings_.width, settings_.height), "snake",
                   sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize,
                   window_settings);

    move_status_ = MoveStatus::LEFT;

    timestamp_ = std::chrono::system_clock::now().time_since_epoch() /
                 std::chrono::seconds(1);

    // fill the board with empty tiles
    for (auto i = 0; i < settings.grid_size; i++) {
        std::vector<TileObject> row;
        for (auto j = 0; j < settings.grid_size; j++) {
            row.push_back(TileObject::Empty);
        }
        state_.push_back(row);
    }

    // place snake head on the board
    auto snake_head_position =
        sf::Vector2i(state_.size() / 2, state_.size() / 2);

    // Always update these two state variables when updating snake position.
    snake_positions_.push_back(snake_head_position);
    state_[snake_positions_.front().x][snake_positions_.front().y] =
        TileObject::Snake;

    std::cout << "snake: x: " << snake_positions_.front().x
              << ", y:" << snake_positions_.front().y << std::endl;

    // place the first point on the board
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> random(0, settings.grid_size - 1);
    point_pos_ = sf::Vector2i(random(generator), random(generator));
    state_[point_pos_.x][point_pos_.y] = TileObject::Pickup;

    std::cout << "point: x: " << point_pos_.x << ", y:" << point_pos_.y
              << std::endl;
}

void Game::move_snake() {
    std::vector<sf::Vector2i> new_snake_positions =
        std::vector(snake_positions_);

    switch (move_status_) {
        case MoveStatus::LEFT: {
            new_snake_positions[0] =
                sf::Vector2i(snake_positions_[0].x - 1, snake_positions_[0].y);
            break;
        }
        case MoveStatus::UP: {
            new_snake_positions[0] =
                sf::Vector2i(snake_positions_[0].x, snake_positions_[0].y - 1);
            break;
        }
        case MoveStatus::RIGHT: {
            new_snake_positions[0] =
                sf::Vector2i(snake_positions_[0].x + 1, snake_positions_[0].y);
            break;
        }
        case MoveStatus::DOWN: {
            new_snake_positions[0] =
                sf::Vector2i(snake_positions_[0].x, snake_positions_[0].y + 1);
            break;
        }
    }

    // out of bounds
    if (new_snake_positions[0].x < 0 || new_snake_positions[0].y < 0 ||
        new_snake_positions[0].x > settings_.grid_size - 1 ||
        new_snake_positions[0].y > settings_.grid_size - 1) {
        std::cout << "LOST (OUT OF BOUNDS)" << std::endl;
        settings_.running = false;
        return;
    }

    // collected crystal
    if (new_snake_positions[0].x == point_pos_.x &&
        new_snake_positions[0].y == point_pos_.y) {
        std::cout << "COLLECT" << std::endl;

        score_++;
        new_crystal();
    }

    // clear old snake
    for (auto& old_snake_pos : snake_positions_) {
        state_[old_snake_pos.x][old_snake_pos.y] = TileObject::Empty;
    }

    // update state with new snake
    snake_positions_ = new_snake_positions;
    for (auto& snake_pos : snake_positions_) {
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
                       sf::Vector2i(x, y)) !=
             snake_positions_
                 .end());  // snake_positions_.fin x == snake_positions_.x && y
                           // == snake_positions_.y);

    state_[x][y] = TileObject::Pickup;
    point_pos_ = sf::Vector2i(x, y);
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

    window_.display();
    sf::sleep(sf::milliseconds(settings_.sleep_time_ms));

    // std::cout << "snake x:" << snake_positions_.x << ", y:" <<
    // snake_positions_.y
    //           << std::endl;

    return true;
}

void Game::handle_logic() {
    auto new_timestamp = std::chrono::system_clock::now().time_since_epoch() /
                         std::chrono::milliseconds(500);

    if (new_timestamp > timestamp_) {
        std::cout << static_cast<int>(move_status_) << std::endl;
        move_snake();
    }

    // post-update
    timestamp_ = new_timestamp;
}

void Game::handle_input() {
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
            if (event.key.code == sf::Keyboard::Left)
                move_status_ = MoveStatus::LEFT;
            if (event.key.code == sf::Keyboard::Up)
                move_status_ = MoveStatus::UP;
            if (event.key.code == sf::Keyboard::Right)
                move_status_ = MoveStatus::RIGHT;
            if (event.key.code == sf::Keyboard::Down)
                move_status_ = MoveStatus::DOWN;
        }
    }
}

void Game::render_grid() {
    double width = settings_.width / settings_.grid_size;
    double height = settings_.height / settings_.grid_size;

    for (int i = 0; i < settings_.grid_size; i++) {
        for (int j = 0; j < settings_.grid_size; j++) {
            auto x = width * i;
            auto y = height * j;
            auto tileObject = state_[i][j];

            Color color = settings_.color_background;
            if (tileObject == TileObject::Snake) {
                color = settings_.color_snake;
            }
            if (tileObject == TileObject::Pickup) {
                color = settings_.color_pickup;
            }

            auto tile = sf::RectangleShape(sf::Vector2f(width, height));
            tile.setPosition(x, y);
            tile.setFillColor(color);
            window_.draw(tile);
        }
    }
}

void Game::render_ui() {
    sf::Text text;

    text.setFont(settings_.font);

    // set the string to display
    std::string s;
    s.append("Score: ");
    s.append(std::to_string(score_));
    text.setString(s);

    text.setCharacterSize(24);  // in pixels, not points!
    text.setFillColor(settings_.color_ui);
    text.setStyle(sf::Text::Bold);
    text.setPosition(32, 32);
    window_.draw(text);
}

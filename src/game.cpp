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

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> random(0, settings.grid_size - 1);

    // place snake head on the board
    snake_head_pos_ = sf::Vector2i(random(generator), random(generator));
    state_[snake_head_pos_.x][snake_head_pos_.y] = TileObject::Snake;

    std::cout << "snake: x: " << snake_head_pos_.x
              << ", y:" << snake_head_pos_.y << std::endl;

    // place the first point on the board
    point_pos_ = sf::Vector2i(random(generator), random(generator));
    state_[point_pos_.x][point_pos_.y] = TileObject::Point;

    std::cout << "point: x: " << point_pos_.x << ", y:" << point_pos_.y
              << std::endl;
}

void Game::move_snake() {
    sf::Vector2i snake_new_head_pos;

    switch (move_status_) {
        case MoveStatus::LEFT: {
            snake_new_head_pos =
                sf::Vector2i(snake_head_pos_.x - 1, snake_head_pos_.y);
            break;
        }
        case MoveStatus::UP: {
            snake_new_head_pos =
                sf::Vector2i(snake_head_pos_.x, snake_head_pos_.y - 1);
            break;
        }
        case MoveStatus::RIGHT: {
            snake_new_head_pos =
                sf::Vector2i(snake_head_pos_.x + 1, snake_head_pos_.y);
            break;
        }
        case MoveStatus::DOWN: {
            snake_new_head_pos =
                sf::Vector2i(snake_head_pos_.x, snake_head_pos_.y + 1);
            break;
        }
    }

    // out of bounds
    if (snake_new_head_pos.x < 0 || snake_new_head_pos.y < 0 ||
        snake_new_head_pos.x > settings_.grid_size - 1 ||
        snake_new_head_pos.y > settings_.grid_size - 1) {
        std::cout << "LOST (OUT OF BOUNDS)" << std::endl;
        settings_.running = false;
    }

    state_[snake_head_pos_.x][snake_head_pos_.y] = TileObject::Empty;
    snake_head_pos_ = snake_new_head_pos;
    state_[snake_head_pos_.x][snake_head_pos_.y] = TileObject::Snake;
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

    window_.display();
    sf::sleep(sf::milliseconds(settings_.sleep_time_ms));
    return true;
}

void Game::handle_logic() {
    auto new_timestamp = std::chrono::system_clock::now().time_since_epoch() /
                         std::chrono::seconds(1);

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
    double grid_size = settings_.grid_size;
    double width = settings_.width / grid_size;
    double height = settings_.height / grid_size;

    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            auto x = width * i;
            auto y = height * j;
            auto tileObject = state_[i][j];

            Color color = Color::Black;
            if (tileObject == TileObject::Snake) {
                color = Color::Green;
            }
            if (tileObject == TileObject::Point) {
                color = Color::Yellow;
            }

            auto tile = sf::RectangleShape(sf::Vector2f(width, height));
            tile.setPosition(x, y);
            tile.setFillColor(color);
            window_.draw(tile);
        }
    }
}

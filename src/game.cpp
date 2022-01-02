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

    move_status_ = MoveStatus::UP;

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
    int snake_initial_i = random(generator);
    int snake_initial_j = random(generator);
    state_[snake_initial_i][snake_initial_j] = TileObject::Snake;

    // place the first point on the board
    int point_initial_i = random(generator);
    int point_initial_j = random(generator);
    state_[point_initial_i][point_initial_j] = TileObject::Point;
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

        switch (move_status_) {
            case MoveStatus::LEFT:

                break;
            case MoveStatus::UP:
                break;
            case MoveStatus::RIGHT:
                break;
            case MoveStatus::DOWN:
                break;
        }
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
            auto x = width * j;
            auto y = height * i;

            Color color = Color::Black;
            if (state_[i][j] == TileObject::Snake) {
                color = Color::Green;
            }
            if (state_[i][j] == TileObject::Point) {
                color = Color::Yellow;
            }

            auto tile = sf::RectangleShape(sf::Vector2f(width, height));
            tile.setPosition(x, y);
            tile.setFillColor(color);
            window_.draw(tile);

            auto point = sf::Vertex(sf::Vector2f(x, y), color);
            window_.draw(&point, 1, sf::Points);
        }
    }
}

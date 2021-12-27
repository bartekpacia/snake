#include "../include/app.h"

void debug() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last
        // iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Red);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }
}

int main(int argc, char** argv) {
    // debug();

    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }
    return App(args).run();
}

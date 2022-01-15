#include "../include/app.h"
#include <algorithm>

App::App(std::vector<std::string>& args) {
    running_ = true;
    if (!init(args)) {
        running_ = false;
    }
}

App::~App() = default;

int App::run() {
    while (running_) {
        if (running_ && game_) {
            bool ok = game_->update();
            if (!ok) {
                running_ = false;
            }
        }
    }

    std::ofstream high_score_output("high_score.txt");
    high_score_output << game_->high_score();
    high_score_output.close();

    return 0;
}

void App::print_help() {
    std::cout
        << "-----------------------------------------------------------\n"
        << "snake -- simple snake game written in SFML \n"
        << "-----------------------------------------------------------\n"
        << "Source: https://github.com/bartekpacia/snake\n"
        << "\n"
        << "\tOptions:\n"
        << "\t-h  --height [val]             window height (default: 720)\n"
        << "\t-w  --width [val]              window width (default: 720)\n"
        << "\t-ms  --interval [val]          interval (ms) (default: 500)\n"
        << "\t-sz  --grid-size [val]         grid size (default: 16)\n";
}

bool App::is_flag_present(std::vector<std::string>& all_args,
                          const std::string& short_arg,
                          const std::string& long_arg) {
    auto it = std::find_if(all_args.begin(), all_args.end(),
                           [short_arg, long_arg](const std::string& s) {
                               return s == short_arg || s == long_arg;
                           });

    if (it == all_args.end()) {
        return false;
    }

    all_args.erase(it);
    return true;
}

std::string App::get_flag_value(std::vector<std::string>& all_args,
                                const std::string& short_arg,
                                const std::string& long_arg,
                                const std::string& default_value) {
    auto it = std::find_if(all_args.begin(), all_args.end(),
                           [short_arg, long_arg](const std::string& s) {
                               return s == short_arg || s == long_arg;
                           });

    std::string value = default_value;
    if (it == all_args.end()) {
    } else if (it + 1 == all_args.end()) {
        all_args.erase(it);
    } else {
        value = *(it + 1);
        all_args.erase(it, it + 2);
    }
    return value;
}

bool App::init(std::vector<std::string>& args) {
    // print help
    if (is_flag_present(args, "-h", "--help")) {
        print_help();
        return false;
    }

    // Initialize the GUI
    GameSettings settings;

    std::stringstream(get_flag_value(args, "-w", "--width")) >> settings.width;
    std::stringstream(get_flag_value(args, "-h", "--height")) >>
        settings.height;
    std::stringstream(get_flag_value(args, "-sz", "--grid-size")) >>
        settings.grid_size;
    std::stringstream(get_flag_value(args, "-ms", "--interval")) >>
        settings.interval_ms;

    game_ = std::make_unique<Game>(settings);

    return true;
}

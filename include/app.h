#pragma once
#include "game.h"

class App {
   public:
    explicit App(std::vector<std::string>& args);
    ~App();

    int run();
    static void print_help();

   private:
    static bool is_flag_present(std::vector<std::string>& all_args,
                                const std::string& short_arg,
                                const std::string& long_arg);

    static std::string get_flag_value(std::vector<std::string>& all_args,
                                      const std::string& short_arg,
                                      const std::string& long_arg,
                                      const std::string& default_value = "");

    /**
     * Init performs command-line argument and flag parsing. Then it initializes
     * core components of the game.
     * @param args command-line arguments and flags
     * @return true if init was successful successful, false otherwise
     */
    bool init(std::vector<std::string>& args);

    bool running_;
    std::unique_ptr<Game> gui_;
};

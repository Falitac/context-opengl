#include "../include/App.hpp"
#include <iostream>
#include <exception>


int main(int argc, char** argv) {
    App app;
    try {
        app.run();
    } catch(const std::exception& error) {
        std::cerr << "An error occured: " << error.what() << std::endl;
    } catch(...) {
        std::cerr << "unknown error\n";
    }

    return EXIT_SUCCESS;
}
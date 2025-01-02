#include <iostream>
#include <thread>
#include "Cool/spawn_process.hpp"

int main()
{
    using namespace std::literals;

    {
        std::cout << "Start spawning process\n";
        auto const maybe_err = Cool::spawn_process(EXECUTABLE_PATH);
        if (maybe_err)
            std::cerr << "ERR" << *maybe_err << '\n';
        std::cout << "Done spawning process\n";
        std::this_thread::sleep_for(6s);
    }

    {
        std::cout << "Start spawning process\n";
        auto const maybe_err = Cool::spawn_process(EXECUTABLE_PATH, {"Hello"});
        if (maybe_err)
            std::cerr << "ERR" << *maybe_err << '\n';
        std::cout << "Done spawning process\n";
        std::this_thread::sleep_for(6s);
    }

    {
        std::cout << "Start spawning process\n";
        auto const maybe_err = Cool::spawn_process(EXECUTABLE_PATH, {"Hello", "World"});
        if (maybe_err)
            std::cerr << "ERR" << *maybe_err << '\n';
        std::cout << "Done spawning process\n";
        std::this_thread::sleep_for(6s);
    }

    {
        std::cout << "Start spawning process\n";
        auto const maybe_err = Cool::spawn_process(EXECUTABLE_PATH, {"Hello", "World", "two words"});
        if (maybe_err)
            std::cerr << "ERR" << *maybe_err << '\n';
        std::cout << "Done spawning process\n";
        std::this_thread::sleep_for(6s);
    }

    {
        std::cout << "Start spawning process\n";
        auto const maybe_err = Cool::spawn_process(EXECUTABLE_PATH, {"Hello", "World", "\\\"two words\\\""}); // NB: this will output something different on Windows vs Linux/Mac
        if (maybe_err)
            std::cerr << "ERR" << *maybe_err << '\n';
        std::cout << "Done spawning process\n";
        std::this_thread::sleep_for(6s);
    }
}
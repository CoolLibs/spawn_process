#include <iostream>
#include <thread>
#include "Cool/spawn_process.hpp"

int main()
{
    using namespace std::literals;

    std::cout << "Start spawning process\n";
    Cool::spawn_process(EXECUTABLE_PATH);
    std::cout << "Done spawning process\n";

    std::this_thread::sleep_for(6s);

    std::cout << "Start spawning process\n";
    Cool::spawn_process(EXECUTABLE_PATH, {"Hello"});
    std::cout << "Done spawning process\n";

    std::this_thread::sleep_for(6s);

    std::cout << "Start spawning process\n";
    Cool::spawn_process(EXECUTABLE_PATH, {"Hello", "World"});
    std::cout << "Done spawning process\n";

    std::this_thread::sleep_for(6s);

    std::cout << "Start spawning process\n";
    Cool::spawn_process(EXECUTABLE_PATH, {"Hello", "World", "two words"});
    std::cout << "Done spawning process\n";

    std::this_thread::sleep_for(6s);

    std::cout << "Start spawning process\n";
    Cool::spawn_process(EXECUTABLE_PATH, {"Hello", "World", "\\\"two words\\\""});
    std::cout << "Done spawning process\n";

    std::this_thread::sleep_for(6s);
}
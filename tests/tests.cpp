#include <iostream>
#include "Cool/spawn_process.hpp"

int main()
{
    std::cout << "Start spawning process\n";
    Cool::spawn_process(EXECUTABLE_PATH);
    std::cout << "Done spawning process\n";
}
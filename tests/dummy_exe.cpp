#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
        std::cout << "Command-line arg: '" << argv[i] << "'\n";

    int prev_sec{};
    int count{0};
    while (count < 5)
    {
        std::time_t const    current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm const* const local_time   = std::localtime(&current_time);
        if (prev_sec != local_time->tm_sec)
        {
            prev_sec = local_time->tm_sec;
            count++;
            std::cout << "Current time: "
                      << std::setfill('0') << std::setw(2) << local_time->tm_hour << ":"
                      << std::setfill('0') << std::setw(2) << local_time->tm_min << ":"
                      << std::setfill('0') << std::setw(2) << local_time->tm_sec
                      << '\n';
        }
    }
    std::cout << "Exiting process\n";
}
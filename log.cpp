#include "log.h"

void Log::info(std::string expr)
{
    std::cout << expr << '\n';
};

void Log::warn(std::string expr)
{
    std::cerr << expr << '\n';
};

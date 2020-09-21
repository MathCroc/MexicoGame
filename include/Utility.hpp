#pragma once

#include <cstdarg>
#include <iostream>
#include <utility>

namespace mexico {

using Roll = std::pair<int, int>;

constexpr int mexicoScore = 1000;

int getRollScore(const Roll& roll)
{
    if (roll.first < roll.second)
        return getRollScore({ roll.second, roll.first });

    if (roll.first == 2 and roll.second == 1)
        return mexicoScore;

    int score = roll.first * 10 + roll.second;
    if (roll.first == roll.second)
        score += 100;
    return score;
}

struct Logger
{
    static bool enable;

    static void print(const char* fmt, ...)
    {
        if (not enable)
            return;
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        std::cout << std::endl;
    }
};

class LoggerScope
{
public:
    LoggerScope(bool enable) : previous(Logger::enable) { Logger::enable = enable; }
    ~LoggerScope() { Logger::enable = previous; }

private:
    bool previous;
};
} // namespace mexico

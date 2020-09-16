#pragma once

#include <utility>

namespace mexico {

using Roll = std::pair<int, int>;

constexpr int mexicoScore = 1000;

int getRollScore(const Roll& roll)
{
    if (roll.first < roll.second)
        getRollScore({ roll.second, roll.first });

    if (roll.first == 2 and roll.second == 1)
        return mexicoScore;

    int score = roll.first * 10 + roll.second;
    if (roll.first == roll.second)
        score += 100;
    return score;
}
} // namespace mexico

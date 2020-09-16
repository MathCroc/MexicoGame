#pragma once

#include <random>
#include <string>

#include "Utility.hpp"

namespace mexico {

class Player
{
public:
    static constexpr int maxRollsPerTurn{ 3 };

    virtual void prepareForRound() = 0;
    virtual void observe(const Roll& roll) = 0;
    virtual std::string getName() const = 0;

    Roll takeTurn()
    {
        Roll roll{};
        for (int i = 0; i < maxRollsPerTurn; ++i)
        {
            roll = std::make_pair(rollDice(), rollDice());
            if (stay(roll))
                break;
        }
        return roll;
    }

    void reduceLifeCount(int count) { lifeCount -= count; }
    int getLifeCount() const { return lifeCount; }

protected:
    virtual bool stay(const Roll& roll) = 0;

private:
    int rollDice()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(1, 6);
        return distribution(gen);
    }

    int lifeCount{ 1 };
};
} // namespace mexico

#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#include "Player.hpp"
#include "Utility.hpp"

namespace mexico {

class GameMaster
{
public:
    GameMaster() = default;

    template <typename PlayerT, typename... Args>
    void registerToGame(Args&&... args)
    {
        players.push_back(std::make_unique<PlayerT>(std::forward<Args>(args)...));
    }

    void playGame()
    {
        shufflePlayers();
        while (players.size() > 1)
        {
            std::rotate(players.begin(), players.begin() + 1, players.end());
            playOneRound();
            eliminateLosers();
        }

        if (players.empty())
        {
            std::cout << "Nobody won" << std::endl;
        }

        std::cout << "Winner is: " << players.front()->getName() << "!!!" << std::endl;
    }

private:
    using Players = std::vector<std::unique_ptr<Player>>;

    void shufflePlayers()
    {
        std::random_device rng;
        std::mt19937 urng(rng());
        std::shuffle(players.begin(), players.end(), urng);
    }

    void playOneRound()
    {
        std::vector<Players::iterator> worstPlayers;
        int worstScore{ mexicoScore + 1 };
        int livesOnTable{ 1 };

        std::for_each(
            players.begin(), players.end(), [](auto& player) { player->prepareForRound(); });
        for (auto playerIt = players.begin(); playerIt != players.end(); ++playerIt)
        {
            const Roll roll = (*playerIt)->takeTurn();
            std::for_each(std::next(playerIt), players.end(), [&roll](auto& player) {
                player->observe(roll);
            });

            const int rollScore = getRollScore(roll);
            if (rollScore == mexicoScore)
                livesOnTable = 2;
            if (rollScore <= worstScore)
            {
                if (rollScore < worstScore)
                {
                    worstPlayers.clear();
                    worstScore = rollScore;
                }
                worstPlayers.push_back(playerIt);
            }
        }

        std::for_each(worstPlayers.begin(), worstPlayers.end(), [livesOnTable](auto it) {
            (*it)->reduceLifeCount(livesOnTable);
        });
    }

    void eliminateLosers()
    {
        players.erase(std::remove_if(players.begin(), players.end(), [](const auto& player) {
            return player->getLifeCount() <= 0;
        }));
    }

    Players players;
};
} // namespace mexico

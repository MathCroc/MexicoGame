#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
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

    void playTournament()
    {
        constexpr int numGames = 1000;

        {
            LoggerScope scope(false);

            for (int i = 0; i < numGames; ++i)
                playGame();
        }
        {
            LoggerScope scope(true);
            Logger::print("### Results ###");
            for (const auto& [name, wins] : stats)
            {
                Logger::print("%s: %.1f", name.c_str(), 100.0 * ((float)wins / numGames));
            }
        }
    }

    void playGame()
    {
        addPlayers();
        shufflePlayers();
        while (players.size() > 1)
        {
            std::rotate(players.begin(), players.begin() + 1, players.end());
            playOneRound();
            eliminateLosers();
        }

        if (players.empty())
        {
            Logger::print("Nobody won");
            return;
        }

        stats[players.front()->getName()]++;

        Logger::print("Winner is: %s!!!", players.front()->getName().c_str());
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
        Logger::print("#### Start new round ####");

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

            Logger::print("Player %s stayed on (%d, %d)",
                          (*playerIt)->getName().c_str(),
                          roll.first,
                          roll.second);

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

    void addPlayers()
    {
        std::move(eliminated.begin(), eliminated.end(), std::back_inserter(players));
        eliminated.clear();
        std::for_each(players.begin(), players.end(), [](auto& player) { player->reset(); });
    }

    void eliminateLosers()
    {
        for (auto it = players.begin(); it != players.end();)
        {
            Logger::print(
                "Player %s has %d lives left", (*it)->getName().c_str(), (*it)->getLifeCount());
            if ((*it)->getLifeCount() <= 0)
            {
                it = eliminatePlayer(it);
            }
            else
            {
                ++it;
            }
        }
    }

    Players::iterator eliminatePlayer(Players::iterator it)
    {
        if (it == players.end())
            return it;

        eliminated.push_back(std::move(*it));
        return players.erase(it);
    }

    Players players;
    Players eliminated;
    std::unordered_map<std::string, int> stats;
};
} // namespace mexico

#include <vector>

#include "DummyPlayer.hpp"
#include "GameMaster.hpp"
#include "GreedyPlayer.hpp"
#include "Player.hpp"
#include "Utility.hpp"

using namespace mexico;

bool Logger::enable = true;

int main()
{
    GameMaster gameMaster;
    gameMaster.registerToGame<DummyPlayer>("A");
    gameMaster.registerToGame<DummyPlayer>("B");
    gameMaster.registerToGame<GreedyPlayer>("C");
    gameMaster.playTournament();
}

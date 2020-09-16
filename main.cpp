#include <vector>

#include "DummyPlayer.hpp"
#include "GameMaster.hpp"
#include "Player.hpp"

using namespace mexico;

int main()
{
    GameMaster gameMaster;
    gameMaster.registerToGame<DummyPlayer>("A");
    gameMaster.registerToGame<DummyPlayer>("B");
    gameMaster.playGame();
}

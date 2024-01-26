#include "Game.h"

int main(int argc, char *args[]) {
    while (Game::GetInstance().IsRunning()){
        Game::GetInstance().Update();
        Game::GetInstance().Render();
        Game::GetInstance().HandleEvents();
    }

    Game::GetInstance().Clean();
    return 0;
}

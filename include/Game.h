//
// Created by pylinskyi.k on 25.01.2024.
//

#include "SDL.h"
#include "entites/objects/Block.h"

class Game {
public:
    static Game& GetInstance();

    bool Init();
    bool Clean();
    void Quit();

    void Update();
    void Render();
    void HandleEvents();

    SDL_Renderer* GetRenderer() const { return _renderer; }
    SDL_Window* GetWindow() const { return _window; }
    bool IsRunning() const { return _isRunning; }

private:
    Game();

    static Game* _instance;

    SDL_Window* _window;
    SDL_Renderer* _renderer;

    bool _isRunning;

    Block* _block;
};

#ifndef SDL_TETRIS_GAME_H
#define SDL_TETRIS_GAME_H

#endif //SDL_TETRIS_GAME_H

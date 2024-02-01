//
// Created by pylinskyi.k on 25.01.2024.
//

#include "Game.h"
#include "SDL_image.h"
#include "systems/InputSystem.h"

Game* Game::_instance = nullptr;

Game::Game() {
    _isRunning = Init();
}

Game &Game::GetInstance() {
    if (!_instance) _instance = new Game();
    return *_instance;
}

bool Game::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Failed initialize SDL: %s", SDL_GetError());
        return false;
    }

    _window = SDL_CreateWindow("SDL Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 200, 0);
    if (!_window) {
        SDL_Log("Failed initialize Window: %s", SDL_GetError());
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_renderer) {
        SDL_Log("Failed initialize Renderer: %s", SDL_GetError());
        return false;
    }

    _level = new Level(20, 8, 8, 2);

    return true;
}

bool Game::Clean() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    IMG_Quit();
    SDL_Quit();
    return false;
}

void Game::Quit() {
    _isRunning = false;
}

void Game::Update() {
    _level->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    _level->Render();
    SDL_RenderPresent(_renderer);
}

void Game::HandleEvents() {
    InputSystem::GetInstance().Listen();
}
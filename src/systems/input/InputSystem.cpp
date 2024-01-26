//
// Created by pylinskyi.k on 25.01.2024.
//

#include "systems/input/InputSystem.h"
#include "SDL.h"
#include "Game.h"

InputSystem* InputSystem::_instance = nullptr;

InputSystem::InputSystem() {
    // Initialize all keys to released state
    const Uint8* initialKeyStates = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        _keyStates[static_cast<SDL_Scancode>(i)] = initialKeyStates[i] == 1;
    }
}

InputSystem &InputSystem::GetInstance() {
    if (!_instance) _instance = new InputSystem();
    return *_instance;
}


void InputSystem::Listen() {
    SDL_Event event;

    while (SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_QUIT:
                Game::GetInstance().Quit();
                break;
            case SDL_KEYDOWN:
                HandleKeyDown(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                HandleKeyUp(event.key.keysym.scancode);
                break;
        }
    }
}

void InputSystem::HandleKeyDown(SDL_Scancode key) {
    _keyStates[key] = true;
}

void InputSystem::HandleKeyUp(SDL_Scancode key) {
    _keyStates[key] = false;
}

bool InputSystem::IsKeyDown(SDL_Scancode key) {
    return _keyStates[key];
}
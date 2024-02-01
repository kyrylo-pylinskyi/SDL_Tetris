//
// Created by pylinskyi.k on 25.01.2024.
//

#include <map>
#include <cstdint>
#include "SDL_scancode.h"

class InputSystem {
public:
    static InputSystem& GetInstance();

    void Listen();
    bool IsKeyDown(SDL_Scancode key);
    std::int8_t GetSteer();
    bool GetRotate();

private:
    InputSystem();
    static InputSystem* _instance;

    std::map<SDL_Scancode, bool> _keyStates;

    void HandleKeyDown(SDL_Scancode key);
    void HandleKeyUp(SDL_Scancode key);

};

#ifndef SDL_TETRIS_INPUTSYSTEM_H
#define SDL_TETRIS_INPUTSYSTEM_H

#endif //SDL_TETRIS_INPUTSYSTEM_H

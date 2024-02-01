//
// Created by pylinskyi.k on 26.01.2024.
//

#ifndef SDL_TETRIS_RIGIDBODY_H
#define SDL_TETRIS_RIGIDBODY_H

#include <cstdint>
#include <vector>
#include "CoreTypes.h"
#include "Position.h"

class RigidBody {
public:
    RigidBody(std::uint8_t x, std::uint8_t y, std::uint8_t width, std::uint8_t height);

    Position& GetPosition() const { return *_position; }

    void Fall();
    void SteerLeft();
    void SteerRight();
    void ResetPosition(std::uint8_t maxY);

private:
    Position* _position;
    std::uint8_t _width;
    std::uint8_t _height;
};

#endif //SDL_TETRIS_RIGIDBODY_H

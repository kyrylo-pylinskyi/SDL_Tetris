//
// Created by pylinskyi.k on 25.01.2024.
//

#ifndef SDL_TETRIS_OBJECT_H
#define SDL_TETRIS_OBJECT_H

#include "math/Vector2D.h"

class Object {
public:
    Object(int x, int y);

protected:
    Vector2D* _position;
};

#endif //SDL_TETRIS_OBJECT_H

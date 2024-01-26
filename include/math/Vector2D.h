//
// Created by pylinskyi.k on 25.01.2024.
//

#ifndef SDL_TETRIS_VECTOR2D_H
#define SDL_TETRIS_VECTOR2D_H

class Vector2D {
public:
    Vector2D(int x = 0, int y = 0) : X(x), Y(y) {}

    int GetX() const { return X; }
    int GetY() const { return Y; }

    void SetX(int x) { X = x; }
    void SetY(int y) { Y = y; }

private:
    int X;
    int Y;
};

#endif //SDL_TETRIS_VECTOR2D_H

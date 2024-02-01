//
// Created by pylinskyi.k on 25.01.2024.
//

#ifndef SDL_TETRIS_POSITION_H
#define SDL_TETRIS_POSITION_H

class Position {
public:
    Position(std::uint8_t x = 0, std::uint8_t y = 0) : X(x), Y(y) {}

    std::uint8_t GetX() const { return X; }
    std::uint8_t GetY() const { return Y; }

    void SetX(std::uint8_t x) { X = x; }
    void SetY(std::uint8_t y) { Y = y; }

private:
    std::uint8_t X;
    std::uint8_t Y;
};

#endif //SDL_TETRIS_POSITION_H

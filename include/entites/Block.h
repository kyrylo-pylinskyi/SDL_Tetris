//
// Created by pylinskyi.k on 25.01.2024.
//


#ifndef SDL_TETRIS_BLOCK_H
#define SDL_TETRIS_BLOCK_H

#include "SDL.h"
#include <vector>
#include <functional>
#include "CoreTypes.h"
#include "components/RigidBody.h"

class Block {
public:
    Block(BlockType type);

    void Initialize();
    void Fall();
    void Rotate();
    void Steer(Side side);
    void Respawn(std::uint8_t gridWidth);

    SDL_Color GetColor() const { return _color; };
    Position GetPosition() const { return _rigidBody->GetPosition(); }

    std::uint8_t GetMatrixHeight() const { return _matrix.size(); }
    std::uint8_t GetMatrixWidth() const { return _matrix.size() > 0 ? _matrix[0].size() : 0; }
    bool IsTileActive(std::uint8_t i, std::uint8_t j) const { return _matrix[i][j]; }
    std::vector<Position> GetActiveCellPositions() const;
    BlockType GetBlockType() const { return _type; }

private:
    static constexpr std::uint8_t MAX_BLOCK_TYPES = 7;

    std::function<bool(std::uint8_t i, std::uint8_t j)> _defineMatrixCallback;
    void SetDefineMatrixCallback(const std::function<bool(std::uint8_t i, std::uint8_t j)>& callback) { _defineMatrixCallback = callback; }

    BlockType _type;
    SDL_Color _color;

    RigidBody* _rigidBody;
    std::vector<std::vector<bool>> _matrix;

    void ClearMatrix();
};

#endif //SDL_TETRIS_BLOCK_H

//
// Created by pylinskyi.k on 25.01.2024.
//
#include <functional>
#include "entites/Block.h"
#include "Game.h"
#include "systems/InputSystem.h"

Block::Block(BlockType type) : _type(type) {
    Initialize();
}

void Block::Initialize() {
    ClearMatrix();

    std::uint8_t height = 0;
    std::uint8_t width = 0;

    switch (_type) {
        case BlockType::I:
            height = 4;
            width = 1;
            _color = SDL_Color(0, 255, 255, 255);
            SetDefineMatrixCallback([](int i, int j){ return true; });
            break;

        case BlockType::O:
            height = width = 2;
            _color = SDL_Color(255, 255, 0, 255);
            SetDefineMatrixCallback([](int i, int j){ return true; });
            break;

        case BlockType::J:
            height = 2;
            width = 3;
            _color = SDL_Color(128, 0, 128, 255);
            SetDefineMatrixCallback([](int i, int j){ return (i == 0 && j == 2) || i == 1 ; });
            break;

        case BlockType::L:
            height = 2;
            width = 3;
            _color = SDL_Color(0, 255, 0, 255);
            SetDefineMatrixCallback([](int i, int j){ return (i == 0 && j == 0) || i == 1 ; });
            break;

        case BlockType::S:
            height = 2;
            width = 3;
            _color = SDL_Color(255, 0, 0, 255);
            SetDefineMatrixCallback([](int i, int j){ return (i == 0 && j < 2) || (i == 1 && j > 0); });
            break;

        case BlockType::Z:
            height = 2;
            width = 3;
            _color = SDL_Color(0, 0, 255, 255);
            SetDefineMatrixCallback([](int i, int j){return (i == 0 && j > 0) || (i == 1 && j < 2); });
            break;

        case BlockType::T:
            height = 2;
            width = 3;
            _color = SDL_Color(255, 127, 0, 255);
            SetDefineMatrixCallback([](int i, int j){return (i == 0 && j == 1) || (i == 1); });
            break;
    }

    for (std::uint8_t i = 0; i < height; i++){
        std::vector<bool> row;
        for (std::uint8_t j = 0; j < width; j++){
            row.push_back(_defineMatrixCallback(i, j));
        }
        _matrix.push_back(row);
    }

    _rigidBody = new RigidBody(0, 0, width, height);
}

void Block::Respawn(std::uint8_t gridWidth) {
    _type = static_cast<BlockType>((static_cast<uint8_t>(_type) + 1) % MAX_BLOCK_TYPES);
    _rigidBody->ResetPosition(gridWidth);
    Initialize();
}

void Block::Fall() {
    _rigidBody->Fall();
}

void Block::Steer(Side side) {
    switch(side){
        case LEFT:
            _rigidBody->SteerLeft();
            break;
        case RIGHT:
            _rigidBody->SteerRight();
            break;
    }
}

void Block::Rotate() {
    std::uint8_t height = _matrix.size();
    std::uint8_t width = _matrix.size() > 0 ? _matrix[0].size() : 0;

    std::vector<std::vector<bool>> newMatrix(width, std::vector<bool>(height, false));

    for (std::uint8_t i = 0; i < height; ++i) {
        for (std::uint8_t j = 0; j < width; ++j) {
            newMatrix[j][height - 1 - i] = _matrix[i][j];
        }
    }

    _matrix = newMatrix;
}

void Block::ClearMatrix() {
    if (_matrix.size() > 0){
        for (auto row: _matrix) row.clear();
        _matrix.clear();
    }
}

std::vector<Position> Block::GetActiveCellPositions() const {
    std::vector<Position> blocks;

    for (std::uint8_t i = 0; i < GetMatrixHeight(); ++i){
        for (std::uint8_t j = 0; j < GetMatrixHeight(); ++j){
            blocks.push_back(Position(i, j));
        }
    }

    return blocks;
}

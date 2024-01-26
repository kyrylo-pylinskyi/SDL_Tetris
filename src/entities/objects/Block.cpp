//
// Created by pylinskyi.k on 25.01.2024.
//
#include <functional>
#include "entites/objects/Block.h"
#include "Game.h"
#include "systems/input/InputSystem.h"

Block::Block(int x, int y, BlockType type) : Object(x, y), _type(type) {
    Initialize();
}

void Block::Render() {
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), _color.r, _color.g, _color.b, _color.a);

    for (std::uint8_t i = 0; i < _height; i++){
        for (std::uint8_t j = 0; j < _width; j++){
            if (_matrix[i][j]){
                SDL_Rect rect = {_position->GetX() + j * (_tileSize + _tilePadding), _position->GetY() + i * (_tileSize + _tilePadding), _tileSize, _tileSize};

                SDL_RenderFillRect(Game::GetInstance().GetRenderer(), &rect);
            }
        }
    }
}

void Block::Update() {

    Uint32 currentTime = SDL_GetTicks();

    std::vector<BlockType> types = { I, O, S, Z, L, J, T };

    if (InputSystem::GetInstance().IsKeyDown(SDL_SCANCODE_SPACE) && (currentTime - _lastPressTime) >= COOLDOWN_DURATION){
        std::uint8_t currentType = static_cast<uint8_t>(_type);
        std::uint8_t newType = (currentType + 1) % types.size();

        _type = static_cast<BlockType>(newType);
        Initialize();

        _lastPressTime = currentTime;

        SDL_Log("Block Type: %i", static_cast<uint8_t>(_type));
    }
}

void Block::Initialize() {
    if (_matrix.size() > 0){
        for (auto row: _matrix) row.clear();
        _matrix.clear();
    }

    switch (_type) {
        case BlockType::I:
            _height = 4;
            _width = 1;
            _color = SDL_Color(0, 255, 255, 255);
            SetDefineMatrixCallback([](int i, int j){ return true; });
            break;

        case BlockType::O:
            _height = 2;
            _width = 2;
            _color = SDL_Color(255, 255, 0, 255);
            SetDefineMatrixCallback([](int i, int j){ return true; });
            break;

        case BlockType::J:
            _height = 2;
            _width = 3;
            _color = SDL_Color(128, 0, 128, 255);
            SetDefineMatrixCallback([](int i, int j){ return (i == 0 && j == 2) || i == 1 ; });
            break;

        case BlockType::L:
            _height = 2;
            _width = 3;
            _color = SDL_Color(0, 255, 0, 255);
            SetDefineMatrixCallback([](int i, int j){ return (i == 0 && j == 0) || i == 1 ; });
            break;

        case BlockType::S:
            _height = 2;
            _width = 3;
            _color = SDL_Color(255, 0, 0, 255);
            SetDefineMatrixCallback([](int i, int j){ return (i == 0 && j < 2) || (i == 1 && j > 0); });
            break;

        case BlockType::Z:
            _height = 2;
            _width = 3;
            _color = SDL_Color(0, 0, 255, 255);
            SetDefineMatrixCallback([](int i, int j){return (i == 0 && j > 0) || (i == 1 && j < 2); });
            break;

        case BlockType::T:
            _height = 2;
            _width = 3;
            _color = SDL_Color(255, 127, 0, 255);
            SetDefineMatrixCallback([](int i, int j){return (i == 0 && j == 1) || (i == 1); });
            break;
    }

    for (std::uint8_t i = 0; i < _height; i++){
        std::vector<bool> row;
        for (std::uint8_t j = 0; j < _width; j++){
            row.push_back(_defineMatrixCallback(i, j));
        }
        _matrix.push_back(row);
    }
}
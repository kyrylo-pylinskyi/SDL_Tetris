//
// Created by pylinskyi.k on 27.01.2024.
//
#include <iostream>
#include <memory>
#include <random>
#include "entites/Level.h"
#include "Game.h"
#include "systems/InputSystem.h"

Level::Level(std::uint8_t height, std::uint8_t width, std::uint8_t blockSize, std::uint8_t blockPadding) :
        _blockSize(blockSize), _fallingBlock(nullptr), _lastSteerTime(0), _blockPadding(blockPadding), _lastFallTime(0) {
    InitializeGrid(height, width);
}

Level::~Level() {
    _fallingBlock.reset();
}

void Level::InitializeGrid(std::uint8_t height, std::uint8_t width) {
    _grid.clear();

    for (std::uint8_t i = 0; i < height; ++i) {
        _grid.push_back(std::vector<Tile>(width, Tile(SDL_Color(), false)));
    }

    _fallingBlock = std::make_unique<Block>(BlockType::Z);
}

void Level::Render() {
    SDL_Rect levelBox = {0, 0, static_cast<int>(_blockSize * GetGridWidth()), static_cast<int>(_blockSize * GetGridHeight())};
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 255, 255, 255);

    SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &levelBox);

    for (std::uint8_t i = 0; i < GetGridHeight(); i++) {
        for (std::uint8_t j = 0; j < GetGridWidth(); j++) {
            SDL_Rect rect = {static_cast<int>(j * _blockSize), static_cast<int>(i * _blockSize), static_cast<int>(_blockSize - _blockPadding), static_cast<int>(_blockSize - _blockPadding)};
            if (_grid[i][j].Active) {
                auto color = _grid[i][j].Color;
                SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(Game::GetInstance().GetRenderer(), &rect);
            }
        }
    }
}

void Level::Update() {
    std::uint8_t gridHeight = _grid.size();
    std::uint8_t gridWidth = gridHeight > 0 ? _grid[0].size() : 0;

    if (_fallingBlock->GetPosition().GetX() >= 0 &&
        _fallingBlock->GetPosition().GetX() + _fallingBlock->GetMatrixWidth() <= gridWidth) {
        AddBlockToGrid();
    }


    Uint32 currentTime = SDL_GetTicks();

    if (InputSystem::GetInstance().IsKeyDown(SDL_SCANCODE_S) || InputSystem::GetInstance().IsKeyDown(SDL_SCANCODE_DOWN)){
        _currentFallCooldown = FALL_SPEED_UP_COOLDOWN;
    }
    else{
        _currentFallCooldown = FALL_NORMAL_COOLDOWN;
    }

    if ((currentTime - _lastSteerTime) >= STEER_COOLDOWN) {
        int steerDirection = InputSystem::GetInstance().GetSteer();
        if ((steerDirection > 0 && CanSteerRight()) || (steerDirection < 0 && CanSteerLeft())) {
            RemoveBlockFromGrid();
            _fallingBlock->Steer(steerDirection > 0 ? RIGHT : LEFT);
            _lastSteerTime = currentTime;
        }
    }

    if ((currentTime - _lastFallTime) >= _currentFallCooldown && CanFall()){
        RemoveBlockFromGrid();
        _fallingBlock->Fall();

        _lastFallTime = currentTime;
    }
    if (!CanFall()){
        AddBlockToGrid();
        _fallingBlock->Respawn(GetGridWidth());
    }
}

void Level::AddBlockToGrid() {
    auto blockPos = _fallingBlock->GetPosition();

    for (std::uint8_t i = 0; i < _fallingBlock->GetMatrixHeight(); ++i) {
        for (std::uint8_t j = 0; j < _fallingBlock->GetMatrixWidth(); ++j) {
            if (blockPos.GetX() + j < GetGridWidth() && blockPos.GetY() + i < GetGridHeight()) {
                bool isBlockActiveTile = _fallingBlock->IsTileActive(i, j);
                bool isGridActiveTile = _grid[blockPos.GetY() + i][blockPos.GetX() + j].Active;

                if (isBlockActiveTile && !isGridActiveTile) {
                    _grid[blockPos.GetY() + i][blockPos.GetX() + j].Active = true;
                    _grid[blockPos.GetY() + i][blockPos.GetX() + j].Color = _fallingBlock->GetColor();
                }
            }
        }
    }
}

void Level::RemoveBlockFromGrid() {
    const auto blockPos = _fallingBlock->GetPosition();

    for (std::uint8_t i = 0; i < _fallingBlock->GetMatrixHeight(); ++i) {
        for (std::uint8_t j = 0; j < _fallingBlock->GetMatrixWidth(); ++j) {
            if (blockPos.GetX() + j < GetGridWidth() && blockPos.GetY() + i < GetGridHeight()) {
                bool isBlockActiveTile = _fallingBlock->IsTileActive(i, j);
                bool isGridActiveTile = _grid[blockPos.GetY() + i][blockPos.GetX() + j].Active;

                if (isBlockActiveTile && isGridActiveTile) {
                    _grid[blockPos.GetY() + i][blockPos.GetX() + j].Active = false;
                }
            }
        }
    }
}

bool Level::CanFall() {
    Position bottomPositions[_fallingBlock->GetMatrixWidth()];

    for (std::uint8_t i = 0; i < _fallingBlock->GetMatrixHeight(); ++i) {
        for (std::uint8_t j = 0; j < _fallingBlock->GetMatrixWidth(); ++j) {
            if(_fallingBlock->IsTileActive(i, j)){
                bottomPositions[j].SetX(_fallingBlock->GetPosition().GetX() + j);
                bottomPositions[j].SetY(_fallingBlock->GetPosition().GetY() + i + 1);
            }
        }
    }

    for (const auto position : bottomPositions){
        if (position.GetY() >= GetGridHeight() || _grid[position.GetY()][position.GetX()].Active) {
            return false;
        }
    }

    return true;
}

bool Level::CanSteerRight() {
    Position sidePositions[_fallingBlock->GetMatrixHeight()];

    for (int i = 0; i < _fallingBlock->GetMatrixHeight(); ++i) {
        for (int j = 0; j < _fallingBlock->GetMatrixWidth(); ++j) {
            if (_fallingBlock->IsTileActive(i, j)) {
                sidePositions[i].SetX(_fallingBlock->GetPosition().GetX() + j + 1);
                sidePositions[i].SetY(_fallingBlock->GetPosition().GetY() + i);
            }
        }
    }

    for (const auto position : sidePositions) {
        if (position.GetX() >= GetGridWidth() || _grid[position.GetY()][position.GetX()].Active) {
            return false;
        }
    }

    return true;
}

bool Level::CanSteerLeft() {
    Position sidePositions[_fallingBlock->GetMatrixHeight()];

    for (int i = 0; i < _fallingBlock->GetMatrixHeight(); ++i) {
        for (int j = 0; j < _fallingBlock->GetMatrixWidth(); ++j) {
            if (_fallingBlock->IsTileActive(i, j)) {
                sidePositions[i].SetX(_fallingBlock->GetPosition().GetX() + _fallingBlock->GetMatrixWidth() - j - 2);
                sidePositions[i].SetY(_fallingBlock->GetPosition().GetY() + i);
            }
        }
    }

    for (const auto position : sidePositions) {
        if (_fallingBlock->GetPosition().GetX() == 0 || _grid[position.GetY()][position.GetX()].Active) {
            return false;
        }
    }

    return true;
}

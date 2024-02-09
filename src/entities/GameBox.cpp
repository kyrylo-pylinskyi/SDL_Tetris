//
// Created by pylinskyi.k on 27.01.2024.
//
#include <iostream>
#include <memory>
#include <random>
#include "entites/GameBox.h"
#include "Game.h"
#include "systems/InputSystem.h"

GameBox::GameBox(std::uint8_t height, std::uint8_t width, int paddingLeft, int paddingUp, std::uint8_t blockSize, std::uint8_t blockPadding) :
        _paddingX(paddingLeft), _paddingY(paddingUp), _blockSize(blockSize), _blockPadding(blockPadding),
        _fallingBlock(nullptr), _lastSteerTime(0), _lastRotateTime(0), _lastFallTime(0) {
    InitializeGrid(height, width);
}

GameBox::~GameBox() {
    _fallingBlock.reset();
}

void GameBox::InitializeGrid(std::uint8_t height, std::uint8_t width) {
    _grid.assign(height, std::vector<Tile>(width, Tile(SDL_Color(), false)));
    _fallingBlock = std::make_unique<Block>(BlockType::Z);
}

void GameBox::Render() {
    SDL_Rect levelBox = {_paddingX, _paddingY, static_cast<int>(_blockSize * GetGridWidth()), static_cast<int>(_blockSize * GetGridHeight())};
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 255, 255, 255);

    SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &levelBox);

    for (std::uint8_t i = 0; i < GetGridHeight(); i++) {
        for (std::uint8_t j = 0; j < GetGridWidth(); j++) {
            SDL_Rect rect = {_paddingX + static_cast<int>(j * _blockSize), _paddingY + static_cast<int>(i * _blockSize), static_cast<int>(_blockSize - _blockPadding), static_cast<int>(_blockSize - _blockPadding)};

            if (_grid[i][j].Active) {
                auto color = _grid[i][j].Color;
                SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(Game::GetInstance().GetRenderer(), &rect);
            }
        }
    }
}

void GameBox::Update() {
    std::uint8_t gridHeight = _grid.size();
    std::uint8_t gridWidth = gridHeight > 0 ? _grid[0].size() : 0;

    if (_fallingBlock->GetPosition().GetX() >= 0 &&
        _fallingBlock->GetPosition().GetX() + _fallingBlock->GetMatrixWidth() <= gridWidth) {
        AddBlockToGrid();
    }


    _currentTime = SDL_GetTicks();

    HandleSpeedUp();
    HandleFall();
    HandleSteer();
    HandleRotate();
    HandleSpawn();
    HandleLineClear();
}

void GameBox::HandleSpeedUp() {
    _currentFallCooldown = (InputSystem::GetInstance().IsKeyDown(SDL_SCANCODE_S) || InputSystem::GetInstance().IsKeyDown(SDL_SCANCODE_DOWN)) ? FALL_SPEED_UP_COOLDOWN : FALL_NORMAL_COOLDOWN;
}

void GameBox::HandleSteer() {
    int steerDirection = InputSystem::GetInstance().GetSteer();
    if (_currentTime - _lastSteerTime < ACTION_COOLDOWN) return;

    if ((steerDirection > 0 && CanSteerRight()) || (steerDirection < 0 && CanSteerLeft())) {
        RemoveBlockFromGrid();
        steerDirection > 0 ? _fallingBlock->SteerRight() : _fallingBlock->SteerLeft();
        _lastSteerTime = _currentTime;
    }
}

void GameBox::HandleFall() {
    if ((_currentTime - _lastFallTime) >= _currentFallCooldown && CanFall()) {
        RemoveBlockFromGrid();
        _fallingBlock->Fall();
        _lastFallTime = _currentTime;
    }
}

void GameBox::HandleRotate() {
    if (InputSystem::GetInstance().IsKeyDown(SDL_SCANCODE_SPACE) && (_currentTime - _lastRotateTime) >= ACTION_COOLDOWN && CanRotate()) {
        RemoveBlockFromGrid();
        _fallingBlock->Rotate();
        _lastRotateTime = _currentTime;
    }
}

void GameBox::HandleSpawn() {
    if (!CanFall()) {
        AddBlockToGrid();
        _fallingBlock->Respawn(GetGridWidth());
    }
}


void GameBox::HandleLineClear() {
    for (int i = GetGridHeight() - 1; i >= 0; --i) {
        if (CanClearLine(i)) {
            _grid.erase(_grid.begin() + i);
            _grid.insert(_grid.begin(), std::vector<Tile>(GetGridWidth(), Tile(SDL_Color(), false)));
            _score += 100;
        }
    }

    SDL_Log("SCORE %i", _score);
}

void GameBox::AddBlockToGrid() {
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

void GameBox::RemoveBlockFromGrid() {
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

bool GameBox::CanFall() const {
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

bool GameBox::CanSteerRight() const {
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

bool GameBox::CanSteerLeft() const {
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
bool GameBox::CanRotate() const {
    std::vector<std::vector<bool>> rotated(_fallingBlock->GetMatrixWidth(), std::vector<bool>(_fallingBlock->GetMatrixHeight(), false));

    for (int i = 0; i < _fallingBlock->GetMatrixHeight(); ++i) {
        for (int j = 0; j < _fallingBlock->GetMatrixWidth(); ++j) {
            rotated[j][_fallingBlock->GetMatrixHeight() - 1 - i] = _fallingBlock->IsTileActive(i, j);
        }
    }

    for (std::uint8_t i = 0; i < rotated.size(); ++i){
        for (std::uint8_t j = 0; j < rotated.size(); ++j){
            int y = i + _fallingBlock->GetPosition().GetY() - 1;
            int x = j + _fallingBlock->GetPosition().GetX() - 1;

            bool outOfBounds = x < 0 || x > GetGridWidth() || y < 0 || y > GetGridHeight();
            if (outOfBounds) return false;

            bool blockTileActive = rotated[i][j];
            bool gridTileActive = _grid[y][x].Active;
            if(blockTileActive && gridTileActive) return false;
        }
    }

    return true;
}

bool GameBox::CanClearLine(std::uint8_t rowIndex) const {
    for (int j = 0; j < GetGridWidth(); ++j) {
        if (!_grid[rowIndex][j].Active) {
            return false;
        }
    }
    return true;
}
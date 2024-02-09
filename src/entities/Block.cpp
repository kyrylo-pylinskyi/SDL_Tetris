#include "entites/Block.h"

Block::Block(BlockType type) : _type(type) {
    Initialize();
}

void Block::Initialize() {
    ClearMatrix();

    switch (_type) {
        case BlockType::I:
            InitializeMatrix(4, 1, [](int i, int j) { return true; }, SDL_Color(0, 255, 255, 255));
            break;
        case BlockType::O:
            InitializeMatrix(2, 2, [](int i, int j) { return true; }, SDL_Color(255, 255, 0, 255));
            break;
        case BlockType::J:
            InitializeMatrix(2, 3, [](int i, int j) { return (i == 0 && j == 2) || i == 1; }, SDL_Color(128, 0, 128, 255));
            break;
        case BlockType::L:
            InitializeMatrix(2, 3, [](int i, int j) { return (i == 0 && j == 0) || i == 1; }, SDL_Color(0, 255, 0, 255));
            break;
        case BlockType::S:
            InitializeMatrix(2, 3, [](int i, int j) { return (i == 0 && j < 2) || (i == 1 && j > 0); }, SDL_Color(255, 0, 0, 255));
            break;
        case BlockType::Z:
            InitializeMatrix(2, 3, [](int i, int j) { return (i == 0 && j > 0) || (i == 1 && j < 2); }, SDL_Color(0, 0, 255, 255));
            break;
        case BlockType::T:
            InitializeMatrix(2, 3, [](int i, int j) { return (i == 0 && j == 1) || i == 1; }, SDL_Color(255, 127, 0, 255));
            break;
    }

    _rigidBody = std::make_unique<RigidBody>(rand() % 6 + 2, 0, GetMatrixWidth(), GetMatrixHeight());
}

void Block::InitializeMatrix(int height, int width, std::function<bool(int, int)> defineMatrixCallback, const SDL_Color& color) {
    _color = color;
    _matrix.resize(height, std::vector<bool>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            _matrix[i][j] = defineMatrixCallback(i, j);
        }
    }
}

void Block::Respawn(std::uint8_t gridWidth) {
    _type = static_cast<BlockType>((static_cast<uint8_t>(_type) + rand()) % MAX_BLOCK_TYPES);
    _rigidBody->ResetPosition(gridWidth);
    Initialize();
}

void Block::Fall() {
    _rigidBody->Fall();
}

void Block::SteerRight() {
    _rigidBody->SteerRight();
}

void Block::SteerLeft() {
    _rigidBody->SteerLeft();
}

void Block::Rotate() {
    std::vector<std::vector<bool>> rotated(GetMatrixWidth(), std::vector<bool>(GetMatrixHeight(), false));

    for (int i = 0; i < GetMatrixHeight(); ++i) {
        for (int j = 0; j < GetMatrixWidth(); ++j) {
            rotated[j][GetMatrixHeight() - 1 - i] = _matrix[i][j];
        }
    }

    _matrix = rotated;
}

void Block::ClearMatrix() {
    _matrix.clear();
}

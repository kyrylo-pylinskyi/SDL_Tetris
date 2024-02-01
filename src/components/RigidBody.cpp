//
// Created by pylinskyi.k on 26.01.2024.
//

#include "components/RigidBody.h"

RigidBody::RigidBody(std::uint8_t x, std::uint8_t y, std::uint8_t width, std::uint8_t height) : _width(width), _height(height) {
    _position = new Position(x, y);
}

void RigidBody::Fall() {
    _position->SetY(_position->GetY() + 1);
}

void RigidBody::SteerLeft() {
    _position->SetX(_position->GetX() - 1);
}

void RigidBody::SteerRight() {
    _position->SetX(_position->GetX() + 1);
}

void RigidBody::ResetPosition(std::uint8_t gridWidth) {
    std::uint8_t limitX = gridWidth - _width - 1;
    _position->SetX(3);
    _position->SetY(-4);
}

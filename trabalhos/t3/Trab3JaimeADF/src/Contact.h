#pragma once

#include <memory>

struct Collider;
struct GameObject;

struct Contact
{
    float Duration;

    std::shared_ptr<::Collider> Collider;

    std::shared_ptr<::GameObject> OtherObject;
    std::shared_ptr<::Collider> OtherCollider;
};
#pragma once

#include "osiris/reflect/markers.hpp"

struct Vec3 {
    float x;
    float y;
    float z;
};

CLASS()
struct DemoComponent {
    PROPERTY()
    Vec3 position;

    PROPERTY()
    float mass;

    PROPERTY()
    int id;
};

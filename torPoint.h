#pragma once
#include "types.h"
struct torPoint {
    torPoint* next = nullptr;
    torPoint* intersect = nullptr;
    FPoint p;
};
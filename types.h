#include <math.h>
#pragma once

struct FPoint {
    FPoint() {
        x = 0.0;
        y = 0.0;
    }
    FPoint(const FPoint& f) {
        x = f.x;
        y = f.y;
    }
    FPoint(float x, float y) {
        this->x = x;
        this->y = y;
    }
    FPoint operator+(FPoint const& obj)
    {
        FPoint res;
        res.x = x + obj.x;
        res.y = y + obj.y;
        return res;
    }

    FPoint operator-(FPoint const& obj)
    {
        FPoint res;
        res.x = x - obj.x;
        res.y = y - obj.y;
        return res;
    }

    float operator*(FPoint const& obj) {
        return x * obj.x + y * obj.y;
    }

    FPoint operator/(float const& obj)
    {
        FPoint res;
        res.x = x / obj;
        res.y = y / obj;
        return res;
    }
    FPoint operator*(float const& obj)
    {
        FPoint res;
        res.x = x * obj;
        res.y = y * obj;
        return res;
    }

    bool operator==(const FPoint& other) const {
        return (this->x == other.x) && (this->y == other.y);
    }
    float x;
    float y;
};
#pragma once

#include <cmath>

class Vector2 {
public:
    float x; // x坐标
    float y; // y坐标

    // 构造函数
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x_, float y_) : x(x_), y(y_) {}

    // 向量加法
    Vector2 operator+ (const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    // 向量减法
    Vector2 operator- (const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    // 向量乘法（点积）
    float operator* (const Vector2& v) const {
        return x * v.x + y * v.y;
    }

    // 向量数乘
    Vector2 operator* (const float& f) const {
        return Vector2(x * f, y * f);
    }

    // 向量除法
    Vector2 operator/ (const float& f) const {
        return Vector2(x / f, y / f);
    }

    // 向量长度
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // 向量长度平方
    float length_squared() const {
        return x * x + y * y;
    }

    // 向量规范化
    void normalize() {
        float len = length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
        }
    }

    // 向量旋转
    Vector2 rotate(float angle) const {
        float radians = angle * 3.14159265358979323846f / 180.0f;
        float c = std::cos(radians);
        float s = std::sin(radians);
        return Vector2(x * c - y * s, x * s + y * c);
    }

    // 判断向量是否为零向量
    bool is_zero() const {
        return (x == 0.0f && y == 0.0f);
    }

    Vector2 operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};
#pragma once
#include <iostream>

struct Vec2 {
	float x = 0.f; float y = 0.f;

    Vec2() = default;
    Vec2(float px, float py) : x(px), y(py) {}

    Vec2 operator+(const Vec2& o) const { return Vec2(x + o.x, y + o.y); }
    Vec2 operator-(const Vec2& o) const { return Vec2(x - o.x, y - o.y); }
    Vec2 operator*(float k) const { return Vec2(x * k, y * k); }
    bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Vec2& o) const { return !(*this == o); }
};

struct Rect {
    Vec2 pos; Vec2 size;
};

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;

    Color() = default;
    Color(char pr, char pg, char pb, char pa = 255)
        : r(pr), g(pg), b(pb), a(pa) {
    }

    Color hex(int rgb, int alpha = 255) {
        return Color((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF, alpha);
    }

    Color withAlpha(int alpha) const { return Color(r, g, b, alpha); }

    Color mixColors(const Color& a, const Color& b, int t) {
        if (t < 0) t = 0;
        else if (t > 255) t = 255;
        auto lerp = [t](int x, int y) {
            return (x + (y - x) * t)/255;
        };
        return Color(lerp(a.r, b.r), lerp(a.g, b.g), lerp(a.b, b.b), lerp(a.a, b.a));
    }
};

enum MouseButton {
    Left, Right, Middle
};

enum Key {
    Unknown, Escape, Enter, Tab, Backspace, Space,
    Left, Up, Right, Down,
    Digit1, Digit2, Digit3, Digit4, Digit5, LetterA
};

enum EventType {
    None, Closed, Resized,
    MouseMoved, MousePressed, MouseReleased, MouseWheel,
    TextEntered, KeyPressed
};

struct Event {
    EventType type = EventType::None;
    Vec2 mouse;
    MouseButton button = MouseButton::Left;
    int wheelDelta = 0;
    Key key = Key::Unknown;
    char unicode = 0;
    Vec2 windowSize;
};

enum FontId {
    Display, Body
};

struct Date {
    int day = 1;
    int month = 1;
    int year = 2026;

    Date() = default;
    Date(int d, int m, int y) : day(d), month(m), year(y) {}


};

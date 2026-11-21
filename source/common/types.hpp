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

    Rect() = default;
    Rect(Vec2 p, Vec2 s) : pos(p), size(s) {}
    Rect(float px, float py, float w, float h) : pos(px, py), size(w, h) {}

    float right() { return pos.x + size.x; }
    float bottom() { return pos.y + size.y; }
    Vec2 center() { return Vec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f); }

    bool contains(Vec2 p) {
        return p.x >= pos.x && p.x <= right() && p.y >= pos.y && p.y <= bottom();
    }

    Rect shrunk(float d) { return Rect(pos.x + d, pos.y + d, size.x - 2 * d, size.y - 2 * d); }
    Rect moved(Vec2 d) { return Rect(pos + d, size); }
};

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;

    Color() = default;

    Color(int pr, int pg, int pb, int pa = 255) : r(pr), g(pg), b(pb), a(pa) {}

    static Color hex(int rgb, int alpha = 255) {
        return Color((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF, alpha);
    }

    Color withAlpha(int alpha) const { return Color(r, g, b, alpha); }

    static Color mixColors(const Color& a, const Color& b, int t) {
        if (t < 0) t = 0;
        else if (t > 255) t = 255;
        auto lerp = [t](int x, int y) {
            return (x + (y - x) * t) / 255;
            };
        return Color(lerp(a.r, b.r), lerp(a.g, b.g), lerp(a.b, b.b), lerp(a.a, b.a));
    }
};

enum class MouseButton {
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

    bool operator<(const Date& o) const {
        if (year != o.year)   return year < o.year;
        if (month != o.month) return month < o.month;
        return day < o.day;
    }

    bool operator>(const Date& o) const {
        return o < *this;
    }

    bool operator==(const Date& o) const {
        return year == o.year && month == o.month && day == o.day;
    }

    bool operator!=(const Date& o) const {
        return !(*this == o);
    }

    bool isLeap(int y) const { return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0); }

    int daysInMonth(int m, int y) const {
        int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (m < 1 || m > 12) return 0;
        return (m == 2 && isLeap(y)) ? 29 : days[m - 1];
    }

    bool isValid() const {
        return year > 1970 && year < 3000 && month >= 1 && month <= 12 && day >= 1 && day <= daysInMonth(month, year);
    }
};

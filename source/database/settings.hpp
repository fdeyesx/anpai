#pragma once

class Settings {
public:
    int workMin = 25;
    int shortBreakMin = 5;
    int longBreakMin = 15;
    float volume = 60.f;

    int workMinutes() const { return workMin; }
};

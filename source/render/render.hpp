#pragma once
#include <string>
#include "common/types.hpp"

class Render {
public:
    virtual ~Render() = default;

    virtual bool isOpen() const { return false; };
    virtual void close() {};
    virtual bool pollEvent(Event& event) { (void)event; return false; };
    virtual void beginFrame() {};
    virtual void endFrame() {};
    virtual void drawRect(Rect& r, Color c) {};
    virtual void drawRing(Vec2 centre, float radius, float thickness, float ratio, Color c) {};
    virtual void drawText(std::string text, Vec2 pos, int size, Color c, FontId font = FontId::Body) {};
    virtual int loadTexture(std::string path) const {return 0;}
};

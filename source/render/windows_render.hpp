#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "types.hpp"
#include "render.hpp"

class WindowsRender : public Render {
private:
    sf::RenderWindow window;
    sf::Font fonts;
public:
    WindowsRender(int width, int height, const std::string& title) {
        window = sf::RenderWindow(sf::VideoMode({width, height }), title);
    }
    ~WindowsRender(){
        if (window.isOpen()) window.close();
    }

    bool isOpen() const {
        return window.isOpen();
    };

    std::optional<sf::Event> pollEvent(Event& event) {
        return window.pollEvent();
    };

    virtual void beginFrame() {
        window.clear(sf::Color::Black);
    };

    virtual void endFrame() {
        window.display();
    };

    virtual void drawRect(Rect& r, Color c) {
        sf::RectangleShape rect(sf::Vector2f(r.right(),r.bottom()));
        rect.setPosition({r.pos.x, r.pos.y});
        rect.setFillColor(sf::Color(c.r, c.g, c.b, c.a));
        window.draw(rect);
    };

    virtual void drawRing(Vec2 centre, float radius, float thickness, float ratio, Color c) {
        sf::CircleShape circle(radius);
        circle.setPosition({ centre.x - radius, centre.y - radius });
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(thickness);
        circle.setOutlineColor(sf::Color(c.r, c.g, c.b, c.a));
        window.draw(circle);
    };

    virtual void drawText(std::string text, Vec2 pos, int size, Color c, FontId font = FontId::Body) {
        sf::Text sfText(fonts);
        sfText.setString(text);
        sfText.setCharacterSize(size);
        sfText.setFillColor(sf::Color(c.r, c.g, c.b, c.a));
        sfText.setPosition({ pos.x, pos.y });
        window.draw(sfText);
    };

    virtual int loadTexture(std::string path) const {

    };
};

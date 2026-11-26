#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "common/types.hpp"
#include "gui/widget.hpp"
#include "render.hpp"

class WindowsRender : public Render {
private:
    sf::RenderWindow window;
public:
    WindowsRender(int width, int height, const std::string& title) {
        window = sf::RenderWindow(sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}), title);
    }
    ~WindowsRender(){
        if (window.isOpen()) window.close();
    }

    bool isOpen() const {
        return window.isOpen();
    };

    bool pollEvent(Event& event) override {
        std::optional<sf::Event> sfEvent = window.pollEvent();
        if (!sfEvent) return false;

        if (sfEvent->is<sf::Event::Closed>()) {
            event.type = Closed;
            return true;
        }

        if (const auto* r = sfEvent->getIf<sf::Event::Resized>()) {
            event.type = Resized;
            event.windowSize = Vec2(static_cast<float>(r->size.x), static_cast<float>(r->size.y));
            return true;
        }

        if (const auto* m = sfEvent->getIf<sf::Event::MouseMoved>()) {
            event.type = MouseMoved;
            event.mouse = Vec2(static_cast<float>(m->position.x), static_cast<float>(m->position.y));
            return true;
        }

        if (const auto* m = sfEvent->getIf<sf::Event::MouseButtonPressed>()) {
            event.type = MousePressed;
            event.mouse = Vec2(static_cast<float>(m->position.x), static_cast<float>(m->position.y));
            event.button = (m->button == sf::Mouse::Button::Left)  ? MouseButton::Left
                         : (m->button == sf::Mouse::Button::Right) ? MouseButton::Right
                                                                   : MouseButton::Middle;
            return true;
        }

        if (const auto* m = sfEvent->getIf<sf::Event::MouseButtonReleased>()) {
            event.type = MouseReleased;
            event.mouse = Vec2(static_cast<float>(m->position.x), static_cast<float>(m->position.y));
            return true;
        }

        if (const auto* w = sfEvent->getIf<sf::Event::MouseWheelScrolled>()) {
            event.type = MouseWheel;
            event.wheelDelta = static_cast<int>(w->delta);
            event.mouse = Vec2(static_cast<float>(w->position.x), static_cast<float>(w->position.y));
            return true;
        }

        if (const auto* t = sfEvent->getIf<sf::Event::TextEntered>()) {
            event.type = TextEntered;
            event.unicode = (t->unicode < 128) ? static_cast<char>(t->unicode) : '?';
            return true;
        }

        if (const auto* k = sfEvent->getIf<sf::Event::KeyPressed>()) {
            event.type = KeyPressed;
            switch (k->code) {
                case sf::Keyboard::Key::Escape: event.key = Escape; break;
                case sf::Keyboard::Key::Enter: event.key = Enter; break;
                case sf::Keyboard::Key::Tab: event.key = Tab; break;
                case sf::Keyboard::Key::Backspace: event.key = Backspace; break;
                case sf::Keyboard::Key::Space: event.key = Space; break;
                case sf::Keyboard::Key::Left: event.key = Left; break;
                case sf::Keyboard::Key::Up: event.key = Up; break;
                case sf::Keyboard::Key::Right: event.key = Right; break;
                case sf::Keyboard::Key::Down: event.key = Down; break;
                case sf::Keyboard::Key::Num1: event.key = Digit1; break;
                case sf::Keyboard::Key::Num2: event.key = Digit2; break;
                case sf::Keyboard::Key::Num3: event.key = Digit3; break;
                case sf::Keyboard::Key::Num4: event.key = Digit4; break;
                case sf::Keyboard::Key::Num5: event.key = Digit5; break;
                case sf::Keyboard::Key::A: event.key = LetterA; break;
                default: event.key = Unknown; break;
            }
            return true;
        }

        event.type = None;
        return true;
    }

    virtual void beginFrame() {
        window.clear(sf::Color::Black);
    };

    virtual void endFrame() {
        window.display();
    };

    virtual void drawRect(Rect& r, Color c) {
        sf::RectangleShape rect(sf::Vector2f(r.size.x, r.size.y));
        rect.setPosition(sf::Vector2f(r.pos.x, r.pos.y));
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
        sf::Text sfText(Fonts::getFont(font), sf::String::fromUtf8(text.begin(), text.end()),size);
        sfText.setString(text);
        sfText.setCharacterSize(size);
        sfText.setFillColor(sf::Color(c.r, c.g, c.b, c.a));
        sfText.setPosition({ pos.x, pos.y });
        window.draw(sfText);
    };

    virtual int loadTexture(std::string path) const {return 0;}

    void close() override {
        window.close();
        return;
    }

    sf::RenderWindow& target() { return window; }
};

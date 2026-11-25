#pragma once
#include "common/vector.hpp"
#include "widget.hpp"
#include "render/render.hpp"

class Scene {
protected:
    Array<widget*> widgets;
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void handleEvent(const Event& e) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget& target) = 0; 

    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual std::string name() const = 0;
};

class MainScene : public Scene {
public:
    void onEnter() override { std::cout << "Entered Main Menu\n"; }
    void onExit() override { std::cout << "Left Main Menu\n"; }

    void handleEvent(const Event& e) override {
        for (int i = 0; i < widgets.getsize(); ++i) {
            widgets[i]->handleEvent(e);
        }
    }

    void update(float dt) override { (void)dt; }

    void draw(sf::RenderTarget& target) override {
        target.clear(sf::Color(30, 30, 40)); // Фон

        for (int i = 0; i < widgets.getsize(); ++i) {
            widgets[i]->draw(target);
        }
    }

    std::string name() const override { return "MainScene"; }

    void addWidget(widget* w) { widgets.push_back(w); }
    int widgetCount() const { return widgets.getsize(); }
};

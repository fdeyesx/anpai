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

class SceneManager {
private:
	Array<Scene*> scenes;
	int active = -1;
public:
	SceneManager() = default;
	~SceneManager() {}

	void addScene(Scene* scene) {
		scenes.push_back(scene);
		if (active < 0) { active = 0; scenes[0]->onEnter(); }
	}

	int count() const { return scenes.getsize(); }
	int activeIndex() const { return active; }

	Scene* currentScene() {
		if (active < 0 || active >= scenes.getsize()) return nullptr;
		return scenes[active];
	}

	void switchTo(int index) {
		if (index < 0 || index >= scenes.getsize()) return;
		if (index == active) return;
		if (Scene* previous = currentScene()) previous->onExit();
		active = index;
		scenes[active]->onEnter();
	}

	void handleEvent(const Event& e) { 
        if (Scene* s = currentScene()) s->handleEvent(e); 
    }

	void update(float dt) { 
        if (Scene* s = currentScene()) s->update(dt); 
    }

	void draw(sf::RenderTarget& t) { 
        if (Scene* s = currentScene()) s->draw(t); 
    }
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
        target.clear(sf::Color(30, 30, 40));

        for (int i = 0; i < widgets.getsize(); ++i) {
            widgets[i]->draw(target);
        }
    }

    std::string name() const override { return "MainScene"; }

    void addWidget(widget* w) { widgets.push_back(w); }
    int widgetCount() const { return widgets.getsize(); }
};

class TasksScene : public Scene {
public:
	void onEnter() override { std::cout << "Entered TasksScene\n"; }
	void onExit() override { std::cout << "Left TasksScene\n"; }

	void handleEvent(const Event& e) override {
		for (int i = 0; i < widgets.getsize(); ++i) widgets[i]->handleEvent(e);
	}

	void update(float dt) override { (void)dt; }

	void draw(sf::RenderTarget& target) override {
		target.clear(sf::Color(28, 32, 40));
		for (int i = 0; i < widgets.getsize(); ++i) 
            widgets[i]->draw(target);
	}

	std::string name() const override { return "TasksScene"; }

    void addWidget(widget* w) { widgets.push_back(w); }
    int widgetCount() const { return widgets.getsize(); }
};

class SessionScene : public Scene{

};

class StatsScene : public Scene{

};

class SettingsScene : public Scene{

};

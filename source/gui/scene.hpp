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

    void addWidget(widget* w) {
        widgets.push_back(w);
    }

    int widgetCount() const {
        return widgets.getsize();
    }
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
        target.clear(sf::Color(30, 30, 40)); // Фон

        for (int i = 0; i < widgets.getsize(); ++i) {
            widgets[i]->draw(target);
        }
    }

    std::string name() const override { return "MainScene"; }
};

class TasksScene : public Scene {
private:
    Database& database;
	TaskService taskService;
	CategoryService categoryService;
	TaskTable table;
	Panel panel;
	int currentCategoryId = 0;
public:
    TasksScene(Database& db): database(db), taskService(db), categoryService(db){
        panel.setTitle("задачи");
		panel.setAccent(Color(221, 237, 228));
		panel.setPosition(Vec2(40.f, 240.f));
		panel.setSize(Vec2(600.f, 400.f));

		table.setService(&taskService);
		table.setPosition(Vec2(60.f, 290.f));
		table.setSize(Vec2(560.f, 330.f));

		addWidget(&panel);
		addWidget(&table);
    }

	void onEnter() override { 
        std::cout << "Entered TasksScene\n"; 
        int flag = 0;
        if (currentCategoryId == 0) {
			for (Listiterator<Category> it = database.categories.begin(); it != database.categories.end_it() && flag == 0; ++it) {
				currentCategoryId = (*it).id;
				flag++;
			}
		}
		table.setCategory(categoryService.find(currentCategoryId));
    }

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
};

#pragma once
#include <string>
#include <chrono>
#include "render/windows_render.hpp"
#include "gui/scene.hpp"
#include "crud/tasks_service.hpp"

class App {
private:
	WindowsRender render;
	Fonts fonts;
	Database db;
	SceneManager manager;

	MainScene mainScene;
	TasksScene tasksScene;
	//SessionScene sessionScene;
	//StatsScene statsScene;
	//SettingsScene settingsScene;

	NavigationBar rail;
	BackgroundBanner banner;
	Panel quickPanel;

	void buildMainScene(){
        banner.setPosition(Vec2(0.f, 0.f));
	    banner.setSize(Vec2(1000.f, 220.f));
	    banner.setTitle("get things done");
	    banner.loadImage("assets/backgrounds/bg1.png");

	    quickPanel.setTitle("быстрый доступ");
	    quickPanel.setAccent(Color(247, 215, 222));
	    quickPanel.setPosition(Vec2(40.f, 260.f));
	    quickPanel.setSize(Vec2(320.f, 340.f));

	    mainScene.addWidget(&banner);
	    mainScene.addWidget(&quickPanel);
    }

	void loadFonts(){
        try{
            fonts.load("assets/fonts/display.ttf", "assets/fonts/body.ttf");
		    return;
        } catch (const errors& e){
            std::cout << "assets/fonts is empty\n";
        }
        fonts.load("C:/Windows/Fonts/georgia.ttf", "C:/Windows/Fonts/segoeui.ttf");
    }

public:
	App() : render(1000, 700, "Get Things Done"), rail(5) {}
	~App() {}
		int run(){
        loadFonts();
        try{
            db.load();
        } catch (const errors& e){
            std::cout << "load: " << e.msg() << "\n";
        }

        buildMainScene();

        manager.addScene(&mainScene);
        manager.addScene(&tasksScene);

        rail.setPosition(Vec2(960.f, 260.f));
        rail.setSelected(0);

        int lastRail = 0;
        int flag = 0;
        int frame = 0;
        auto previousTime = std::chrono::steady_clock::now();

        while (render.isOpen()) {
            Event e;
            while (render.pollEvent(e) && flag == 0) {
                if (e.type == Closed) { render.close(); flag = 1; }
                if (e.type == KeyPressed && e.key == Escape) { render.close(); flag = 1; }

                if (flag == 0) {
                    manager.handleEvent(e);
                    rail.handleEvent(e);
                }
            }
            if (!render.isOpen()) break;

            const auto now = std::chrono::steady_clock::now();
            const float dt = std::chrono::duration<float>(now - previousTime).count();
            previousTime = now;

            manager.update(dt);

            if (rail.getSelected() != lastRail) {
                lastRail = rail.getSelected();
                manager.switchTo(lastRail);
            }

            render.beginFrame();
            manager.draw(render.target());
            rail.draw(render.target());
            render.endFrame();

            ++frame;
        }

        try{
            db.saveAll();
        } catch (const errors& e){
            std::cout << "save: " << e.msg() << "\n";
        }

        std::cout << "frames: " << frame << "\n";
        return 0;
	}
};

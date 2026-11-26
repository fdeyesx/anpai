#pragma once
#include <string>
#include "widget.hpp"
#include "crud/tasks_service.hpp"

class TaskTable : public widget {
private:
    struct Row {
		Task* task = nullptr;
		int depth = 0;
	};

    TaskService* service;
    Category* category;
    Array<Row> rows;

    float rowHeight = 28.f;
    int hovered = -1;

    static sf::Color priorityColor(Priority p) {
		if (p == high)   return sf::Color(217, 138, 146);
		if (p == medium) return sf::Color(227, 197, 138);
		return sf::Color(157, 191, 168);
	}

    template <typename T>
    void collect(T*node, int depth){
        if (node == nullptr) return;

        if (node->value.id() != 0) {
			Row row;
			row.task = &(node->value);
			row.depth = depth;
			rows.push_back(row);
		}

        for (int i = 0; i < node->children.getsize(); ++i) {
			collect(node->children[i], depth + 1);
		}
    }

    static std::string deadlineText(Task& task) {
		Date d = task.getDeadline();
		if (d.year == 2026 && d.month == 1 && d.day == 1) return "";

		std::string result;
		if (d.day < 10) result += "0";
		result += std::to_string(d.day) + ".";
		if (d.month < 10) result += "0";
		result += std::to_string(d.month);
		return result;
	}

    void drawCheckBox(sf::RenderTarget& target, Vec2 at, bool done, bool isHovered){
        sf::RectangleShape box(sf::Vector2f(15.f, 15.f));
		box.setPosition(sf::Vector2f(at.x, at.y));
		box.setFillColor(done ? sf::Color(247, 215, 222) : sf::Color(255, 255, 255));
		box.setOutlineThickness(1.f);
		box.setOutlineColor(isHovered ? sf::Color(217, 138, 146) : sf::Color(200, 192, 184));
		target.draw(box);

        if (done) {
			sf::RectangleShape tick(sf::Vector2f(8.f, 2.f));
			tick.setPosition(sf::Vector2f(at.x + 3.5f, at.y + 7.f));
			tick.setFillColor(sf::Color(74, 59, 51));
			target.draw(tick);
		}
    }

    int rowAt(Vec2 point) {
		if (category == nullptr) return -1;
		if (point.x < pos.x || point.x > pos.x + size.x) return -1;

		const float first = pos.y + 44.f;
		if (point.y < first) return -1;

		int index = static_cast<int>((point.y - first) / rowHeight);
		if (index < 0 || index >= rows.getsize()) return -1;
		return index;
	}
public:
    TaskTable() = default;
	~TaskTable() {}

	void setService(TaskService* s) { service = s; }
    void setCategory(Category* c) {
		category = c;
		rebuild();
	}

    void rebuild(){
        rows.clear();
        hovered = -1;
        if (category == nullptr) return;

        auto* root = category->tasks.getRoot();
        if(root->value.id()==0){
            for (int i = 0; i < root->children.getsize(); ++i) {
				collect(root->children[i], 0);
			}
        } else collect(root,0);
    }

    void draw(sf::RenderTarget& target) override {
        if(!visible) return;

        sf::Font& body = Fonts::getFont(Body);
		sf::Font& display = Fonts::getFont(Display);

        if(category == nullptr){
            drawText(target, body, "without category", Vec2(pos.x, pos.y), 15, sf::Color(138, 126, 116));
            return;
        }

        drawText(target, display, category->name, Vec2(pos.x, pos.y), 20, sf::Color(74, 59, 51));

        std::string progress = std::to_string(category->doneTasks()) + " / " + std::to_string(category->totalTasks());
		drawText(target, body, progress, Vec2(pos.x + size.x - 70.f, pos.y + 4.f), 14, sf::Color(138, 126, 116));

        const float headY = pos.y + 26.f;
        drawText(target, body, "tasks", Vec2(pos.x + 22.f, headY), 12, sf::Color(182, 172, 163));
		drawText(target, body, "deadline", Vec2(pos.x + size.x - 60.f, headY), 12, sf::Color(182, 172, 163));
        
        float y = pos.y + 44.f;
        for (int i = 0; i < rows.getsize(); i++){
            Row& row = rows[i];

            const float x = pos.x + row.depth * 22.f;

            drawCheckBox(target, Vec2(x, y + 5.f), row.task->isDone(), i == hovered);

            sf::CircleShape dot(3.5f);
			dot.setPosition(sf::Vector2f(x + 38.f, y + 9.f));
			dot.setFillColor(row.task->isDone() ? sf::Color(182, 172, 163) : priorityColor(row.task->getPriority()));
			target.draw(dot);
            
            sf::Color textColor = row.task->isDone() ? sf::Color(182, 172, 163) : sf::Color(74, 59, 51);
			drawText(target, body, row.task->getTitle(), Vec2(x + 50.f, y), 15, textColor);
            
            if (row.task->isDone()) {
				sf::RectangleShape line(sf::Vector2f(90.f, 1.f));
				line.setPosition(sf::Vector2f(x + 50.f, y + 10.f));
				line.setFillColor(sf::Color(182, 172, 163));
				target.draw(line);
			}
            
            drawText(target, body, deadlineText(*row.task), Vec2(pos.x + size.x - 60.f, y), 14, row.task->isOverdue() ? sf::Color(217, 138, 146) : sf::Color(138, 126, 116));

			y += rowHeight;
        }

        if(rows.getsize() == 0)
            drawText(target, body, "no tasks yet", Vec2(pos.x + 20.f, pos.y + 50.f), 15, sf::Color(182, 172, 163));
    }

    bool handleEvent(const Event& e) override {
        if (e.type == MouseMoved) {
			hovered = contains(e.mouse) ? rowAt(e.mouse) : -1;
		}
		else if (e.type == MousePressed && e.button == MouseButton::Left) {
			const int index = rowAt(e.mouse);
			if (index >= 0 && service != nullptr) {
				service->toggleDone(rows[index].task->id());
				rebuild();
				return true;
			}
		}
		return false;
    }
};

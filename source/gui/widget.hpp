#include <iostream>
#include <string> 
#include <SFML/Graphics.hpp>
#include "common/vector.hpp"
#include "common/types.hpp"

class widget {
protected:
	Vec2 pos{ 0.f, 0.f };
	Vec2 size{ 0.f, 0.f };
	bool visible = true;
public:
	widget() = default;
	virtual ~widget() {};

	virtual void draw(sf::RenderTarget& target) = 0;
	virtual bool handleEvent(const Event& e) {
		return false;
	};
	bool contains(Vec2 v) const {
		return v.x >= pos.x && v.x <= pos.x + size.x && v.y >= pos.y && v.y <= pos.y + size.y;
	}

	void setPosition(Vec2 p) { pos = p; }
	void setSize(Vec2 s) { size = s; }
	Vec2 getPosition() const { return pos; }
	Vec2 getSize() const { return size; }
};

class Button : public widget {
private:
	std::string label;
	sf::RectangleShape shape;
public:
	Button() = default;
	~Button() {};

	void draw(sf::RenderTarget& target) override {
		if (!visible) return;
		shape.setPosition(sf::Vector2f(pos.x, pos.y));
		shape.setSize(sf::Vector2f(size.x, size.y));
		shape.setFillColor(sf::Color(244, 240, 234));
		target.draw(shape);

		//drawtext
	}
};

class PillButton : public widget {
private:
	std::string label;
	Color fill = Color(244, 240, 234);
	Color textColor = Color(74, 59, 51);
	bool hovered = false;
	bool clicked = false;
public:
	PillButton() = default;
	PillButton(std::string label, Vec2 position, Vec2 box): label(label) {
		pos = position;
		size = box;
	}

	~PillButton() {};

	void setLabel(std::string& l) { label = l; }
	std::string getLabel() const { return label; }

	bool takeClick() {
		bool result = clicked;
		clicked = false;
		return result;
	}

	bool handleEvent(const Event& e) override {
		if (e.type == MouseMoved) {
			hovered = contains(e.mouse);
		}
		else if (e.type == MousePressed && e.button == MouseButton::Left) {
			if (contains(e.mouse)) clicked = true;
		}
		return false;
	}

	void draw(sf::RenderTarget& target) override {
		if (!visible) return;

		//drawroundrect
		//drawtext
	};
};

class Label : public widget {
private:
	std::string text;
	int fontSize = 15;
	Color color = Color(74, 59, 51);
	FontId fontId = Body;
public:
	Label() = default;
	Label(std::string t, int fontSize, Color color, FontId fontId = Body)
		: text(t), fontSize(fontSize), color(color), fontId(fontId) {
	}

	void setText(const std::string& t) { text = t; }
	void setColor(Color c) { color = c; }
	std::string getText() const { return text; }

	void draw(sf::RenderTarget& target) override {
		if (!visible) return;
		//drawtext;
	}
};

struct CheckBox {
	bool checked = false;
};

class Panel : public widget {
private:
	std::string title;
	Color accent = Color(247, 215, 222);
	Color paper = Color(255, 255, 255);

public:
	Panel() = default;
	Panel(std::string title, Color accent) : title(title), accent(accent) {}

	void setTitle(const std::string& t) { title = t; }
	void setAccent(Color a) { accent = a; }
	void setPaper(Color p) { paper = p; }
	std::string getTitle() const { return title; }

	//draw
};

class BackgroundBanner : public widget {
private:
	sf::Texture texture;
	bool hasImage = false;
	std::string title;
	Color top = Color(155, 196, 214);
	Color bottom = Color(214, 233, 240);
public:
	BackgroundBanner() = default;

	bool loadImage(const std::string& path) {
		hasImage = texture.loadFromFile(path);
		return hasImage;
	}

	void setTitle(const std::string& t) { title = t; }
	void setGradient(Color t, Color b) { top = t; bottom = b; }
	bool hasImage() const {
		return hasImage;
	}

	//draw
};

class NavigationBar : public widget {
private:
	int count = 5;
	int selected = 0;

	float step() const { return 14.f; }
public:
	NavigationBar() = default;
	NavigationBar(int c) : count(c) {}

	int getSelected() const { return selected; }
	void setSelected(int index) {
		if (index >= 0 && index < count) selected = index;
	}

	bool handleEvent(const Event& e) override {
		if (e.type == MousePressed && e.button == MouseButton::Left) {
			if (contains(e.mouse)) {
				int flag = 0;
				for (int i = 0; i < count && flag == 0; ++i) {
					float itemY = pos.y + i * step();
					if (e.mouse.y >= itemY && e.mouse.y < itemY + step()) {
						selected = i;
						flag = 1;
					}
				}
			}
		}
		return false;
	}

	//draw
};

class Slider : public widget {
private:
	double value_ = 0.0, min_ = 0.0, max_ = 100.0;
public:
	Slider() = default;
	~Slider() {};
	
	void draw(sf::RenderTarget& target) override {};
	double value() const { return value_; }

};

struct TextField {
	std::string text_;
};

class Chart : public widget{
protected:
	Array<double> values;
	Array<std::string> labels;
public:
	Chart() = default;
	~Chart() {};

	void setData(const Array<double>& v) { values = v; }
};

struct ProgressRing {
	double ratio = 0.0, radius = 0.0, thickness = 0.0;
};

struct ProgressBar {
	double ratio;
};

struct TabBar {
	Array<std::string> titles;
	int active = 0;
};

#include <iostream>
#include <string> 
#include <SFML/Graphics.hpp>
#include "common/vector.hpp"
#include "common/types.hpp"

#include <iostream>
#include <string> 
#include <SFML/Graphics.hpp>
#include "vector.hpp"
#include "types.hpp"

#define FSIZE 2
#define POINT_COUNT 20

class Fonts {
private:
	sf::Font fonts_[FSIZE];
	bool loaded_[2] = {false, false};
	static Fonts* instance_;
public:
	Fonts() { instance_ = this; }

	void load(std::string& displayPath, std::string& bodyPath) {
		loaded_[0] = fonts_[0].openFromFile(displayPath);
		loaded_[1] = fonts_[1].openFromFile(bodyPath);
		if (!loaded_[0] || !loaded_[1]) throw FontLoadFailed();
	}
	
	static sf::Font& getFont(FontId id) {
		int index = (id == Display) ? 0 : 1;
		return instance_->fonts_[instance_->loaded_[index] ? index : 0];
	}
};

inline Fonts* Fonts::instance_ = nullptr;

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


void drawRoundRect(sf::RenderTarget& target, sf::FloatRect r, float radius, sf::Color color) {
	if (radius <= 0.5f) {
		sf::RectangleShape shape(r.size);;
		shape.setPosition(sf::Vector2f(r.position));
		shape.setFillColor(color);
		target.draw(shape);
		return;
	}

	sf::RectangleShape horizontal(sf::Vector2f(r.size.x, r.size.y - 2 * radius));
	horizontal.setPosition(sf::Vector2f(r.position.x, r.position.y + radius));;
	horizontal.setFillColor(color);
	target.draw(horizontal);

	sf::RectangleShape vertical(sf::Vector2f(r.size.x - 2 * radius, r.size.y));
	vertical.setPosition(sf::Vector2f(r.position.x + radius, r.position.y));
	vertical.setFillColor(color);
	target.draw(vertical);

	sf::CircleShape corner(radius, POINT_COUNT);
	corner.setFillColor(color);
	float offsets[4][2] = {
		{0.f,0.f}, //left top angle
		{ r.size.x - 2 * radius, 0.f }, //right top angle
		{ 0.f, r.size.y - 2 * radius }, // left bottom angle
		{ r.size.x - 2 * radius, r.size.y - 2 * radius } //right bottom angle
	};
	for (int i = 0; i < 4; i++) {
		corner.setPosition(sf::Vector2f(r.position.x + offsets[i][0] + radius, r.position.y + offsets[i][1] + radius));
		target.draw(corner);
	}
}

void drawShadow(sf::RenderTarget& target, sf::FloatRect r, float radius) {
	sf::RectangleShape shadow(sf::Vector2f(r.size.x, r.size.y));
	shadow.setPosition(sf::Vector2f(r.position.x + 4.f, r.position.y + 4.f));
	shadow.setFillColor(sf::Color(0, 0, 0, 50)); // Полупрозрачный черный
	target.draw(shadow);
}

void drawText(sf::RenderTarget& target, sf::Font& font, std::string text, Vec2 pos, int size, sf::Color color) {
	sf::Text t(font, text, size);
	t.setPosition(pos);
	t.setFillColor(color);
	target.draw(t);
}

float textWidth(const sf::Font& font, const std::string& text, unsigned size) {
	sf::Text t(font, sf::String::fromUtf8(text.begin(), text.end()), size);
	return t.getLocalBounds().size.x;
}

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

		Color currentFill = fill;
		if (hovered) fill = Color(fill.r - 8, fill.g - 8, fill.b - 8, fill.a);

		drawRoundRect(target, sf::FloatRect(pos, size), size.y * 0.5f, currentFill);
		sf::Font& font = Fonts::getFont(Body);
		float w = textWidth(font, label, 15);
		drawText(target, font, label, { pos.x + (size.x - w) * 0.5f, pos.y + size.y * 0.5f - 10.f }, 15, textColor);
	}
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
		
		drawText(target, Fonts::getFont(fontId), text, pos, fontSize, color);
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

	void draw(sf::RenderTarget& target) override {
		if (!visible) return;

		const sf::FloatRect box(pos, size);
		drawShadow(target, box, 14.f);
		drawRoundRect(target, box, 14.f, paper);

		drawRoundRect(target, sf::FloatRect({ pos.x + 12.f, pos.y + 12.f }, { size.x - 24.f, 30.f }), 8.f, accent);

		drawText(target, Fonts::getFont(Display), title, { pos.x + 24.f, pos.y + 17.f }, 17, Color(74, 59, 51, 255));
	}
};

class BackgroundBanner : public widget {
private:
	sf::Texture texture;
	bool hasImage_ = false;
	std::string title;
	Color top = Color(155, 196, 214);
	Color bottom = Color(214, 233, 240);
public:
	BackgroundBanner() = default;

	bool loadImage(const std::string& path) {
		hasImage_ = texture.loadFromFile(path);
		return hasImage_;
	}

	void setTitle(const std::string& t) { title = t; }
	void setGradient(Color t, Color b) { top = t; bottom = b; }
	bool hasImage() const {
		return hasImage_;
	}

	void draw(sf::RenderTarget& target) override {
		if (!visible) return;

		if (hasImage_) {
			const sf::Vector2u s = texture.getSize();
			if (s.x > 0 && s.y > 0) {
				const float scale = std::max(size.x / s.x, size.y / s.y);
				sf::Sprite sprite(texture);
				sprite.setScale(sf::Vector2f(scale, scale));
				sprite.setPosition(sf::Vector2f(pos.x - (s.x * scale - size.x) * 0.5f, pos.y - (s.y * scale - size.y) * 0.5f));
				target.draw(sprite);
			}
		}
		else {
			sf::VertexArray gradient(sf::PrimitiveType::TriangleStrip);
			sf::Vertex v;

			v.color = top;
			v.position = sf::Vector2f(pos.x, pos.y);
			gradient.append(v);

			v.position = sf::Vector2f(pos.x + size.x, pos.y);
			gradient.append(v);

			v.color = bottom;
			v.position = sf::Vector2f(pos.x, pos.y + size.y);
			gradient.append(v);

			v.position = sf::Vector2f(pos.x + size.x, pos.y + size.y);
			gradient.append(v);

			target.draw(gradient);
		}

		if (!title.empty()) {
			drawText(target, Fonts::getFont(Display), title, { pos.x + 110.f, pos.y + size.y + 34.f }, 32, Color(74, 59, 51));
		}
	}
};

class NavigationBar : public widget {
private:
	int count = 5;
	int selected = 0;

	float getStep() const { return 14.f; }
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
					float itemY = pos.y + i * getStep();
					if (e.mouse.y >= itemY && e.mouse.y < itemY + getStep()) {
						selected = i;
						flag = 1;
					}
				}
			}
		}
		return false;
	}

	void draw(sf::RenderTarget& target) override {
		if (!visible) return;

		const sf::Color active(74, 59, 51);
		const sf::Color inactive(182, 172, 163);

		for (int i = 0; i < count; ++i) {
			const bool isActive = (i == selected);
			sf::RectangleShape dash(sf::Vector2f(isActive ? 16.f : 9.f, 3.f));
			dash.setPosition(sf::Vector2f(pos.x, pos.y + i * getStep()));
			dash.setFillColor(isActive ? active : inactive);
			target.draw(dash);
		}
	}
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



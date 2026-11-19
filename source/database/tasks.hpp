#pragma once
#include <iostream>
#include "common/vector.hpp"
#include "common/list.hpp"
#include "common/tree.hpp"

enum Priority {
	high, medium, low
};

class Task {
protected:
	int id_;
	std::string title;
	Priority priority;
	std::string deadline;
	bool done;

public:
	Task() {
		id_ = 0;
		title = "";
		priority = medium;
		done = false;
	};

	virtual ~Task() {};

	int id() const { return id_; };
	void toggleDone() { done = !done; };
	bool isDone() const { return done; }
	bool operator==(const Task& t2) const {
		return id_ == t2.id_;
	};
	std::string getTitle() const { return title; }
};

class TaskTree : public Tree<Task> {
public:
	TaskTree() = default;
	~TaskTree() {};

	void addRoot(Task& t) { this->addChild(t, t); }
	void addChild(Task& parent, Task& t) { this->addChild(parent, t); ; }

	bool hasTask(Task& t) {
		return this->contains(t);
	}

	bool removeTask(Task& t) {
		return this->contains(t);
	}
};

class Category {
public:
	int id;
	std::string name;
	TaskTree tasks;

	Category(int i, std::string n): id(i), name(n){}

	bool operator==(const Category& other) const {
		return id == other.id;
	}
};

class CategoryList : public List<Category> {
public:
	CategoryList() : List<Category>() {}
	~CategoryList() = default;
};


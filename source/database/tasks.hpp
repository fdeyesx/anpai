#pragma once
#include <iostream>
#include "common/vector.hpp"
#include "common/list.hpp"
#include "common/tree.hpp"
#include "common/types.hpp"
#include <ctime>
#include <sstream>

enum Priority {
	high, medium, low
};

class Task {
protected:
	int id_;
	std::string title;
	Priority priority;
	Date deadline;
	bool done;

public:
	Task() {
		id_ = 0;
		title = "";
		priority = medium;
		done = false;
		deadline = Date();
	};

	Task(int id, std::string t, Priority p, Date d) {
		id_ = id;
		title = t;
		priority = p;
		done = false;
		deadline = d;
	};

	virtual ~Task() {};

	static int& idCounter() { static int counter = 0; return counter; }
	static int newId() { return ++idCounter(); }
	static void reserveId(int id) { if (id > idCounter()) idCounter() = id; }

	int id() const { return id_; };
	void toggleDone() { done = !done; };
	bool isDone() const { return done; }
	bool operator==(const Task& t2) const {
		return id_ == t2.id_;
	};
	std::string getTitle() const { return title; }

	int doneCount() const { return done ? 1 : 0; }

	void setDeadline(time_t d) {
		std::tm timeinfo;
		localtime_s(&timeinfo, &d);
		deadline = Date(timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
	}
	Date getDeadline() const { return deadline; }

	Priority getPriority() const { return priority; }

	bool isOverdue() {
		if (done) return false;

		time_t now = time(nullptr);
		std::tm timeinfo;
		localtime_s(&timeinfo, &now);

		Date today(timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
		return today > deadline;
	}

	std::string getDeadlineString() const {
		if (deadline.year == 2026 && deadline.month == 1 && deadline.day == 1) {
			return "None\n";
		}

		std::ostringstream out;
		out << deadline.year << "-";
		if (deadline.month < 10) out << "0";
		out << deadline.month << "-";
		if (deadline.day < 10) out << "0";
		out << deadline.day << "\n";

		return out.str();
	}

	void setPriority(Priority p) {
		priority = p;
	}

	void setDeadline(Date d) {
		deadline = d;
	}

	friend class FileStorage;
};

class TaskTree : public Tree<Task> {
public:
	TaskTree() = default;
	~TaskTree() {};

	Task* addRoot(Task& t) {
		if (this->getRoot() == nullptr) {
			return &(this->addRootNode(t)->value);
		}
		return addChild(this->getRoot()->value, t);
	}

	Task* addChild(Task& parent, Task& t) {
		typename Tree<Task>::Node* node = Tree<Task>::addChild(parent, t);
		if (node == nullptr) return nullptr;
		return &(node->value);
	}

	Task* findById(int id) {
		return findByIdFrom(this->getRoot(), id);
	}

	Task* findTask(Task& t, Task& value) {
		Node* node = this->findNode(this->getRoot(), value);
		if (node) return &(node->value);
		return nullptr;
	}

	bool hasTask(Task& t) {
		return this->contains(t);
	}

	bool removeTask(Task& t) {
		return Tree<Task>::remove(t);
	}

	int totalCount() {
		return countNodes(this->getRoot());
	}

	double progress() {
		int total = totalCount();
		if (total == 0) return 0.0;
		int done = countDone(this->getRoot());
		return (double)done / total;
	}
private:
	Task* findByIdFrom(typename Tree<Task>::Node* node, int id) {
		if (!node) return nullptr;
		if (node->value.id() == id) return &(node->value);
		for (int i = 0; i < node->children.getsize(); ++i) {
			Task* found = findByIdFrom(node->children[i], id);
			if (found) return found;
		}
		return nullptr;
	}

	int countNodes(typename Tree<Task>::Node* node) {
		if (!node) return 0;
		int count = 1;
		for (int i = 0; i < node->children.getsize(); i++) {
			count += countNodes(node->children[i]);
		}
		return count;
	}

	int countDone(typename Tree<Task>::Node* node) {
		if (!node) return 0;
		int count = node->value.isDone() ? 1 : 0;
		for (int i = 0; i < node->children.getsize(); ++i) {
			count += countDone(node->children[i]);
		}
		return count;
	}
};

class Category {
public:
	int id;
	std::string name;
	TaskTree tasks;

	Category(int i, std::string n) : id(i), name(n) {}

	bool operator==(const Category& other) const {
		return id == other.id;
	}

	double progres() {
		return tasks.progress();
	}
	int totalTasks() {
		return tasks.totalCount();
	}
	int doneTasks() {
		return (int)(tasks.progress() * tasks.totalCount());
	}

};

class CategoryList : public List<Category> {
public:
	CategoryList() : List<Category>() {}
	~CategoryList() = default;

	void pushback(Category& c) {
		this->push_back(c);
	}

	bool remove(int id) {
		Category* c = find(id);
		if (c == nullptr) return false;
		return List<Category>::remove(*c);
	}

	void moveUp(int id) {
		Category* c = find(id);
		if (c == nullptr) return;
		List<Category>::moveUp(*c);
	}

	void moveDown(int id) {
		Category* c = find(id);
		if (c == nullptr) return;
		List<Category>::moveDown(*c);
	}

	Category* find(int id) {
		Category* result = nullptr;
		for (Listiterator<Category> it = begin(); it != end_it() && result == nullptr; ++it) {
			if ((*it).id == id) {
				result = &(*it);
			}
		}
		return result;
	}
};

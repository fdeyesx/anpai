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

	virtual ~Task() {};

	int id() const { return id_; };
	void toggleDone() { done = !done; };
	bool isDone() const { return done; }
	bool operator==(const Task& t2) const {
		return id_ == t2.id_;
	};
	std::string getTitle() const { return title; }

	int doneCount() const { return done ? 1 : 0; }

	void setDeadline(time_t d) {deadline = d;}
	Date getDeadline() const {return deadline;}

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
};

class TaskTree : public Tree<Task> {
public:
	TaskTree() = default;
	~TaskTree() {};

	void addRoot(Task& t) { this->addChild(t, t); }
	void addChild(Task& parent, Task& t) { this->addChild(parent, t); ; }

	Task* findTask(Task& t, Task& value) {
		Node* node = this->findNode(this->getRoot(), value);
		if (node) return &(node->value);
		return nullptr;
	}

	bool hasTask(Task& t) {
		return this->contains(t);
	}

	bool removeTask(Task& t) {
		return this->contains(t);
	}

	int totalCount(){
		return countNodes(this->getRoot());
	}

	double progress(){
		int total = totalCount();
		if (total == 0) return 0.0;
		int done = countDone(this->getRoot());
		return (double)done / total;
	}
private:
	int countNodes(typename Tree<Task>::Node* node){
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

	Category(int i, std::string n): id(i), name(n){}

	bool operator==(const Category& other) const {
		return id == other.id;
	}
	
	double progres(){ 
		return tasks.progress(); 
	}
	int totalTasks() {
		return tasks.totalCount();
	}
	int doneTasks(){
		return tasks.progress() * tasks.totalCount();
	}

};

class CategoryList : public List<Category> {
public:
	CategoryList() : List<Category>() {}
	~CategoryList() = default;

	void pushback(Category& c) {
		this->push_back(c);
	}

	bool remove(int id){
		Category *c = find(id);
		return List<Category>::remove(*c);
	}

	void moveUp(int id) {
		Category* c = find(id);
		List<Category>::moveUp(*c);
	}

	void moveDown(int id) {
		Category* c = find(id);
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

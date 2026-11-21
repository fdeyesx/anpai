#pragma once
#include "database/tasks.hpp"
#include "database/session.hpp"
#include "database/settings.hpp"

struct Database {
	CategoryList categories;
	SessionLog sessions;
	Settings settings;
	void load();
	void saveAll();
};

class TaskService {
protected:
	Database& db;

	Category* findCategoryOfTask(int id) {
		for (Listiterator<Category> it = db.categories.begin(); it != db.categories.end_it(); ++it) {
			if ((*it).tasks.findById(id) != nullptr) return &(*it);
		}
		return nullptr;
	}

public:
	TaskService(Database& db_) : db(db_) {};
	~TaskService() {};

	Task& addTask(int Catid, std::string title, Priority p, Date& d) {
		if (title.empty()) throw InvalidTask();
		if (!d.isValid()) throw InvalidDate();

		Category* cat = db.categories.find(Catid);
		if (cat == nullptr) throw TaskNotFound();

		Task task(Task::newId(), title, p, d);
		Task* created = cat->tasks.addRoot(task);
		if (created == nullptr) throw InvalidTask();

		return *created;
	}

	Task& addSubtask(int parentId, std::string title) {
		if (title.empty()) throw InvalidTask();

		Category* cat = findCategoryOfTask(parentId);
		if (cat == nullptr) throw TaskNotFound();

		Task* parent = cat->tasks.findById(parentId);
		if (parent == nullptr) throw TaskNotFound();

		Task subtask(Task::newId(), title, parent->getPriority(), parent->getDeadline());
		Task* created = cat->tasks.addChild(*parent, subtask);
		if (created == nullptr) throw InvalidTask();

		return *created;
	}

	Task* findTask(int id) {
		for (Listiterator<Category> it = db.categories.begin(); it != db.categories.end_it(); ++it) {
			Task* found = (*it).tasks.findById(id);
			if (found != nullptr) return found;
		}
		return nullptr;
	}

	bool toggleDone(int id) {
		Task* t = findTask(id);
		bool result = false;
		if (t) {
			t->toggleDone();
			result = true;
		}
		return result;
	}

	bool setPriority(int id, Priority p) {
		Task* t = findTask(id);
		bool result = false;
		if (t) {
			t->setPriority(p);
			result = true;
		}
		return result;
	}

	bool setDeadline(int id, Date d) {
		if (!d.isValid()) return false;

		Task* t = findTask(id);
		bool result = false;
		if (t) {
			t->setDeadline(d);
			result = true;
		}
		return result;
	}

	bool removeTask(int id) {
		Category* cat = findCategoryOfTask(id);
		if (cat == nullptr) return false;

		Task* t = cat->tasks.findById(id);
		if (t == nullptr) return false;

		return cat->tasks.removeTask(*t);
	}
};

class CategoryService {
protected:
	Database& db;

	int nextId() {
		int maxId = 0;
		for (Listiterator<Category> it = db.categories.begin(); it != db.categories.end_it(); ++it) {
			if ((*it).id > maxId) maxId = (*it).id;
		}
		return maxId + 1;
	}
public:
	CategoryService(Database& db_): db(db_){}
	~CategoryService() {}

	Category& addCategory(std::string name){
		if (name.empty()) throw InvalidArgument();
		Category cat(nextId(), name);
		db.categories.pushback(cat);

		return *db.categories.find(cat.id);
	}

	bool renameCategory(int id, std::string name){
		if (name.empty()) throw InvalidArgument();

		Category* cat = db.categories.find(id);
		if (cat == nullptr) return false;

		cat->name = name;
		return true;
	}

	bool removeCategory(int id) {
		return db.categories.remove(id);
	}

	void moveUp(int id){
		db.categories.moveUp(id);
	}

	void moveDown(int id) {
		db.categories.moveDown(id);
	}

	Category* find(int id) {
		return db.categories.find(id);
	}
	
	CategoryList& list(){ return db.categories; }
	int count() { return db.categories.getsize(); }
};

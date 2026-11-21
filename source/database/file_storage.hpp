#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "tasks.hpp"
#include "common/tree.hpp"
#include "session.hpp"

#define MAXT 20
#define MAXD 100

class FileStorage {
public:
	FileStorage(){}
	~FileStorage(){}

	bool isDummyRoot(Task& t) {
		return t.id() == 0 && t.getTitle().empty();
	}

	template <class NodeT>
	void saveTaskTree(std::ofstream& file, int catId, const std::string& catName, NodeT* node, int depth) {
		if (node == nullptr) return;

		Task& t = node->value;

		file << catId << "|" << catName << "|"
			<< t.id() << "|" << t.getTitle() << "|"
			<< static_cast<int>(t.getPriority()) << "|"
			<< t.getDeadline().day << "|"
			<< t.getDeadline().month << "|"
			<< t.getDeadline().year << "|"
			<< (t.isDone() ? 1 : 0) << "\n";

		for (int i = 0; i < node->children.getsize(); ++i) {
			saveTaskTree(file, catId, catName, node->children[i]);
		}
	}

	void saveTasks(CategoryList& categories, std::string& path) {
		std::ofstream file(path);
		if (!file.is_open()) throw FileOpenFailed();

		for (Listiterator<Category> it = categories.begin(); it != categories.end_it(); ++it) {
			Category& cat = *it;

			if (isDummyRoot(cat.tasks.getRoot()->value)) {
				for (int i = 0; i < cat.tasks.getRoot()->children.getsize(); ++i) {
					saveTaskTree(file, cat.id, cat.name, cat.tasks.getRoot()->children[i], 0);
				}
			}
			else {
				saveTaskTree(file, cat.id, cat.name, cat.tasks.getRoot(), 0);
			}
		}

		file.close();
		if (!file) throw FileWriteFailed();
	}

	void saveSession(std::string& path, SessionLog& log) {
		std::ofstream file(path);
		if (!file.is_open()) throw FileOpenFailed();

		for (int i = 0; i < log.sessions.getsize(); ++i) {
			Session s = log.getSession(i);

			file << s.task_id << "|"
				<< s.minutes << "|"
				<< static_cast<int>(s.type) << "|"
				<< s.date.day << "|"
				<< s.date.month << "|"
				<< s.date.year << "\n";
		}

		file.close();
		if (!file) throw FileWriteFailed();
	}

	void loadTasks(CategoryList& categories, std::string& path) {
		std::ifstream file(path);
		if (!file.is_open()) return;

		categories.clear();

		Task* parents[MAXD];
		for (int i = 0; i < MAXD; ++i) parents[i] = nullptr;

		std::string line;
		while (std::getline(file, line)) {
			if (line.length() > 0 && line[0] == '#') {
				std::string tokens[MAXT];
				int tokenIndex = 0;

				for (int i = 0; i < line.length(); ++i) {
					char c = line[i];
					if (c == '|') if (tokenIndex + 1 < MAXT) tokenIndex++;
					else tokens[tokenIndex] += c;
				}
				try {
					int catId = std::stoi(tokens[0]);
					std::string catName = tokens[1];
					int depth = std::stoi(tokens[2]);
					int taskId = std::stoi(tokens[3]);
					std::string title = tokens[4];
					Priority p = static_cast<Priority>(std::stoi(tokens[5]));
					Date d(std::stoi(tokens[6]), std::stoi(tokens[7]), std::stoi(tokens[8]));
					bool isDone = (std::stoi(tokens[9]) == 1);

					if (depth < 0 || depth >= MAXT) throw ParseError();

					Category* cat = categories.find(catId);
					if (!cat) {
						Category newCat(catId, catName);
						categories.pushback(newCat);
						cat = categories.find(catId);

						for (int i = 0; i < MAXD; ++i) parents[i] = nullptr;
					}

					Task newTask(taskId, title, p, d);
					if (isDone) newTask.toggleDone();

					Task::reserveId(taskId);

					if (depth == 0) {
						parents[0] = cat->tasks.addRoot(newTask);
					}
					else {
						if (parents[depth - 1] == nullptr) throw ParseError();
						parents[depth] = cat->tasks.addChild(*parents[depth - 1], newTask);
					}

				}
				catch (const errors&e) {
					throw ParseError();
				}

			}
		}
	}

	void loadSessions(std::string& path, SessionLog& log) {
		std::ifstream file(path);
		if (!file.is_open()) return;

		std::string line;
		while (std::getline(file, line)) {
			if (line.length() > 0) {
				std::string tokens[MAXT];
				int tokenIndex = 0;

				for (int i = 0; i < line.length(); ++i) {
					if (line[i] == '|') if (tokenIndex + 1 < MAXT) tokenIndex++;
					else tokens[tokenIndex] += line[i];
				}

				try {
					Session s;
					s.task_id = std::stoi(tokens[0]);
					s.minutes = std::stoi(tokens[1]);
					s.type = static_cast<SessionType>(std::stoi(tokens[2]));
					s.date = Date(std::stoi(tokens[3]), std::stoi(tokens[4]), std::stoi(tokens[5]));

					log.add(s);
				}
				catch (...) {
					throw ParseError();
				}
			}
		}
	}

	bool fileExists(std::string& path) const {
		std::ifstream file(path);
		return file.good();
	}
};

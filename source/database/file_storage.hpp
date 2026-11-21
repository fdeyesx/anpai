#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "tasks.hpp"
#include "common/tree.hpp"
#include "session.hpp"

class FileStorage {
public:
	FileStorage(){}
	~FileStorage(){}

	void saveTaskTree(std::ofstream& file, int catId, const std::string& catName, Tree<Task>::Node* node) {
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

		try {
			for (Listiterator<Category> it = categories.begin(); it != categories.end_it(); ++it) {
				Category& cat = *it;

				if (cat.tasks.getRoot() != nullptr) {
					saveTaskTree(file, cat.id, cat.name, cat.tasks.getRoot());
				}
			}
		}
		catch (const errors& e) {
			throw FileWriteFailed();
		}
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
	}

	void loadTasks(CategoryList& categories, std::string& path) {
		std::ifstream file(path);
		if (!file.is_open()) return;

		std::string line;
		while (std::getline(file, line)) {
			if (line.length() > 0) {
				std::string tokens[9];
				int tokenIndex = 0;

				for (int i = 0; i < line.length(); ++i) {
					char c = line[i];
					if (c == '|') tokenIndex++;
					else tokens[tokenIndex] += c;

				}
				try {
					int catId = std::stoi(tokens[0]);
					std::string catName = tokens[1];
					int taskId = std::stoi(tokens[2]);
					std::string title = tokens[3];
					Priority p = static_cast<Priority>(std::stoi(tokens[4]));
					Date d(std::stoi(tokens[5]), std::stoi(tokens[6]), std::stoi(tokens[7]));
					bool isDone = (std::stoi(tokens[8]) == 1);

					Category* cat = categories.find(catId);
					if (!cat) {
						Category newCat(catId, catName);
						categories.pushback(newCat);
						cat = categories.find(catId);
					}

					Task newTask(taskId, title, p, d);
					if (isDone) newTask.toggleDone();

					cat->tasks.addRoot(newTask);
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
				std::string tokens[6];
				int tokenIndex = 0;

				for (int i = 0; i < line.length(); ++i) {
					if (line[i] == '|') tokenIndex++;
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

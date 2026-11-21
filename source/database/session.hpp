#pragma once
#include <iostream>
#include <ctime>
#include "vector.hpp"

enum SessionType {
	Work, Short_break, Long_break
};

struct Session {
	int minutes;
	SessionType type;
	time_t date;
	int task_id;
};

class SessionLog : public Session {
protected:
	Array<Session> sessions;
public:
	SessionLog() = default;
	~SessionLog();
	
	void add(Session& s) {
		sessions.push_back(s);
	}
	
	Array<Session> filter() {}

	void save(){}
	void load(){}
};

#pragma once
#include <iostream>
#include <ctime>
#include "common/vector.hpp"
#include "common/types.hpp"

enum SessionType {
	Work, Short_break, Long_break
};

struct Session {
	int minutes;
	SessionType type;
	Date date;
	int task_id;
};

class SessionLog : public Session {
protected:
	Array<Session> sessions;
public:
	SessionLog() = default;
	~SessionLog() = default;
	
	void add(Session& s) {
		sessions.push_back(s);
	}
	
	//Array<Session> filter() {};

	void save() {};
	void load() {};

	int getCount() const { return sessions.getsize(); }
	Session getSession(int index) { return sessions[index]; }

	friend class FileStorage;
};

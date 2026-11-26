#include <iostream>
#include "source/app.hpp"
#include "gui/tasks_table.hpp"

int main() {
	try {
		App app;
		return app.run();
	}
	catch (const errors& e) {
		std::cerr << "Ошибка: " << e.msg() << "\n";
		return 1;
	}
}
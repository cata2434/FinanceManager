#pragma once

#include "UI.h"
#include "Repository.h"
#include "Action.h"
#include <sstream>
#include <stack>
#include <vector>

using std::getline;
using std::cin;
using std::stack;
using std::vector;

class Logic {
	friend class Action;
public :
	void add_user_to_repository();    // type 1
	void update_user_to_repository(); // type 2
	void remove_user_to_repository(); // type 3
	void display_all_users();         
	void write_to_file();             
	void display_user_by_id();        

	void add_to_undo(Action last_action);
	
	void undo();
	void undo_operation(Action last_action);
	void redo();
	void redo_operation(Action last_action);

	void start();

protected:
	Repository main_repository;
	stack<Action> undo_stack;
	stack<Action> redo_stack;

};




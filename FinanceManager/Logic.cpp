#include "Logic.h"
#include <fstream>
#include <string>

void Logic::add_user_to_repository() {
	unsigned int _id = main_repository.get_size() + 1;
	cout << "Your name is: " << endl;
	string _name;
	getline(cin, _name);
	unsigned int _account_balance = 0;
	cout << "Initial account balance: ";
	string balance;
	getline(cin, balance);
	try {
		for (int i = 0; i < balance.length(); i++) {
			if (balance[i] != '9' && balance[i] != '8' && balance[i] != '7' && balance[i] != '6' && balance[i] != '5' && balance[i] != '4' && balance[i] != '3' && balance[i] != '2' && balance[i] != '1' && balance[i] != '0')
			{
				throw 251;
			}
		}
	}
	catch (int) {
		cout << "You must enter a number bigger than 0.";
		return;
	}
	_account_balance = stoi(balance);


	User new_user(_id, _name, _account_balance);

	bool keep_adding_purchases = true;
	cout << "Add purchase or write STOP/stop to finish adding the user: \n";
	while (keep_adding_purchases) {

		string item;
		cout << "Item name: ";
		getline(cin, item);
		if (item == "STOP" || item == "stop")
			break;
		cout << "Item price: ";
		string price_str;
		getline(cin, price_str);
		unsigned int price = 0;
		if (price_str == "STOP" || price_str == "stop")
			break;
		else {
			price = stoi(price_str);
		}
		cout << "\n";
		new_user.add_purchase_to_map(item, price);
	}
	
	undo_stack.push(Action(1, new_user));
	main_repository.add_user_to_repository(new_user);
}

void Logic::update_user_to_repository(){
	cout << "User id: ";
	string id_string;
	getline(cin, id_string);
	try {
		for (int i = 0; i < id_string.length(); i++) {
			if (id_string[i] != '9' && id_string[i] != '8' && id_string[i] != '7' && id_string[i] != '6' && id_string[i] != '5' && id_string[i] != '4' && id_string[i] != '3' && id_string[i] != '2' && id_string[i] != '1' && id_string[i] != '0')
			{
				throw 252;
			}
		}
	}
	catch (int) {
		cout << "You must enter a number bigger than 0.";
		return;
	}
	unsigned int _id = stoi(id_string);

	for (User& user : main_repository.get_rep()) {
		if (user.get_id() == _id) {
			// purchase item for user at the given id;

			string item;
			cout << "Item name: ";
			getline(cin, item);
			cout << "Item price: ";
			string price_str;
			getline(cin, price_str);
			try {
				for (int i = 0; i < price_str.length(); i++) {
					if (price_str[i] != '9' && price_str[i] != '8' && price_str[i] != '7' && price_str[i] != '6' && price_str[i] != '5' && price_str[i] != '4' && price_str[i] != '3' && price_str[i] != '2' && price_str[i] != '1' && price_str[i] != '0')
					{
						throw 253;
					}
				}
			}
			catch (int) {
				cout << "You must enter a number bigger than 0.";
				return;
			}
			unsigned int price = 0;
			price = stoi(price_str);
			user.purchase_item(item, price);

			undo_stack.push(Action(2, user));
			main_repository.update_user_by_id(user);
		}
	}
}

void Logic::remove_user_to_repository(){
	cout << "User id: ";
	string id_string;
	getline(cin, id_string);
	try {
		for (int i = 0; i < id_string.length(); i++) {
			if (id_string[i] != '9' && id_string[i] != '8' && id_string[i] != '7' && id_string[i] != '6' && id_string[i] != '5' && id_string[i] != '4' && id_string[i] != '3' && id_string[i] != '2' && id_string[i] != '1' && id_string[i] != '0')
			{
				throw 252;
			}
		}
	}
	catch (int) {
		cout << "You must enter a number bigger than 0.";
		return;
	}
	unsigned int _id = stoi(id_string);

	undo_stack.push(Action(3, main_repository.get_user_by_id(_id)));
	main_repository.remove_user_by_id(_id);
}

void Logic::display_all_users(){
	for (User user : main_repository.get_rep()) {
		cout << user;
	}
}

void Logic::write_to_file() {
	main_repository.write_to_file();
}

void Logic::display_user_by_id(){
	cout << "User id: ";
	string id_string;
	getline(cin, id_string);
	try {
		for (int i = 0; i < id_string.length(); i++) {
			if (id_string[i] != '9' && id_string[i] != '8' && id_string[i] != '7' && id_string[i] != '6' && id_string[i] != '5' && id_string[i] != '4' && id_string[i] != '3' && id_string[i] != '2' && id_string[i] != '1' && id_string[i] != '0')
			{
				throw 252;
			}
		}
	}
	catch (int) {
		cout << "You must enter a number bigger than 0.";
		return;
	}
	unsigned int _id = stoi(id_string);
	cout << main_repository.get_user_by_id(_id);
}

void Logic::add_to_undo(Action last_action) {
	undo_stack.push(last_action);
}

void Logic::undo() {
	// pops last action from the stack into redo
	if (undo_stack.empty() == false) {
		Action last_action = undo_stack.top();
		undo_stack.pop();
		undo_operation(last_action);
		redo_stack.push(last_action);
	}
}

void Logic::undo_operation(Action last_action) {
	// apply undo directly
	// using switch to apply the proper command
	switch (last_action.type) {
	case 1:
		main_repository.remove_user_by_id(last_action.user_action.get_id());
		break;
	case 2:
		main_repository.update_user_by_id(last_action.get_user_action());
		break;
	case 3:
		main_repository.add_user_to_repository(last_action.get_user_action());
		break;
	default:
		printf("Operation not available.");
		break;
	}
}

void Logic::redo()
{
	if (redo_stack.empty() == false) {
		Action last_action = redo_stack.top();
		redo_stack.pop();
		redo_operation(last_action);
		undo_stack.push(last_action);
	}
}

void Logic::redo_operation(Action last_action) {
	switch (last_action.type) {
	case 1:
		main_repository.add_user_to_repository(last_action.get_user_action());
		break;
	case 2:
		main_repository.update_user_by_id(last_action.get_user_action());
		break;
	case 3:
		main_repository.remove_user_by_id(last_action.user_action.get_id());
		break;
	default:
		printf("Operation not available.");
		break;
	}
}


void Logic::start() {

	displayMenu();

	string command;

	// reading data from file
	main_repository.read_from_file();

	cout << "Your command: ";
	while (getline(cin, command)) {



		cout << "\n";
		
		if (command == "m" || command == "M") {
			displayMenu();
		}

		else if (command == "q" || command == "Q") {
			break;
		}

		else if (command == "a" || command == "A") {
			add_user_to_repository();
		}

		else if (command == "u" || command == "U") {
			update_user_to_repository();
		}

		else if (command == "r" || command == "R") {
			remove_user_to_repository();
		}

		else if (command == "d" || command == "D") {
			display_all_users();
		}

		else if (command == "i" || command == "I") {
			display_user_by_id();
		}


		else if (command == "b" || command == "B") {
			undo();
		}


		else if (command == "f" || command == "F") {
			redo();
		}

		else if (command == "s" || command == "S") {
			write_to_file();
		}
		else{
			try {
				throw 250;
			}
			catch (int) {
				cout << "\nPlease pick a proper command!";
			}
		}


		cout << "\n\n\nYour command: ";
	}

}


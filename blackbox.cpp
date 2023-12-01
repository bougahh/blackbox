#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>

//plansza
struct board {
	unsigned ui_size = 1;
	unsigned x = ui_size - 1, y = ui_size - 1;

	std::vector <char> char_board;
	std::vector <unsigned> atom_position_list = {};
	char player_input = 'w';
};

void random_atom_positions(board& board) {

	unsigned atom_amount = 3;
	bool error_present = false;

	switch (board.ui_size) {
	case 6:
		atom_amount = 3;
		break;
	case 9:
		atom_amount = 5;
		break;
	case 11:
		atom_amount = 8;
		break;
	}
	srand(time(NULL));

	do {
		error_present = false;
		//generuje miejsca na podstawie calej planszy od pierwszego miejsca na atomy do ostatniego (nie sa wykluczone brzegi)
		for (unsigned i = 0; i < atom_amount; i++)
			board.atom_position_list.push_back((rand() % ((board.ui_size * board.ui_size) - 4) + board.ui_size + 2));

		std::sort(board.atom_position_list.begin(), board.atom_position_list.end());
		//sprawdzamy czy miejsca sie powtarzaja i generujemy liczby ponownie, jesli tak
		for (unsigned i = 1; i < board.atom_position_list.size(); i++) {
			if (board.atom_position_list[i - 1] == board.atom_position_list[i] || board.atom_position_list[i - 1] == board.atom_position_list[i] + 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] - 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] + board.ui_size + 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] - (board.ui_size + 1)) {
				error_present = true;
				board.atom_position_list.resize(0);
				break;
			}
		}

		//sprawdzamy czy sa atomy na prawej lub lewej stronie planszy i generujemy ponownie, jesli tak
		for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
			for (unsigned j = 1; j < board.ui_size; j++) {
				if (board.atom_position_list[i] == j * (board.ui_size + 1) || board.atom_position_list[i] == ((j + 1) * (board.ui_size + 1)) - 1) {
					error_present = true;
					board.atom_position_list.resize(0);
					break;
				}
			}
		}

	} while (error_present);
}

unsigned ray_shoot(board& board/*, char start_direction*/) {
	unsigned x_init = board.x, y_init = board.y;
	unsigned distance = 0;
	bool collision = false;
	char collision_type = ' '; //set this to start_direction
	//while(!finish){}
	unsigned a = 0;
	//switch (collision_type) {
	//case 'u':
		while (!collision) {
			while (a != 3) {
				if (board.x + board.y * (board.ui_size + 1) - board.ui_size == board.atom_position_list[a]) {
					collision_type = 'r';
					collision = true;
					break;
				}
				else if (board.x + board.y * (board.ui_size + 1) - board.ui_size == board.atom_position_list[a] + 1) {
					collision_type = 'h';
					//finish true;
					collision = true;
					break;
				}
				else if (board.x + board.y * (board.ui_size + 1) - board.ui_size == board.atom_position_list[a] + 2) {
					collision_type = 'l';
					collision = true;
					break;
				}
				else
					a++;
			}
			if (!collision) {
				board.y--;
				distance++;
				a = 0;
			}
		}
	//}
	//if (y_init == board.ui_size) {

	//}
	std::cout << collision_type;
	board.y = y_init;
	return distance;
}

void start_prompt(board& board) {
	do {
		system("CLS");
		std::string size_input = "6";
		std::cout << "Witaj w grze Black Box! Aby zwyciezyc, znajdz kazdy atom na planszy za pomoca promieni, ktore wystrzeliwujesz z brzegow planszy\nPodaj wielkosc planszy (dostepne sa rozmiary: 5, 8, 10): ";

		std::getline(std::cin, size_input);
		board.ui_size = std::stoi(size_input) + 1;
	} while (!(board.ui_size - 1 == 5) && !(board.ui_size - 1 == 8) && !(board.ui_size - 1 == 10));
}

/*
* wysylamy promien z miejsca oznaczonego x, y
* sprawdzamy, czy na poczatku zdarzylo sie odbicie lub trafienie
* jesli nie, to idziemy dalej
* jezeli trafimy na trafienie, to bierzemy x lub y (ktorekolwiek bylo 0 lub board.ui_size) i je zwracamy
* jezeli trafimy na 2 atomy powodujace odbicie, to bierzemy x lub y (ktorekolwiek bylo 0 lub board.ui_size) i je zwracamy
* jezeli trafimy na atom powodujacy rykoszet, to odpowienio zmieniamy trase promienia (pamietac by zapisac poczatkowe x, y)!
*/

void use_cursor(board& board) {
	switch (board.player_input) {
	case 'W':
	case 'w':
		if (board.y == 0) board.y = board.ui_size;
		else if ((board.x == 0 || board.x == board.ui_size) && board.y == 1) board.y = board.ui_size - 1;
		else board.y--;

		break;
	case 'S':
	case 's':
		if (board.y == board.ui_size) board.y = 0;
		else if ((board.x == 0 || board.x == board. ui_size) && board.y == board.ui_size - 1) board.y = 1;
		else board.y++;

		break;
	case 'A':
	case 'a':
		if (board.x == 0) board.x = board.ui_size;
		else if ((board.y == 0 || board.y == board. ui_size) && board.x == 1) board.x = board.ui_size - 1;
		else board.x--;

		break;
	case 'D':
	case 'd':
		if (board.x == board.ui_size) board.x = 0;
		else if ((board.y == 0 || board.y == board.ui_size) && board.x == board.ui_size - 1) board.x = 1;
		else board.x++;

		break;
	case ' ':
		std::cout << ray_shoot(board);
		break;

	case 'K':
	case 'k':
		std::cout << "dziekujemy za gre!";
		 
	}
}

void print_board(board& game_board) {
	//system("CLS");
	std::cout << "\n\n";
	for (unsigned col = 0; col <= game_board.ui_size; col++) {
		std::cout << ' ';
		for (unsigned row = 0; row <= game_board.ui_size; row++) {
			unsigned printer_position = row + col * (game_board.ui_size + 1);
			unsigned position = game_board.x + game_board.y * (game_board.ui_size + 1);

			//sprawdza czy w miejscu jest atom
			bool atom_is_here = false;
			if ((row != 0) || (row != game_board.ui_size) || (col != 0) || (col != game_board.ui_size))
				for (unsigned i = 0; i < game_board.atom_position_list.size(); i++)
					if (game_board.atom_position_list[i] == printer_position) atom_is_here = true;


			if ((position == printer_position) && (col == game_board.y)) std::cout << '@';
			else if (row == 0) {
				if (col == 0) std::cout << '/';
				else if (col == game_board.ui_size) std::cout << '\\';
				else std::cout << '=';
			}
			else if (row == game_board.ui_size) {
				if (col == 0) std::cout << '\\';
				else if (col == game_board.ui_size) std::cout << '/';
				else std::cout << '=';
			}
			else if (col == 0 || col == game_board.ui_size) std::cout << '=';
			else if (atom_is_here) std::cout << 'O';
			else std::cout << printer_position;
			std::cout << '\t';
		}
		std::cout << '\n';
	}
}

int main() {
	//moze zmienia znaki nwm
	//system("chcp 852");

	board game_board;
	start_prompt(game_board);
	random_atom_positions(game_board);

	game_board.x = game_board.ui_size - 1;
	game_board.y = game_board.ui_size;

	do{
		print_board(game_board);

		std::cout << "\n\nco robisz wariacie: ";
		std::string a;
		std::getline(std::cin,a);
		game_board.player_input = a[0];
		if (game_board.player_input == 'k') {
			std::cout << "dzieki za gre!";
			return 0;
		}

		use_cursor(game_board);

	} while (true);

	return 0;
}

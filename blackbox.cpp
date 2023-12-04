#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <istream>


//plansza
struct Board {
	unsigned ui_size = 1;
	unsigned x = ui_size - 1, y = ui_size - 1;
	unsigned atom_amount = 3;

	unsigned E_amount = 0;

	unsigned atom_position_list[8] = {};
	//std::vector <std::tuple<unsigned, unsigned, unsigned, unsigned, char> > display;
	char player_input = 'w';
};
void bubble_sort(unsigned arr[], unsigned size) {

	unsigned buffer;
	for (unsigned i = 0; i < (size - 1); i++)
	{
		for (unsigned j = 0; j < (size - i - 1); j++)
		{
			if (arr[j] > arr[j + 1])
			{
				buffer = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = buffer;
			}
		}
	}
}

void random_atom_positions(Board& board) {

	bool error_present = false;

	switch (board.ui_size) {
	case 6:
		board.atom_amount = 3;
		break;
	case 9:
		board.atom_amount = 5;
		break;
	case 11:
		board.atom_amount = 8;
		break;
	}
	srand(time(NULL));

	do {
		error_present = false;
		//generuje miejsca na podstawie calej planszy od pierwszego miejsca na atomy do ostatniego (nie sa wykluczone brzegi)
		for (unsigned i = 0; i < board.atom_amount; i++)
			board.atom_position_list[i] = ((rand() % ((board.ui_size * board.ui_size) - 4) + board.ui_size + 2));

		bubble_sort(board.atom_position_list, board.atom_amount);

		//sprawdzamy czy miejsca sie powtarzaja i generujemy liczby ponownie, jesli tak
		for (unsigned i = 1; i < board.atom_amount; i++) {
			if (board.atom_position_list[i - 1] == board.atom_position_list[i] || board.atom_position_list[i - 1] == board.atom_position_list[i] + 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] - 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] + board.ui_size + 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] - (board.ui_size + 1)) {
				error_present = true;
				break;
			}
		}

		//sprawdzamy czy sa atomy na prawej lub lewej stronie planszy i generujemy ponownie, jesli tak
		for (unsigned i = 0; i < board.atom_amount; i++) {
			for (unsigned j = 1; j < board.ui_size; j++) {
				if (board.atom_position_list[i] == j * (board.ui_size + 1) || board.atom_position_list[i] == ((j + 1) * (board.ui_size + 1)) - 1) {
					error_present = true;
					break;
				}
			}
		}

	} while (error_present);
}
/*
void shoot_history(board& board, unsigned x_init, unsigned y_init, char collision_type) {
	board.display.push_back(std::make_tuple(x_init, y_init, board.x, board.y, collision_type));
}
*/
void ray_shoot(Board& board, char start_direction) {
	unsigned x_init = board.x, y_init = board.y;

	bool 
		collision = false,
		atom1 = false,
		atom2 = false,
		finish = false;

	char collision_type = start_direction;
	
	while (!finish) {
		unsigned a = 0;
		switch (collision_type) {
			//kursor w górê
		case 'u':
			collision = false;
			atom1 = false, atom2 = false;
			while (!collision) {
				while (a != board.atom_amount) {
					if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size == board.atom_position_list[a]) {//atom prawa góra
						if (board.y == board.ui_size) {
							collision_type = 'R';
							collision = true;
							finish = true;
							break;
						}
						else {
							atom1 = true;
							collision = true;
							collision_type = 'l';
						}
					}
					else if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size - 2 == board.atom_position_list[a]) {//atom lewa góra
						if (board.y == board.ui_size) {
							collision_type = 'R';
							collision = true;
							finish = true;
							break;
						}
						else { 
							atom2 = true; 
							collision = true; 
							collision_type = 'r';
						}	
					}
					else if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size - 1 == board.atom_position_list[a]) {//atom góra
						collision_type = 'H';
						finish = true;
						collision = true;
						break;
					}		
					if (atom1 && atom2) {//atom prawa góra i lewa góra
						collision_type = 'R';
						collision = true;
						finish = true;
						break;
					}
					a++;
				}
				if (!collision && !atom1 && !atom2) {//brak atomów nad kursorem
					board.y--;
					a = 0;
				}
				if (board.y == 0) {//czy dotarliœmy do koñca
					collision_type = 'E';
					board.E_amount++;
					finish = true;

					break;
				}
			}
			break;
			//kursor w dó³
		case 'd':
			collision = false;
			atom1 = false, atom2 = false;
			while (!collision) {
				while (a != board.atom_amount) {
					//atom lewy dó³
					if ((board.x + board.y * (board.ui_size + 1)) + board.ui_size == board.atom_position_list[a]) {
						if (board.y == 0) {
							collision_type = 'R';
							collision = true;
							finish = true;
							break;
						}
						else {
							atom1 = true;
							collision = true;
							collision_type = 'r';
						}
					}
					//atom prawy dó³
					else if ((board.x + board.y * (board.ui_size + 1)) + board.ui_size + 2 == board.atom_position_list[a]) {
						if (board.y == 0) {
							collision_type = 'R';
							collision = true;
							finish = true;
							break;
						}
						else {
							atom2 = true;
							collision = true;
							collision_type = 'l';
						}
					}
					//atom dó³
					else if ((board.x + board.y * (board.ui_size + 1)) + board.ui_size + 1 == board.atom_position_list[a]) {
						collision_type = 'H';
						finish = true;
						collision = true;
						break;
					}
					//atom lewy dó³ i prawy dó³
					if (atom1 && atom2) {
						collision_type = 'R';
						collision = true;
						finish = true;
						break;
					}
					a++;
				}
				
				if (!collision && !atom1 && !atom2) {
					board.y++;
					a = 0;
				}
				if (board.y == board.ui_size) {//czy dotarliœmy do koñca
					collision_type = 'E';
					board.E_amount++;
					finish = true;
					break;
				}
			}
			break;
			//kursor w prawo
		case 'r':
			collision = false;
			atom1 = false, atom2 = false;
			while (!collision) {
				while (a != board.atom_amount) {
					if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size == board.atom_position_list[a]) {					//atom prawa góra
						if (board.x == 0) {
							collision_type = 'R';
							collision = true;
							finish = true;
							break;
						}
						else {
							atom1 = true;
							collision = true;
							collision_type = 'd';
						}
					}
					else if ((board.x + board.y * (board.ui_size + 1)) + board.ui_size + 2 == board.atom_position_list[a]) {					//atom prawy dó³
						if (board.x == 0) {
							collision_type = 'R';
							collision = true;
							finish = true;
							break;
						}
						else {
							atom2 = true;
							collision = true;
							collision_type = 'u';
						}
					}
					else if ((board.x + board.y * (board.ui_size + 1)) + 1 == board.atom_position_list[a]) {//atom prawy
						collision_type = 'H';
						finish = true;
						collision = true;
						break;
					}
					if (atom1 && atom2) {//atom prawa góra i prawy dó³
						collision_type = 'R';
						collision = true;
						finish = true;
						break;
					}
					a++;
				}
				if (!collision && !atom1 && !atom2) {
					board.x++;
					a = 0;
				}
				if (board.x == board.ui_size) {//czy dotarliœmy do koñca
					collision_type = 'E';
					board.E_amount++;
					finish = true;
					break;
				}

			}
			break;
			//kursor w lewo
		case 'l':
			collision = false;
			atom1 = false, atom2 = false;
			while (!collision) {
				while (a != board.atom_amount) {
					//atom lewa góra
					if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size - 2 == board.atom_position_list[a]) {
						if (board.x == board.ui_size) {
							collision_type = 'R';
							collision = true;
							finish = true;
							break;
						}
						else {
							atom1 = true;
							collision = true;
							collision_type = 'd';
						}
					}
					//atom lewy dó³
					else if ((board.x + board.y * (board.ui_size + 1)) + board.ui_size == board.atom_position_list[a]) {
						if (board.x == board.ui_size) {
							collision_type = 'R';
							collision = true;
							finish = true;
							break;
						}
						else {
							atom2 = true;
							collision = true;
							collision_type = 'u';
						}
					}
					//atom lewy
					else if ((board.x + board.y * (board.ui_size + 1)) - 1 == board.atom_position_list[a]) {
						collision_type = 'H';
						finish = true;
						collision = true;
						break;
					}
					//atom lewa góra i lewy dó³
					if (atom1 && atom2) {
						collision_type = 'R';
						collision = true;
						finish = true;
						break;
					}
					a++;
				}
				
				if (!collision && !atom1 && !atom2) {
					board.x--;
					a = 0;
				}
				
				if (board.x == 0) {//czy dotarliœmy do koñca
					collision_type = 'E';
					board.E_amount++;
					finish = true;
					break;
				}

			}
			break;

		}//end of switch
		//std::string coordinates = "x" + std::to_string(board.x) + "y" + std::to_string(board.y);
		std::cout << collision_type << /*coordinates << */ '\n';
	}
	//shoot_history(board, x_init, y_init, collision_type);
	board.y = y_init;
	board.x = x_init;
}

void start_prompt(Board& board) {
	do {
		system("CLS");
		char input[2];
		unsigned size_input;
		std::cout << "Witaj w grze Black Box! Aby zwyciezyc, znajdz kazdy atom na planszy za pomoca promieni, ktore wystrzeliwujesz z brzegow planszy\nPodaj wielkosc planszy (dostepne sa rozmiary: 5, 8, 10): ";
		std::cin >> input;
		board.ui_size = std::atoi(input) + 1;
	} while (!(board.ui_size - 1 == 5) && !(board.ui_size - 1 == 8) && !(board.ui_size - 1 == 10));
}

void use_cursor(Board& board) {
	char drctn = ' ';
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
		if (board.y == board.ui_size) drctn = 'u';
		else if (board.x == 0) drctn = 'r';
		else if (board.y == 0) drctn = 'd';
		else if (board.x == board.ui_size) drctn = 'l';
		else std::cout << "AAAAAA";
		ray_shoot(board, drctn);

		break;

	case 'K':
	case 'k':
		std::cout << "dziekujemy za gre!";
		 
	}
}
/*
std::vector<std::pair<unsigned,char>> get_result(board& board, char side) {
	std::vector <std::pair<unsigned, char>> display = {};
	for (unsigned i = 0; i < board.display.size(); i++) {
		unsigned
			xi = std::get<0>(board.display[i]),
			yi = std::get<1>(board.display[i]),
			xx = std::get<2>(board.display[i]),
			yy = std::get<3>(board.display[i]);

		char cllsn = std::get<4>(board.display[i]);

		switch(side){
		case 'u':
			if(yi == 0)
				display.push_back(std::make_pair(xi, cllsn));
			break;
		case 'd':
			if(yi == board.ui_size)
				display.push_back(std::make_pair(xi, cllsn));
			break;
		case 'l':
			if (xi == 0)
				display.push_back(std::make_pair(yi, cllsn));
			break;
		case 'r':
			if (xi == board.ui_size)
				display.push_back(std::make_pair(yi, cllsn));
			break;
		}
	}
	std::sort(display.begin(), display.end());
	return display;
}

void print_top_result(board& game_board) {

	std::vector <std::pair<unsigned, char>> top_display = get_result(game_board, 'u');

	std::string top_string;
	top_string.insert(top_string.begin(), 2 * game_board.ui_size, ' ');

	for (unsigned j = 0; j < game_board.ui_size; j++) {
		for (unsigned i = 0; i < top_display.size(); i++) {
			if (top_display[i].first == j) {
				top_string[2 * j] = top_display[i].second;
				break;
			}
		}

	}
	std::cout << top_string << '\n';
}

void print_bottom_result(board& game_board) {
	std::vector <std::pair<unsigned, char>> bottom_display = get_result(game_board, 'd');

	std::string bottom_string;
	bottom_string.insert(bottom_string.begin(), 2 * game_board.ui_size, ' ');

	for (unsigned j = 0; j < game_board.ui_size; j++) {
		for (unsigned i = 0; i < bottom_display.size(); i++) {
			if (bottom_display[i].first == j) {
				bottom_string[2 * j] = bottom_display[i].second;
				break;
			}
		}

	}
	std::cout << bottom_string << '\n';
}

void print_left_result(board& game_board, unsigned row) {
	std::vector <std::pair<unsigned, char>> left_display = get_result(game_board, 'l');
	for (unsigned i = 0; i < left_display.size(); i++){
		if (!left_display[i].first) {
			left_display[i].first = i;
			left_display[i].second = ' ';
		}
	}
	std::cout << left_display[row].second;
}
*/
void print_board(Board& game_board) {
	//system("CLS");
	std::cout << "\n\n";
	for (unsigned row = 0; row <= game_board.ui_size; row++) {
		for (unsigned col = 0; col <= game_board.ui_size; col++) {
			unsigned printer_position = col + row * (game_board.ui_size + 1);
			unsigned position = game_board.x + game_board.y * (game_board.ui_size + 1);

			//sprawdza czy w miejscu jest atom
			bool atom_is_here = false;
			if ((col != 0) || (col != game_board.ui_size) || (row != 0) || (row != game_board.ui_size))
				for (unsigned i = 0; i < game_board.atom_amount; i++)
					if (game_board.atom_position_list[i] == printer_position) atom_is_here = true;

			/*
			for (unsigned i = 0; i < game_board.display.size(); i++) {
				unsigned 
					xi = std::get<0>(game_board.display[i]),
					yi = std::get<1>(game_board.display[i]),
					xx = std::get<2>(game_board.display[i]),
					yy = std::get<3>(game_board.display[i]);

				char cllsn = std::get<4>(game_board.display[i]);
				/*
				if (row == 0) {
					if (std::get <3>(game_board.display[i]) == 'H') {

					}
				}
				
				if(row == 0 && col == 0){
					if ((yi == 0) || (yy == 0)) {
						for (unsigned i = 0; i < game_board.ui_size; i++) {
							if (xi == i)
								std::cout << cllsn;
						}
					}
					//std::string output = std::to_string(xi) + ' ' + std::to_string(yi) + ' ' + std::to_string(xx) + ' ' + std::to_string(yy) + ' ' + cllsn + '\n';
					//std::cout << output;
				}
			}
			*/

			if (printer_position % (game_board.ui_size + 1) == 0) {
				//print_left_result(game_board, row);
			}
			
			if ((position == printer_position) && (row == game_board.y)) std::cout << '@';
			else if (col == 0) {
				if (row == 0) {
					//print_top_result(game_board);
					std::cout << '/';
				}
				else if (row == game_board.ui_size) {
					std::cout << '\\';
				}
				else std::cout << '=';
			}
			else if (col == game_board.ui_size) {
				if (row == 0) std::cout << '\\';
				else if (row == game_board.ui_size) std::cout << '/';
				else std::cout << '=';
			}
			else if (row == 0 || row == game_board.ui_size) std::cout << '=';
			else if (atom_is_here) std::cout << 'O';
			else std::cout << '-';
			std::cout << ' ';
			
			if (printer_position == ((game_board.ui_size + 1) * (game_board.ui_size + 1)) - 1) {
				std::cout << '\n';
				//print_bottom_result(game_board);
			}
			
		}
		std::cout << '\n';
	}
}

int main() {
	//moze zmienia znaki nwm
	//system("chcp 852");

	Board game_board;
	start_prompt(game_board);
	random_atom_positions(game_board);

	game_board.x = game_board.ui_size - 1;
	game_board.y = game_board.ui_size;

	char input[2];

	do{
		print_board(game_board);

		std::cout << "\n\nco robisz wariacie: ";
		std::cin.getline(input, 1);
		game_board.player_input = input;
		if (game_board.player_input == 'k') {
			std::cout << "dzieki za gre!";
			return 0;
		}

		use_cursor(game_board);

	} while (true);

	return 0;
}

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <istream>
#include <cstring>

//plansza
struct Board {
	unsigned ui_size = 1;
	unsigned x = ui_size - 1, y = ui_size - 1;

	unsigned atom_amount = 3;
	unsigned atom_position_list[8] = {};

	unsigned shot_count = 0;
	unsigned* shot_history = nullptr;

	// 0 -- E, 1 -- H, 2 -- R
	unsigned* shot_type_history = nullptr;
	//std::vector <std::tuple<unsigned, unsigned, unsigned, unsigned, char> > display;

	char player_input[2];
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
/*
void add_char_to_char_arr(char* array, char character){
	unsigned length = std::strlen(array);
    array[length] = character;
    array[length + 1] = '\0';
}
*/
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

void make_shot_history(Board& board, unsigned init_x, unsigned init_y, unsigned cllsn_type) {
	board.shot_count += 4;
	bool duplicate = false;
	if (board.shot_history == nullptr) {
		board.shot_history = new unsigned[4];
		// 0 -- E, 1 -- H, 2 -- R
		board.shot_type_history = new unsigned[(board.shot_count / 4) + 1];

		for (unsigned i = 0; i < board.shot_count; i += 4) {
			board.shot_history[i] = board.x;
			board.shot_history[i + 1] = board.y;
			board.shot_history[i + 2] = init_x;
			board.shot_history[i + 3] = init_y;

			board.shot_type_history[i] = cllsn_type;
		}
	}
	else{
		for (unsigned i = 0; i < board.shot_count; i += 4) {
			if ((board.shot_history[i + 2] == init_x) && (board.shot_history[i + 3] == init_y)) {
				std::cout << "juz stad strzelales!";
				duplicate = true;
				break;
				board.shot_count -= 4;
			}
		}
		if (!duplicate) {
			unsigned* sh_temp = new unsigned[sizeof(int) * board.shot_count];
			unsigned* sh_type_temp = new unsigned[sizeof(int) * ((board.shot_count / 4) + 1)];

			for (unsigned i = 0; i < board.shot_count; i += 4) {
				sh_temp[i] = board.shot_history[i];
				sh_temp[i + 1] = board.shot_history[i + 1];
				sh_temp[i + 2] = board.shot_history[i + 2];
				sh_temp[i + 3] = board.shot_history[i + 3];

				sh_type_temp[i] = board.shot_type_history[i];
			}
			sh_temp[board.shot_count - 4] = board.x;
			sh_temp[board.shot_count - 3] = board.y;
			sh_temp[board.shot_count - 2] = init_x;
			sh_temp[board.shot_count - 1] = init_y;

			sh_type_temp[(board.shot_count / 4) - 1] = cllsn_type;

			delete[] board.shot_history;
			board.shot_history = sh_temp;

			delete[] board.shot_type_history;
			board.shot_type_history = sh_type_temp;
		}
	}
}

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
					//board.E_amount++;
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
					//board.E_amount++;
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
					//board.E_amount++;
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
					//board.E_amount++;
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

	unsigned cllsn_type_int;
	switch (collision_type) {
	case 'E':
		cllsn_type_int = 0;
		break;
	case 'H':
		cllsn_type_int = 1;
		break;
	case 'R':
		cllsn_type_int = 2;
	}
	make_shot_history(board, x_init, y_init, cllsn_type_int);
	board.y = y_init;
	board.x = x_init;
}

void start_prompt(Board& board) {
	do {
		//system("CLS");
		char input[3];
		std::cout << "Witaj w grze Black Box! Aby zwyciezyc, znajdz kazdy atom na planszy za pomoca promieni, ktore wystrzeliwujesz z brzegow planszy\nPodaj wielkosc planszy (dostepne sa rozmiary: 5, 8, 10): ";
		std::cin >> input;
		board.ui_size = std::atoi(input) + 1;
	} while (!(board.ui_size - 1 == 5) && !(board.ui_size - 1 == 8) && !(board.ui_size - 1 == 10));
}

void use_cursor(Board& board) {
	char drctn = ' ';
	switch (board.player_input[0]) {
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

int change_char_in_string(Board board, char* string, unsigned index_j, unsigned index_i) {
		if (board.shot_type_history[index_j / 4] == 0) {
			char number = char(index_j + 48);
			string[index_i] = number;
			return 0;
		}
		else if (board.shot_type_history[index_j / 4] == 1) {
			string[index_i] = 'H';
			return 0;
		}

		else if (board.shot_type_history[index_j / 4] == 2) {
			string[index_i] = 'R';
			return 0;
		}
		return 0;
}

char* generate_result_string(Board board) {

	char display[41];
	std::fill(display, display + 40, ' ');
	for (unsigned j = 0; j < board.shot_count; j += 4) {
		if (board.shot_history[j + 3] == 0 && board.shot_history[j + 2] > 0 && board.shot_history[j + 2] < board.ui_size) {
			/*
			if (board.shot_type_history[j / 4] == 0) {
				char number = char(j + 48);
				display[i] = number;
				break;
			}
			else if (board.shot_type_history[j / 4] == 1) {
				display[i] = 'H';
				break;
			}

			else if (board.shot_type_history[j / 4] == 2) {
				display[i] = 'R';
				break;
			}
			else {
				break;
			}
			*/
			unsigned i = 0;
			switch (board.shot_history[j+3]) {
			case 0:
				i = board.shot_history[j + 2];
			case 1:
				i = board.shot_history[j + 2] + 20;
			}
			change_char_in_string(board, display, j, i);
		}
		else if (board.shot_history[j + 3] == board.ui_size && board.shot_history[j + 2] > 0 && board.shot_history[j + 2] < board.ui_size) {
			//change_char_in_string(board, display, j, i);
		

		}
	}
	return display;
}
/*
char* print_display(Board& board, unsigned side) {
	//side: 0 -- up 1 -- right, 2 -- down, 3 -- left
	char display[11];
	std::fill(display, display + 10, ' ');
	switch (side) {
	case 0:
		for (unsigned i = 0; i < board.ui_size; i++) {
			for (unsigned j = 0; j < board.shot_count; j += 4) {
				if (board.shot_history[j + 3] == 0 && board.shot_history[j + 2] > 0 && board.shot_history[j + 2] < board.ui_size) {
					if (board.shot_type_history[j / 4] == 0) {
						char number = char(j + 48);
						display[i] = number;
						break;
					}
					else if (board.shot_type_history[j / 4] == 1) {
						display[i] = 'H';
						break;
					}

					else if (board.shot_type_history[j / 4] == 2) {
						display[i] = 'R';
						break;
					}
					else {
						break;
					}
						
				}//display[j] = (get_shot_type_char(board, j) == 'E') ? char(48 + j) : get_shot_type_char(board, j);
			}
		}
		return display;
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		std::cout << "BBBBBB!";
	}
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

			if (col == 0) {
				//if()
				//print_left_result(game_board, row);
			}
			
			if ((position == printer_position) && (row == game_board.y)) std::cout << '@';
			else if (col == 0) {
				if (row == 0) {
					//print_display(game_board,0);
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
			}
			
		}
		std::cout << '\n';
	}
	std::cout << *generate_result_string(game_board);
}

int main() {
	//moze zmienia znaki nwm
	//system("chcp 852");

	Board game_board;
	start_prompt(game_board);
	random_atom_positions(game_board);

	game_board.x = 1;
	game_board.y = 0;

	print_board(game_board);
	std::cout << "\n\nco robisz wariacie: ";
	while(true){
		char input[2];
		input[0] = '\0';
		std::cin.getline(input, 2);
		
		game_board.player_input[0] = input[0];
		if (game_board.player_input[0] == 'k') {
			std::cout << "dzieki za gre!";
			delete[] game_board.shot_history;
			delete[] game_board.shot_type_history;
			return 0;
		}
		if ((game_board.player_input[0] == 'w') || (game_board.player_input[0] == 'a') || (game_board.player_input[0] == 's') || (game_board.player_input[0] == 'd') || (game_board.player_input[0] == ' ')) {
			use_cursor(game_board);
			print_board(game_board);
			std::cout << "\n\nco robisz wariacie: ";
		}
	}

	return 0;
}

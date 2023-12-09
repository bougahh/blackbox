#include <iostream>
#include <cstdlib>
#include <ctime>
#include <istream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <limits>
#include <fstream>

const unsigned LIST_SIZE = 8;

struct Board {
	unsigned ui_size;
	unsigned x, y;

	unsigned atom_amount;
	unsigned atom_position_list[LIST_SIZE];

	unsigned user_guess_list[LIST_SIZE];
	unsigned guess_count;

	unsigned shot_count;

	unsigned shot_history[160];


	bool stop_game;

	unsigned shot_type_history[40];
	char player_input[2];

	void set_game() {
		ui_size = 1;
		x = ui_size - 1, y = ui_size - 1;
		atom_amount = 3;
		guess_count = 0;
		shot_count = 0;
		stop_game = false;
		for (unsigned &i : atom_position_list)
			i = NULL;
		for (unsigned &i : user_guess_list)
			i = NULL;
		for (unsigned &i : shot_type_history)
			i = NULL;
		for (unsigned &i : shot_history)
			i = NULL;
	}
};

void print_title() {
	std::cout <<
		"------------------------------------------------------------------\n"
		"`  /////   //     /////   //// //  //    /////    ////   //  //  `\n"
		"-  //   // //    //   // //    // //     //  //  //  //  //  //  -\n"
		"   /////   //    //////  //    ////      ////    //  //   ////    \n"
		"-  //   // //    //   // //    // //     //  //  //  //  //  //  -\n"
		",  /////   ///// //   //  //// //  //    /////    ////   //  //  ,\n"
		"------------------------------------------------------------------\n";
}
void print_help(Board board) {
	std::cout <<
		"Ilosc atomow: " << board.atom_amount <<
		"\nOto lista dostepnych ruchow:\n"
		"\tw,a,s,d - ruch po planszy\n"
		"\tspacja - gdy na brzegu planszy (=) wystrzelenie promienia\n"
		"\to - gdy wewnatrz planszy (-) zaznaczenie podejrzenia miejsca atomu\n"
		"\tH - pokaz na chwile polozenie atomow\n"
		"\tp - zakonczenie aktywnej rozgrywki, zobaczenie odpowiedzi\n"
		"\tu - cofniecie ruchu\n"
		"\tq - wyjscie do menu glownego\n"
		"\tz - zapisz gre\n"
		"\tx - wczytaj gre\n"
		"\tk - zakonczenie rozgrywki\n";
}

void bubble_sort_desc(unsigned arr[], unsigned size) {
	unsigned buffer;
	for (unsigned i = 0; i < (size - 1); i++)
	{
		for (unsigned j = 0; j < (size - i - 1); j++)
		{
			if (arr[j] < arr[j + 1])
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
		for (unsigned i = 0; i < board.atom_amount; i++)
			board.atom_position_list[i] = ((rand() % ((board.ui_size * board.ui_size) - 4) + board.ui_size + 2));

		bubble_sort_desc(board.atom_position_list, board.atom_amount);

		for (unsigned i = 1; i < board.atom_amount; i++) {
			if (board.atom_position_list[i - 1] == board.atom_position_list[i] || board.atom_position_list[i - 1] == board.atom_position_list[i] + 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] - 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] + board.ui_size + 1 || board.atom_position_list[i - 1] == board.atom_position_list[i] - (board.ui_size + 1)) {
				error_present = true;
				break;
			}
		}

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
	bool duplicate = false;
	for (unsigned i = 0; i < board.shot_count; i += 4) {
		if (((board.shot_history[i + 2] == init_x) && (board.shot_history[i + 3] == init_y)) || ((board.shot_type_history[i/4] == 1) && (board.shot_history[i + 2] == board.x) && (board.shot_history[i+3] == board.y))) {
			std::cout << "juz stad strzelales lub strzelasz gdzie wczesniej skonczyl promien";
			duplicate = true;
			break;
		}
	}
	if (!duplicate) {
			board.shot_history[board.shot_count] = board.x;
			board.shot_history[board.shot_count + 1] = board.y;
			board.shot_history[board.shot_count + 2] = init_x;
			board.shot_history[board.shot_count + 3] = init_y;

			board.shot_type_history[(board.shot_count / 4)] = cllsn_type;
			board.shot_count += 4;
		}
}

void save_game(Board board) {
	std::ofstream save("save.txt");
	if (save.is_open()) {
		save << board.ui_size << ' ' << board.atom_amount << ' ' << board.guess_count << ' ' << board.shot_count << "\n\n";
		for (unsigned i = 0; i < LIST_SIZE; i++)
				save << board.atom_position_list[i] << '\n';
		save << '\n';
		for (unsigned i = 0; i < LIST_SIZE; i++)
				save << board.user_guess_list[i] << '\n';
		save << '\n';
		for (unsigned i = 0; i < 160; i+=4) {
			if (!((board.shot_history[i] == 0) && (board.shot_history[i + 1] == 0) && (board.shot_history[i + 2]) == 0 && (board.shot_history[i + 3] == 0)))
				save << i << ' ' << board.shot_history[i] << ' ' << board.shot_history[i + 1] << ' ' << board.shot_history[i + 2] << ' ' << board.shot_history[i + 3] << '\n';
		}
		save << '\n';
		for (unsigned i = 0; i < 40; i++) {
			if (board.shot_type_history[i] != 0) {
				save << i << ' ' << board.shot_type_history[i] << '\n';
			}
		}
		save << '\n';
	}
}

void load_game(Board& board) {
	std::ifstream load("save.txt");
	if (load.is_open()) {
		load >> board.ui_size >> board.atom_amount >> board.guess_count >> board.shot_count;
		for (unsigned i = 0; i < LIST_SIZE; i++)
			load >> board.atom_position_list[i];

		for (unsigned i = 0; i < LIST_SIZE; i++)
			load >> board.user_guess_list[i];

		unsigned index = 0;
		unsigned i = 0;
		unsigned temp_shot_history[160] = { 0 }; // Temporary array to store shot_history data
		load >> index;
		do {
			load >> temp_shot_history[index] >> temp_shot_history[index + 1] >> temp_shot_history[index + 2] >> temp_shot_history[index + 3] >> index;
			i += 4;
		} while (i < board.shot_count && load);

		// Copy temp_shot_history to board.shot_history
		for (unsigned j = 0; j < 160; j++) {
			board.shot_history[j] = temp_shot_history[j];
		}
			
		for (unsigned i = 0; i < board.shot_count / 4; i++)
			load >> board.shot_type_history[index] >> index;
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
		case 'u':
			collision = false;
			atom1 = false, atom2 = false;
			while (!collision) {
				while (a != board.atom_amount) {
					if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size == board.atom_position_list[a]) {
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
					else if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size - 2 == board.atom_position_list[a]) {
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
					else if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size - 1 == board.atom_position_list[a]) {
						collision_type = 'H';
						finish = true;
						collision = true;
						break;
					}		
					if (atom1 && atom2) {
						collision_type = 'R';
						collision = true;
						finish = true;
						break;
					}
					a++;
				}
				if (!collision && !atom1 && !atom2) {
					board.y--;
					a = 0;
				}
				if (board.y == 0) {
					collision_type = 'E';
					finish = true;

					break;
				}
			}
			break;
		case 'd':
			collision = false;
			atom1 = false, atom2 = false;
			while (!collision) {
				while (a != board.atom_amount) {
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
					else if ((board.x + board.y * (board.ui_size + 1)) + board.ui_size + 1 == board.atom_position_list[a]) {
						collision_type = 'H';
						finish = true;
						collision = true;
						break;
					}
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
				if (board.y == board.ui_size) {
					collision_type = 'E';
					finish = true;
					break;
				}
			}
			break;
		case 'r':
			collision = false;
			atom1 = false, atom2 = false;
			while (!collision) {
				while (a != board.atom_amount) {
					if ((board.x + board.y * (board.ui_size + 1)) - board.ui_size == board.atom_position_list[a]) {
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
					else if ((board.x + board.y * (board.ui_size + 1)) + board.ui_size + 2 == board.atom_position_list[a]) {
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
					else if ((board.x + board.y * (board.ui_size + 1)) + 1 == board.atom_position_list[a]) {
						collision_type = 'H';
						finish = true;
						collision = true;
						break;
					}
					if (atom1 && atom2) {
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
				if (board.x == board.ui_size) {
					collision_type = 'E';
					finish = true;
					break;
				}

			}
			break;
		case 'l':
			collision = false;
			atom1 = false, atom2 = false;
			while (!collision) {
				while (a != board.atom_amount) {
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
					else if ((board.x + board.y * (board.ui_size + 1)) - 1 == board.atom_position_list[a]) {
						collision_type = 'H';
						finish = true;
						collision = true;
						break;
					}
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
				
				if (board.x == 0) {
					collision_type = 'E';
					finish = true;
					break;
				}

			}
			break;

		}
	}

	unsigned cllsn_type_int;
	switch (collision_type) {
	case 'E':
		cllsn_type_int = 1;
		break;
	case 'H':
		cllsn_type_int = 2;
		break;
	case 'R':
		cllsn_type_int = 3;
	}
	make_shot_history(board, x_init, y_init, cllsn_type_int);
	board.y = y_init;
	board.x = x_init;
}
void start_prompt(Board& board) {
	char input;
	print_title();
	std::cout << "\nWitaj w grze Black Box!"
		"\nAby zwyciezyc, znajdz kazdy atom na planszy za pomoca promieni, ktore wystrzeliwujesz z brzegow planszy.\n";
	while (true) {
		std::cout <<
			"\nOto mozliwosci wyboru:\n"
			"\t(1) - 5x5 - 3 atomy\n"
			"\t(2) - 8x8 - 5 atomow\n"
			"\t(3) - 10x10 - 8 atomow\n"
			"\t(k) - wyjdz z gry\n"
			"\t(x) - wczytaj gre\n"
			"\nCzas na wybor: ";
		std::cin >> input;
		std::cin.clear();
		while (std::cin.get() != '\n');
		if (input == 'k') {
			board.stop_game = true;
			break;
		}
		else switch (input) {
		case '1':
			board.ui_size = 5 + 1;
			random_atom_positions(board);
			return;
		case '2':
			board.ui_size = 8 + 1;
			random_atom_positions(board);
			return;
		case '3':
			board.ui_size = 10 + 1;
			random_atom_positions(board);
			return;
		case 'x':
		case 'X':
			board.set_game();
			load_game(board);
			return;
		default:
			std::cout << "Nieprawidlowa wielkosc planszy. Sprobuj ponownie.\n";
		}
	}
}

void use_cursor(Board& board) {
	char drctn = ' ';
	bool delete_guess = false;
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
	case 'o':
		delete_guess = false;
		for (unsigned i = 0; i < board.atom_amount; i++) {
			if (board.user_guess_list[i] == board.x + board.y * (board.ui_size + 1)) {
				delete_guess = true;
				board.user_guess_list[i] = 0;
				board.guess_count--;
				bubble_sort_desc(board.user_guess_list, LIST_SIZE);
				goto end;
			}
		}
		if ((board.x != 0) && (board.x != board.ui_size) && (board.y != 0) && (board.y != board.ui_size) && (board.guess_count < board.atom_amount)) {
			board.user_guess_list[board.guess_count] = board.x + board.y * (board.ui_size + 1);
			board.guess_count++;
		}
		else std::cout << "za duzo przypuszczanych atom�w";
		end:
		board.x = 0;
		board.y = 0;
		break;
	case ' ':
		if (board.y == board.ui_size) drctn = 'u';
		else if (board.x == 0) drctn = 'r';
		else if (board.y == 0) drctn = 'd';
		else if (board.x == board.ui_size) drctn = 'l';
		else break;
		ray_shoot(board, drctn);
		break;
	case 'k':
		board.stop_game = true;
		break;
	case 'Q':
	case 'q':
		break;
	case 'Z':
	case 'z':
		save_game(board);
	case 'X':
	case 'x':
		load_game(board);
	default:
		std::cout << "nie rozpoznaje komendy";
		break;
	}
}

char change_char_to_string(unsigned id) {
	if (id == 2) return 'H';
	else if (id == 3) return 'R';
	else return 'E';
}

void set_string_char(Board board, unsigned i, unsigned& E_index, char* buffer, unsigned c) {
	if ((board.shot_type_history[i / 4] != 1))
		buffer[c] = change_char_to_string(board.shot_type_history[i / 4]);
	else if (board.shot_type_history[i / 4] == 1) {
		buffer[c] = char(48 + E_index);
		if (board.shot_history[i + 1] == 0)
			buffer[0 + board.shot_history[i]] = char(48 + E_index++);
		else if (board.shot_history[i] == board.ui_size)
			buffer[9 + board.shot_history[i + 1]] = char(48 + E_index++);
		else if (board.shot_history[i + 1] == board.ui_size)
			buffer[19 + board.shot_history[i]] = char(48 + E_index++);
		else if (board.shot_history[i] == 0)
			buffer[29 + board.shot_history[i + 1]] = char(48 + E_index++);
	}
}

void generate_result_string(Board board, char* string) {
	char buffer[41];
	for (char& ch : buffer)
		ch = ' ';
	buffer[40] = '\0';
	unsigned E_index = 0;
	if (board.shot_count != 0) {
		for (unsigned i = 0; i < board.shot_count; i += 4) {
			for (unsigned c = 0; c < 40; c++) {
				if (c < 10) {
					if ((board.shot_history[i + 3] == 0) && (board.shot_history[i + 2] - 1 == c))
						set_string_char(board, i, E_index, buffer, c);
				}
				else if (c < 20) {
					if ((board.shot_history[i + 2] == board.ui_size) && (10 + board.shot_history[i + 3] - 1 == c))
						set_string_char(board, i, E_index, buffer, c);
				}
				else if (c < 30) {
					if ((board.shot_history[i + 3] == board.ui_size) && (20 + board.shot_history[i + 2] - 1 == c))
						set_string_char(board, i, E_index, buffer, c);
				}
				else if (c < 40) {
					if ((board.shot_history[i + 2] == 0) && (30 + board.shot_history[i + 3] - 1 == c))
						set_string_char(board, i, E_index, buffer, c);
				}
			}
		}
	}
	for (unsigned i = 0; i < 41; i++)
		string[i] = buffer[i];
}

void print_board(Board& game_board) {
	char prnt[41];
	for (char p : prnt)
		p = ' ';
	generate_result_string(game_board, prnt);
	print_title();
	for (unsigned row = 0; row <= game_board.ui_size; row++) {
		for (unsigned col = 0; col <= game_board.ui_size; col++) {
			unsigned printer_position = col + row * (game_board.ui_size + 1);
			unsigned position = game_board.x + game_board.y * (game_board.ui_size + 1);

			bool atom_is_here = false, atom_guess = false;
			if ((col != 0) || (col != game_board.ui_size) || (row != 0) || (row != game_board.ui_size)) {
				for (unsigned i = 0; i < game_board.atom_amount; i++)
					if (game_board.atom_position_list[i] == printer_position) atom_is_here = true;

				for (unsigned i = 0; i < game_board.atom_amount; i++)
					if (game_board.user_guess_list[i] == printer_position) atom_guess = true;
			}

			if (row == 0 && col == 0) {
				std::cout << "   ";
				for (unsigned i = 0; i < 10; i++)
					std::cout << ' ' << prnt[i];
				std::cout << '\n';
			}
			if (col == 0) {
				if (row == 0)
					std::cout << "  ";
				else
					std::cout << prnt[30 + (row - 1)] << ' ';
			}

			if ((position == printer_position) && (row == game_board.y)) std::cout << '@';
			else if (col == 0) {
				if (row == 0) {
					std::cout << '/';
				}
				else if (row == game_board.ui_size) {
					if (game_board.ui_size == 11)
						std::cout << ' ';
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
			else if (game_board.player_input[0] == 'p') {
				if (atom_is_here) std::cout << 'O'; 
				else if (atom_guess)std::cout << 'X';
				else std::cout << '-';
			}
			else if (game_board.player_input[0] == 'H') {
				if (atom_is_here) std::cout << 'O';
				else std::cout << '-';
			}
			else if (atom_guess) std::cout << 'o';
			else std::cout << '-';

			if (col == game_board.ui_size) {
				std::cout << ' ' << prnt[10 + (row - 1)];
			}
			std::cout << ' ';
			
			if (printer_position == ((game_board.ui_size + 1) * (game_board.ui_size + 1)) - 1) {
				std::cout << '\n';
			}

		}
		if(row == game_board.ui_size){
			std::cout << "    ";
			for (unsigned i = 20; i < 30; i++)
				std::cout << prnt[i] << ' ';
		}

		std::cout << '\n';
	}
	print_help(game_board);
}
unsigned score_counting(Board board,unsigned score) {
	for (unsigned i : board.atom_position_list) {
		for (unsigned j : board.user_guess_list) {
			if (j == i && j != 0)
				score++;
		}
	}
	return score;
}
int main() {
	Board game_board;
	unsigned total_score = 0;
	char input[2];

start:
	game_board.set_game();
	start_prompt(game_board);

	while (true) {
		if (game_board.stop_game) {
			total_score += score_counting(game_board, total_score);
			std::cout << "Twoj wynik to: " << total_score;
			goto end;
		}

		print_board(game_board);
		std::cout << "co robisz wariacie: ";

		input[0] = '\0';

		if (game_board.player_input[0] == 'H') {
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
			print_board(game_board);
			game_board.player_input[0] = '\0';
		}
		else if (game_board.player_input[0] == 'q' || game_board.player_input[0] == 'Q') {
			game_board.player_input[0] = '\0';
			goto start;
		}
		else if (game_board.player_input[0] == 'p') {
			std::cout << "\n\nWcisnij Enter, jesli chcesz rozpoczac nowa gre";
			std::cin.get();
			game_board.player_input[0] = '\0';
			goto start;
		}
		else {
			std::cin.get(input, 2);
			std::cin.clear();
			std::cin.ignore(200, '\n');
		}
		game_board.player_input[0] = input[0];
		use_cursor(game_board);
	}

end:
	std::cout << "\nDzieki za gre!";
	return 0;
}
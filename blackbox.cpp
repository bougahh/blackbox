#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <istream>
#include <cstring>
#include <chrono>
#include <thread>

//plansza
struct Board {
	unsigned ui_size;
	unsigned x, y;

	unsigned atom_amount;
	unsigned atom_position_list[8];

	unsigned user_guess_list[8];
	unsigned guess_count;

	unsigned shot_count;
	unsigned shot_history[400];

	unsigned E_index;

	bool stop_game;

	// 0 -- E, 1 -- H, 2 -- R
	unsigned shot_type_history[100];
	char player_input[2];

	void set_game() {
		ui_size = 1;
		x = ui_size - 1, y = ui_size - 1;
		atom_amount = 3;
		guess_count = 0;
		shot_count = 0;
		E_index = 0;
		stop_game = false;
		for (unsigned i : atom_position_list)
			i = 0;
		for (unsigned i : user_guess_list)
			i = 0;
		for (unsigned i : shot_type_history)
			i = 0;
		for (unsigned i : shot_history)
			i = 0;
	}
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

void make_shot_history(Board& board, unsigned init_x, unsigned init_y, unsigned cllsn_type) {
	bool duplicate = false;
	for (unsigned i = 0; i < board.shot_count; i += 4) {
		if ((board.shot_history[i + 2] == init_x) && (board.shot_history[i + 3] == init_y)) {
			std::cout << "juz stad strzelales!";
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
		char input[3];
		std::cout << "Witaj w grze Black Box! Aby zwyciezyc, znajdz kazdy atom na planszy za pomoca promieni, ktore wystrzeliwujesz z brzegow planszy\n\nMozesz zawsze opuscic gre poprzez wpisanie litery 'k'\n\nPodaj wielkosc planszy (dostepne sa rozmiary: 5, 8, 10): ";
		std::cin >> input;
		if (input[0] == 'k') {
			board.stop_game = true;
			break;
		}
		board.ui_size = std::atoi(input) + 1;
		std::cout << "\n\n\n";
	} while (!(board.ui_size - 1 == 5) && !(board.ui_size - 1 == 8) && !(board.ui_size - 1 == 10));
}

void use_cursor(Board& board) {
	char drctn = ' ';
	unsigned score = 0;
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
		if (board.x != 0 && board.x != board.ui_size && board.y != 0 && board.y != board.ui_size && board.guess_count < 8) {
			board.user_guess_list[board.guess_count] = board.x + board.y * (board.ui_size + 1);
			board.guess_count++;
		}
		else std::cout << "za duzo przypuszczanych atomów";
		board.x = 1;
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
		bubble_sort(board.user_guess_list, 8);
		for (unsigned i = 0; i < board.ui_size; i++) {
			if (board.atom_position_list[i] == board.user_guess_list[i]) {
				if (board.atom_position_list != 0)
					score++;
			}
		}
		std::cout << "Twój wynik to: " << score;
		break;
	case 'Q':
	case 'q':
		break;
	default:
		std::cout << "nie rozpoznaje komendy";
		break;
	}
}

int change_char_in_string(Board board, char* string, unsigned index_j, unsigned index_i) {
	if (board.shot_type_history[index_j / 4] == 1) {
		string[index_i] = 'H';
		return 0;
	}
	else if (board.shot_type_history[index_j / 4] == 2) {
		string[index_i] = 'R';
		return 0;
	}
	else {
		char number = char(board.E_index + 48);
		string[index_i] = number;
		board.E_index++;
		return 0;
	}
}

void generate_result_string(Board board, char* real_display) {
	board.E_index = 0;
	char display[41];
	for (unsigned i = 0; i < 40; i++)
		display[i] = ' ';
	for (unsigned j = 0; j < board.shot_count; j += 4) {
		unsigned i = 0;
		if (board.shot_history[j + 3] == 0 && board.shot_history[j + 2] > 0 && board.shot_history[j + 2] < board.ui_size) {
			if (board.shot_history[j + 3] == 0)
				i = board.shot_history[j + 2];
			else if (board.shot_history[j + 3] == board.ui_size)
				i = board.shot_history[j + 2] + 20;
			change_char_in_string(board, display, j, i);
		}
		else if (board.shot_history[j + 2] == 0 && board.shot_history[j + 3] > 0 && board.shot_history[j + 3] < board.ui_size) {
			if (board.shot_history[j + 2] == 0)
				i = board.shot_history[j + 3] + 10;
			else if (board.shot_history[j + 2] == board.ui_size)
				i = board.shot_history[j + 3] + 30;
			change_char_in_string(board, display, j, i);
		}
	}
	display[40] = 0;
	for (unsigned i = 0; i < 41; i++)
		real_display[i] = display[i];
}
void print_board(Board& game_board) {
	for(unsigned i = 0; i < 3 * game_board.ui_size; i++)
		std::cout << '\n';
	for (unsigned row = 0; row <= game_board.ui_size; row++) {
		for (unsigned col = 0; col <= game_board.ui_size; col++) {
			unsigned printer_position = col + row * (game_board.ui_size + 1);
			unsigned position = game_board.x + game_board.y * (game_board.ui_size + 1);

			//sprawdza czy w miejscu jest atom
			bool atom_is_here = false, atom_guess = false;
			if ((col != 0) || (col != game_board.ui_size) || (row != 0) || (row != game_board.ui_size))
				for (unsigned i = 0; i < game_board.atom_amount; i++)
					if (game_board.atom_position_list[i] == printer_position) atom_is_here = true;
					else if (game_board.user_guess_list[i] == printer_position) atom_guess = true;

			if (col == 0) {
				//print_left_result(game_board, row);

			}
			*/
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
			std::cout << ' ';
			
			if (printer_position == ((game_board.ui_size + 1) * (game_board.ui_size + 1)) - 1) {
				std::cout << '\n';
			}
			
		}
		std::cout << '\n';
	}
	//char prnt[41] = "wow";
	//generate_result_string(game_board ,prnt);
	//std::cout << prnt;
	std::cout << game_board.shot_count / 4 << std::endl;
	std::cout << " end   start  col_id\n  x y | x y\n";
	for (unsigned i = 0; i < game_board.shot_count; i += 4) {
		std::cout << "  " << game_board.shot_history[i] << ' ' << game_board.shot_history[i + 1] << "   " << game_board.shot_history[i + 2] << ' ' << game_board.shot_history[i + 3] << '\t' << game_board.shot_type_history[i / 4] << std::endl;
	}
}

int main() {
	Board game_board;
start:
	game_board.set_game();
	start_prompt(game_board);
	random_atom_positions(game_board);

	game_board.x = 1;
	game_board.y = 0;

	while(true) {
		if (game_board.stop_game) {
			goto end;
		}
		print_board(game_board);
		std::cout << "\n\nco robisz wariacie: ";
		char input[2];
		input[0] = '\0';
		if (game_board.player_input[0] == 'H') {
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
			print_board(game_board);
		}
		else if (game_board.player_input[0] == 'q' || game_board.player_input[0] == 'Q') {
			game_board.player_input[0] = '\0';
			goto start;
		}
		else if (game_board.player_input[0] == 'p') {
			std::cout << "\n\nWcisnij Enter, jesli chcesz rozpoczac nowa gre";
			std::cin.get();
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
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
//promien do atomow
struct ray {
	unsigned x, y;
	char direction, result;
	unsigned x_start = x, y_start = y;
	unsigned ray_position;
	ray(unsigned xx, unsigned yy, char dir, board& brd) {
		x = xx;
		y = yy;
		direction = dir;
		board = brd;
		x_start = xx;
		y_start = yy;
		ray_position = xx + yy * (brd.ui_size + 1);
	}
	board board;

	//char detect_initial_collision();
	//char ray_detect_collision();
	//void ray_change_direction();
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
			if (board.atom_position_list[i - 1] == board.atom_position_list[i]) {
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
char detect_initial_collision(ray& ray, board& board){
	switch (ray.direction) {
	case '^':
		//zaczynamy od dolu
		for (int i = 0; i < board.atom_position_list.size(); i++) {
			//czy jest odbicie
			if ((board.atom_position_list[i] == ray.ray_position - board.ui_size - 1) || (board.atom_position_list[i] == ray.ray_position - board.ui_size + 1)) {
				return 'R';
				break;
			}
			//czy jest trafienie
			else if (board.atom_position_list[i] == ray.ray_position - board.ui_size) {
				return 'H';
				break;
			}
		}
		break;
	case 'V':
		//zaczynamy od gory
		for (int i = 0; i < board.atom_position_list.size(); i++) {
			//czy jest odbicie
			if ((board.atom_position_list[i] == ray.ray_position + board.ui_size - 1) || (board.atom_position_list[i] == ray.ray_position + board.ui_size + 1)) {
				return 'R';
				break;
			}
			//czy jest trafienie
			else if (board.atom_position_list[i] == ray.ray_position + board.ui_size) {
				return 'H';
				break;
			}
		}
		break;
	case '<':
		//zaczynamy od prawej
		for (int i = 0; i < board.atom_position_list.size(); i++) {
			if ((board.atom_position_list[i] == ray.ray_position - board.ui_size - 2) || (board.atom_position_list[i] == ray.ray_position + board.ui_size)) {
				return 'R';
				break;
			}
			else if (board.atom_position_list[i] == ray.ray_position - 1) {
				return 'H';
				break;
			}
		}
		break;
	case '>':
		//zaczynamy od lewej
		for (int i = 0; i < board.atom_position_list.size(); i++) {
			if ((board.atom_position_list[i] == ray.ray_position - board.ui_size) || (board.atom_position_list[i] == ray.ray_position + board.ui_size + 2)) {
				return 'R';
				break;
			}
			else if (board.atom_position_list[i] == ray.ray_position + 1) {
				return 'H';
				break;
			}
		}
		break;
	}
}
*/
/*
void update_ray_position(ray& ray ,board& board) {
	ray.ray_position = ray.x + ray.y * (board.ui_size + 1);
}
char ray_detect_collision(ray& ray, board& board) {
	unsigned atoms_collided = 0;
	switch (ray.direction) {
	case '^':
		atoms_collided = 0;
		for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
			if (board.atom_position_list[i] == ray.ray_position - board.ui_size - 1)
				atoms_collided++;
			if (board.atom_position_list[i] == ray.ray_position - board.ui_size + 1)
				atoms_collided++;
		}
		if (atoms_collided == 2)
			return 'R';
		else {
			for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
				if (board.atom_position_list[i] == ray.ray_position - board.ui_size - 1) {
					ray.direction = '>';
					break;
				}
				if (board.atom_position_list[i] == ray.ray_position - board.ui_size + 1) {
					ray.direction = '<';
					break;
				}
				if (board.atom_position_list[i] == ray.ray_position - board.ui_size)
					return 'H';
			}
		}
		break;
	case 'V':
		atoms_collided = 0;
		for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
			if (board.atom_position_list[i] == ray.ray_position + board.ui_size - 1)
				atoms_collided++;
			if (board.atom_position_list[i] == ray.ray_position + board.ui_size + 1)
				atoms_collided++;
		}
		if (atoms_collided == 2)
			return 'R';
		else {
			for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
				if (board.atom_position_list[i] == ray.ray_position + board.ui_size - 1) {
					ray.direction = '>';
					break;
				}
				if (board.atom_position_list[i] == ray.ray_position + board.ui_size + 1) {
					ray.direction = '<';
					break;
				}
				if (board.atom_position_list[i] == ray.ray_position + board.ui_size)
					return 'H';
			}
		}
		break;
	case '<':
		atoms_collided = 0;
		for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
			if (board.atom_position_list[i] == ray.ray_position - board.ui_size - 2)
				atoms_collided++;
			if (board.atom_position_list[i] == ray.ray_position + board.ui_size)
				atoms_collided++;
		}
		if (atoms_collided == 2)
			return 'R';
		else {
			for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
				if (board.atom_position_list[i] == ray.ray_position - board.ui_size - 2) {
					ray.direction = 'V';
					break;
				}
				if (board.atom_position_list[i] == ray.ray_position + board.ui_size) {
					ray.direction = '^';
					break;
				}
				if (board.atom_position_list[i] == ray.ray_position - 1)
					return 'H';
			}
		}
		break;
	case '>':
		atoms_collided = 0;
		for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
			if (board.atom_position_list[i] == ray.ray_position - board.ui_size)
				atoms_collided++;
			if (board.atom_position_list[i] == ray.ray_position + board.ui_size + 2)
				atoms_collided++;
		}
		if (atoms_collided == 2)
			return 'R';
		else {
			for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
				if (board.atom_position_list[i] == ray.ray_position - board.ui_size) {
					ray.direction = 'V';
					break;
				}
				if (board.atom_position_list[i] == ray.ray_position + board.ui_size + 2) {
					ray.direction = '^';
					break;
				}
				if (board.atom_position_list[i] == ray.ray_position + 1)
					return 'H';
			}
		}
		break;
	}
}

void ray_move(ray& ray, board& board) {
	switch (ray_detect_collision(ray, board)) {
	case 'R':
		std::cout << ray.y_start << ' ' << ray.x_start;
		ray.result = ray_detect_collision(ray, board);
		break;
	case 'H':
		std::cout << ray.y_start << ' ' << ray.x_start;

		break;
	default:
		do{
			switch (ray.direction) {
			case '^':
				ray.y--;
				update_ray_position(ray, board);
				ray_detect_collision(ray, board);
				break;
			case 'V':
				ray.y++;
				update_ray_position(ray, board);
				ray_detect_collision(ray, board);
				break;
			case '<':
				ray.x--;
				update_ray_position(ray, board);
				ray_detect_collision(ray, board);
				break;
			case '>':
				ray.x++;
				update_ray_position(ray, board);
				ray_detect_collision(ray, board);
				break;
			}
		} while (ray.x != 0 || ray.y != 0);

	}

}


*/
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
/*
	case ' ':
		if (board.x == 0 || board.x == board.ui_size || (board.y == board.ui_size || board.y == 0)) {
			char dir = ' ';
				if (board.x == 0) dir = '>';
				if (board.x == board.ui_size) dir = '<';
				if (board.y == 0) dir = 'V';
				if (board.y == board.ui_size) dir = '^';

				ray new_ray(board.x, board.y, dir, board);
				ray_move(new_ray, board);
		}
		break;
*/
	case 'K':
	case 'k':
		std::cout << "dziekujemy za gre!";
		 
	}
}

void ray_shoot(ray& ray, board& board) {
	if (ray.y == board.ui_size) {
		for (unsigned i = 0; i < board.atom_position_list.size(); i++) {
			if (board.atom_position_list[i] == ray.ray_position - board.ui_size - 1 || board.atom_position_list[i] == ray.ray_position - board.ui_size + 1) {
				std::cout << 'R';
			}
			else if (board.atom_position_list[i] == ray.ray_position - board.ui_size) {
				std::cout << 'H';
			}
		}

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

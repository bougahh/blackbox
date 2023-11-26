#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

//Generuje miejsce, w którym pojawi siê atom
int RandomAtomIndex(unsigned size) {
	return rand() % (size * size);
}

//Tworzy planszê ukryt¹ trzymaj¹c¹ w bool miejsca atomów i prawdziw¹ w char dla gracza
std::vector<char> CreateEmptyBoard(unsigned size) {

	std::vector<char> newBoard;
	std::vector<unsigned> randomAtom;
	std::vector<bool> hiddenBoard;

	unsigned atom_amount = 0;

	switch (size) {
	case 5:
		atom_amount = 3;
		break;
	case 8:
		atom_amount = 5;
		break;
	case 10:
		atom_amount = 8;
		break;
	}

	srand(time(NULL));
	for (unsigned i = atom_amount; i > 0; i--) {
		randomAtom.push_back(RandomAtomIndex(size));
	}

	newBoard.resize(size * size);
	hiddenBoard.resize(size * size);

	for (unsigned i = 0; i < size; i++) {
		for (unsigned j = 0; j < size; j++) {
			for (unsigned k = 0; k < atom_amount; k++) {
				if ((i + j * size) == randomAtom[k]) hiddenBoard[i + j * size] = true;
				else hiddenBoard[i + j * size] = false;
			}
			newBoard[i + j * size] = ' ';
		}
	}

	return newBoard;
}

int main() {
	std::cout << "BlackBox\nPodaj wymiary planszy [1 - 5x5, 2 - 8x8, 3 - 10x10]: ";

	unsigned board_choice, board_size;
	std::cin >> board_choice;

	std::vector <char> board{};

	switch (board_choice) {
	case 1: board_size = 5;
		break;
	case 2: board_size = 8;
		break;
	case 3: board_size = 10;
		break;
	default: {
		board_size = 5;
		std::cout << "Wykryto b³¹d\n";
	}
	}

	board = CreateEmptyBoard(board_size);
	std::cout << "Plansza bêdzie mia³a rozmiar " << board_size << 'x' << board_size << "\n";


	for (unsigned i = 0; i < board_size; i++) {
		for (unsigned j = 0; j < board_size; j++) {
			if (board[i + j * board_size]) std::cout << '1';
			else std::cout << '0';
			std::cout << ' ';
		}
		std::cout << "\n";
	}
	return 0;
}
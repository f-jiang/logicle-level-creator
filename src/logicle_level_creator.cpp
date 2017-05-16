//============================================================================
// Name        : logicle-level-creator.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include "gameboard.h"
#include "json.hpp"

using namespace std;
/*
 * classes:
 * Gameboard
 * - data for circles and squares
 * - dimensions
 * - area
 * - state
 * - shift
 * - reset
 * - colorPopulation
 * - print op
 * Level
 * - colors
 * - gameboard
 * - solutions
 * Solutions
 * - difficulty
 * - each solution as a sequence of moves
 * - lengths of solutions
 *
 * LevelPack
 * - num levels
 * - sort levels
 * - duplicate removal
 * - sections
 *
 */

int main(int argc, char* argv[]) {
	// area of 7 with 6 colours leads to SIGABRT after goto
	gameboard h(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

	h.print_initial();

	std::cout << "1: up, 2: down, 3: left, 4: right" << std::endl;

	int choice = 1;
	while (choice != 0) {
		std::cin >> choice;
		switch (choice) {
		case 1:
			h.shift(gameboard::shift_direction::up);
			h.print_current();
			break;
		case 2:
			h.shift(gameboard::shift_direction::down);
			h.print_current();
			break;
		case 3:
			h.shift(gameboard::shift_direction::left);
			h.print_current();
			break;
		case 4:
			h.shift(gameboard::shift_direction::right);
			h.print_current();
			break;
		default:
			break;
		}
	}

	return 0;
}

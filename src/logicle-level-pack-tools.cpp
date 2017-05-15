//============================================================================
// Name        : logicle-level-pack-tools.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
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

int main() {
	// area of 7 with 6 colours leads to SIGABRT after goto
	gameboard h(1, 7, 6, gameboard::color_distribution::random);
	std::cout << "hi" << std::endl;

	return 0;
}

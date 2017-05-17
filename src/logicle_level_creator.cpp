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
    std::size_t w, h, n;
    cout << "width: ";
    cin >> w;
    cout << "height: ";
    cin >> h;
    cout << "num colors: ";
    cin >> n;
    // area of 7 with 6 colours leads to SIGABRT after goto
    gameboard g(w, h, n);

    g.print_initial();

    std::cout << "w: up, s: down, a: left, d: right, r: reset, q: quit" << std::endl;

    char choice;
    do {
        std::cin >> choice;
        switch (choice) {
        case 'w':
            g.shift(gameboard::shift_direction::up);
            g.print_current();
            break;
        case 's':
            g.shift(gameboard::shift_direction::down);
            g.print_current();
            break;
        case 'a':
            g.shift(gameboard::shift_direction::left);
            g.print_current();
            break;
        case 'd':
            g.shift(gameboard::shift_direction::right);
            g.print_current();
            break;
        case 'r':
            g.reset();
            g.print_current();
            break;
        default:
            break;
        }
    } while (choice != 'q');

    return 0;
}

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
//#include "json.hpp"

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
    cout << "generating gameboard..." << std::endl;
    gameboard g(w, h, n);
    cout << "checking for solutions..." << std::endl;
    solution_set solutions(g);

    std::cout << g;

    char choice;
    do {
        std::cout << "w: up, s: down, a: left, d: right, f: solutions, r: reset, q: quit" << std::endl;
        std::cin >> choice;
        switch (choice) {
        case 'w':
            g.shift(gameboard::shift_direction::up);
            std::cout << g;
            break;
        case 's':
            g.shift(gameboard::shift_direction::down);
            std::cout << g;
            break;
        case 'a':
            g.shift(gameboard::shift_direction::left);
            std::cout << g;
            break;
        case 'd':
            g.shift(gameboard::shift_direction::right);
            std::cout << g;
            break;
        case 'f':
            for (std::size_t i = 0; i <  solutions.n_solutions(); i++) {
                std::cout << solutions[i];
            }
            break;
        case 'r':
            g.reset();
            std::cout << g;
            break;
        default:
            break;
        }
    } while (choice != 'q');

    return 0;
}

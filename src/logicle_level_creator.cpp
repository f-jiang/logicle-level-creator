//============================================================================
// Name        : logicle-level-creator.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <random>
#include "gameboard.h"
#include "json.hpp"
#include "level.h"

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

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0x0, 0xFFFFFF);
    std::vector<unsigned> colors(n);

    for (unsigned& c : colors) {
        c = distribution(generator);
    }

    level l(g, solutions, { 0x000000, 0x111111, 0x222222 });

    std::cout << g;

    char choice;
    do {
        std::cout << "w: up, s: down, a: left, d: right, f: solutions, r: reset, j: json, q: quit" << std::endl;
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
        case 'j':
            std::cout << l.as_json().dump(4) << std::endl;
            break;
        default:
            break;
        }
    } while (choice != 'q');

    return 0;
}

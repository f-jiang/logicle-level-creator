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
    cout << "generating gameboard..." << std::endl;
    gameboard g(w, h, n);
    cout << "checking for solutions..." << std::endl;
    vector<gameboard::shift_direction> solution = g.solution();

    std::cout << g;

    char choice;
    do {
        std::cout << "w: up, s: down, a: left, d: right, f: solution, r: reset, q: quit" << std::endl;
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
            if (solution.empty()) {
                std::cout << "no solution" << std::endl;
            } else {
                std::cout << "solution:" << std::endl;
                for (gameboard::shift_direction d : solution) {
                    switch (d) {
                    case gameboard::shift_direction::up:
                        std::cout << "up ";
                        break;
                    case gameboard::shift_direction::down:
                        std::cout << "down ";
                        break;
                    case gameboard::shift_direction::left:
                        std::cout << "left ";
                        break;
                    case gameboard::shift_direction::right:
                        std::cout << "right ";
                        break;
                    default:
                        break;
                    }
                }
                std::cout << std::endl;
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

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <random>
#include "gameboard.h"
#include "json.hpp"
#include "level.h"
#include "level_pack.h"

using namespace std;

unsigned rand_color() {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(0x0, 0xFFFFFF);
    return distribution(generator);
}

void test_level() {
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

    std::vector<unsigned> colors(n);
    for (unsigned& c : colors) {
        c = rand_color();
    }

    level l(g, solutions, colors);

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

}

// TODO level_pack remaining test cases
void test_level_pack() {
    std::ofstream fout("level_pack.json");
    level_pack levels;

    // TODO fn call is ambiguous, possibly due to lack of struct constructor
    levels.add_category("Hard Boards", {
        {2, 2, {rand_color(), rand_color(), rand_color()}, 1, gameboard::color_distribution::uniform},
        {4, 4, {rand_color(), rand_color(), rand_color()}, 100, gameboard::color_distribution::uniform}
    });

    levels.add_category("Easy Boards", {2, 2, {rand_color(), rand_color(), rand_color()}, 1, gameboard::color_distribution::uniform});
    levels.rename_category("Hard Boards", "Difficult Boards");
    levels.add_category("Trivial Boards", {1, 2, {0, 1}, 1, gameboard::color_distribution::uniform});

    levels.reorder_category("Trivial Boards", 0);
    levels.reorder_category("Easy Boards", 1);
    levels.reorder_category("Difficult Boards", 2);

    fout << levels.as_json().dump(4);
    fout.close();
}

int main(int argc, char* argv[]) {
    // test_level();
    test_level_pack();
    return 0;
}

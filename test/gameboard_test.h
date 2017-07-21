#ifndef GAMEBOARD_TEST_H_
#define GAMEBOARD_TEST_H_

#include "../lib/catch.hpp"
#include "../src/gameboard.h"

#include <vector>
#include <algorithm>
#include <sstream>

TEST_CASE("gameboard is constructed to the correct specifications", "[gameboard]") {
    std::size_t h = 3;
    std::size_t w = 3;
    std::size_t n_colors = 9;
    gameboard::color_distribution cdist = gameboard::color_distribution::uniform;
    gameboard gb(h, w, n_colors, cdist);

    REQUIRE(gb.height() == h);
    REQUIRE(gb.width() == w);
    REQUIRE(gb.area() == h * w);
    REQUIRE(gb.n_colors() == n_colors);
    REQUIRE(gb.color_dist() == cdist);

    std::vector<unsigned> hist(n_colors, 0);
    for (std::size_t i = 0; i < h; i++) {
        for (std::size_t j = 0; j < w; j++) {
            hist[gb.squares().at(i, j)]++;
        }
    }
    REQUIRE(std::count(hist.begin(), hist.end(), hist.front()) == n_colors);
}

// TODO gameboard_test: test cases for when state has been implemented
//TEST_CASE("gameboard state is accurate", "[gameboard]") {
//
//}

TEST_CASE("gameboard comparison", "[gameboard]") {
    gameboard gb(3, 3, 9, gameboard::color_distribution::uniform);

    SECTION("comparing gameboards with the same dimensions, number of colours, colour "
            "distributions, and circle and square configurations") {
        gameboard other(gb);

        REQUIRE(other == gb);
        REQUIRE_FALSE(other != gb);
    }

    SECTION("comparing gameboards with different dimensions") {
        gameboard other(3, 3 + 1, 9, gameboard::color_distribution::uniform);

        REQUIRE(other != gb);
        REQUIRE_FALSE(other == gb);
    }

    SECTION("comparing gameboards with different numbers of colours") {
        gameboard other(3, 3, 9 - 1, gameboard::color_distribution::uniform);

        REQUIRE(other != gb);
        REQUIRE_FALSE(other == gb);
    }
}

TEST_CASE("gameboard can be copied", "[gameboard]") {
    gameboard orig(2, 2, 4);

    SECTION("copying via copy constructor") {
        gameboard copy(orig);

        REQUIRE(copy == orig);
        REQUIRE_FALSE(copy != orig);
    }

    SECTION("copying via assignment operator") {
        gameboard copy = orig;

        REQUIRE(copy == orig);
        REQUIRE_FALSE(copy != orig);
    }
}

TEST_CASE("exceptions are thrown when attempting to construct gameboard with "
        "inappropriate parameters", "[gameboard]") {
    REQUIRE_THROWS(gameboard(3, 3, 2)); // square-circle overlap
    REQUIRE_THROWS(gameboard(1, 1, 3)); // not enough cells
    REQUIRE_THROWS(gameboard(9, 9, 1)); // not enough colors
    REQUIRE_THROWS(gameboard(5, 5, 30)); // too many colors
}

TEST_CASE("gameboard can be shifted and reset", "[gameboard]") {
    gameboard gb(1, 3, 3);

    gb.shift(gameboard::shift_direction::left);
    REQUIRE(gb.current_circles() != gb.initial_circles());

    gb.reset();
    REQUIRE(gb.current_circles() == gb.initial_circles());
}

TEST_CASE("number and lengths of gameboard solutions is accurate", "[gameboard][solutions]") {
    gameboard gb(3, 3, 9);
    solution_set solns(gb);

    std::size_t n_solns = 0;
    std::size_t a = solns.shortest_solution().size();
    std::size_t b = solns.longest_solution().size();
    for (std::size_t i = a; i <= b; i++) {
        n_solns += solns.solutions_with_length(i).size(); 
    }
    REQUIRE(n_solns == solns.n_solutions());
}

TEST_CASE("gameboard solutions are accurate", "[gameboard][solutions]") {
    gameboard gb(3, 3, 4);
    solution_set solns(gb);
    const solution_set::solution* soln;

    for (std::size_t i = 0; i < solns.n_solutions(); i++) {
        soln = &solns[i];

        for (gameboard::shift_direction dir : *soln) {
            gb.shift(dir);
        }

        REQUIRE(gb.current_circles() == gb.squares());
        // REQUIRE(gb.current_state() == gameboard::state::solved);
        gb.reset();
    }
}

// TODO gameboard_test: test case for stream insertion operators - need to be able to construct gameboard manually
//TEST_CASE("stream insertion operators give correct output", "[gameboard][solutions]") {
//    std::ostringstream osstr;
//}

#endif /* GAMEBOARD_TEST_H_ */

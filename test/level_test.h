#ifndef LEVEL_TEST_H_
#define LEVEL_TEST_H_

#include "../lib/catch.hpp"
#include "../src/level.h"

TEST_CASE("creating a level from a pre-existing gameboard and solution set", "[level]") {
    gameboard gb(3, 3, 5);
    solution_set solns(gb);
    level lvl(gb, solns, { 0, 1, 2, 3, 4 });

    REQUIRE(lvl.gameboard_height() == gb.height());
    REQUIRE(lvl.gameboard_width() == gb.width());
    REQUIRE(lvl.difficulty() == solns.difficulty());
    REQUIRE(lvl.n_colors() == gb.n_colors());
    REQUIRE(lvl.n_solutions() == solns.n_solutions());
}

TEST_CASE("creating a level from a pre-existing gameboard", "[level]") {
    gameboard gb(3, 3, 5);
    solution_set solns(gb);
    level lvl(gb, { 0, 1, 2, 3, 4 });

    REQUIRE(lvl.gameboard_height() == gb.height());
    REQUIRE(lvl.gameboard_width() == gb.width());
    REQUIRE(lvl.difficulty() == solns.difficulty());
    REQUIRE(lvl.n_colors() == gb.n_colors());
    REQUIRE(lvl.n_solutions() == solns.n_solutions());
}

TEST_CASE("creating a randomly generated level", "[level]") {
    level lvl(3, 3, { 0, 1, 2, 3, 4 });

    REQUIRE(lvl.gameboard_height() == 3);
    REQUIRE(lvl.gameboard_width() == 3);
    REQUIRE(lvl.n_colors() == 5);
}

TEST_CASE("an exception is thrown when trying to construct a level using inappropriate"
        " parameters", "[level]") {
    gameboard gb(3, 3, 3);

//    SECTION("constructing with too many colours") {
//        REQUIRE_THROWS(level(gb, { 1, 2, 3, 4 }));
//    }
    
    SECTION("constructing with too few colours") {
        REQUIRE_THROWS(level(gb, { 1, 2 }));
    }

//    SECTION("constructing with duplicate, but enough unique, colours") {
//        REQUIRE_NOTHROW(level(gb, { 1, 1, 2, 3 }));
//    }

//    SECTION("constructing with duplicate and not enough unique colours") {
//        REQUIRE_THROWS(level(gb, { 1, 1, 2, 2 }));
//    }
}

// TODO: level_test: as json - will first need gameboard manual generation
TEST_CASE("level's JSON representation is accurate", "[level][json]") {

}

#endif /* LEVEL_TEST_H_ */

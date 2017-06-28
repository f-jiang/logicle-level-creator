#ifndef MATRIX_TEST_H_
#define MATRIX_TEST_H_

#include "../lib/catch.hpp"
#include "../src/matrix.h"

TEST_CASE("can create empty matrices", "[matrix]") {
    matrix<int> mat;
    
    REQUIRE(mat.n_rows() == 0);
    REQUIRE(mat.n_cols() == 0);
    REQUIRE(mat.area() == 0);
}

TEST_CASE("can create a matrix of given dimensions", "[matrix]") {
    matrix<bool> mat(3, 1);

    REQUIRE(mat.n_rows() == 3);
    REQUIRE(mat.n_cols() == 1);
    REQUIRE(mat.area() == 3);
}

TEST_CASE("can fill a matrix upon construction", "[matrix]") {
    matrix<double> mat(4, 4, 3.14);

    REQUIRE(mat.at(3, 2) == 3.14);
}

TEST_CASE("matrix element access", "[matrix]") {
    matrix<double> mat(1, 6, 3.2124234);

    REQUIRE(mat.at(0, 5) == 3.2124234);
    REQUIRE(mat.at(5, 0, true) == 3.2124234);
}

TEST_CASE("matrices can be copied", "[matrix]") {
    matrix<char> orig(3, 6, 'c');

    SECTION("copying via copy constructor") {
        matrix<char> copy(orig);

        REQUIRE(copy.n_rows() == orig.n_rows());
        REQUIRE(copy.n_cols() == orig.n_cols());
        REQUIRE(copy.area() == orig.area());
        REQUIRE(copy.at(2, 2) == orig.at(2, 2));
    }

    SECTION("copying via assignment operator") {
        matrix<char> copy = orig;

        REQUIRE(copy.n_rows() == orig.n_rows());
        REQUIRE(copy.n_cols() == orig.n_cols());
        REQUIRE(copy.area() == orig.area());
        REQUIRE(copy.at(2, 2) == orig.at(2, 2));
    }
}

TEST_CASE("matrix comparison", "[matrix]") {
    matrix<float> mat(3, 3, 1.234);

    SECTION("comparing matrices with the same dimensions and elements") {
        matrix<float> other(mat);

        REQUIRE(other == mat);
        REQUIRE_FALSE(other != mat);
    }

    SECTION("comparing matrices with the same dimensions but different elements") {
        matrix<float> other(3, 3, 4.321);

        REQUIRE(other != mat);
        REQUIRE_FALSE(other == mat);
    }

    SECTION("comparing matrices with different dimensions and different elements") {
        matrix<float> other(5, 3, 0.1);

        REQUIRE(other != mat);
        REQUIRE_FALSE(other == mat);
    }
}

#endif /* MATRIX_TEST_H_ */

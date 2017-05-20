/*
 * gameboard.cpp
 *
 *  Created on: May 13, 2017
 *      Author: feilan
 */

#include "gameboard.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <set>
#include <sstream>

static gameboard::shift_direction directions[] = {
    gameboard::shift_direction::up,
    gameboard::shift_direction::down,
    gameboard::shift_direction::left,
    gameboard::shift_direction::right
};

static int rand_int(int min, int max) {
    static bool ready = false;
    time_t t;

    if (!ready) {
        srand((unsigned) time(&t));
        ready = true;
    }

    return rand() % (max + 1 - min) + min;
}

// temp
static std::string matrix_to_string(const matrix<unsigned>& mat) {
    std::ostringstream osstr;
    for (std::size_t i = 0; i < mat.n_rows(); i++) {
        for (std::size_t j = 0; j < mat.n_cols(); j++) {
            osstr << mat.at(i, j);
        }
    }
    return osstr.str();
}

gameboard::gameboard(std::size_t width,
        std::size_t height,
        std::size_t n_colors,
        color_distribution cdist) :
    k_width(width),
    k_height(height),
    k_area(width * height),
    k_n_colors(n_colors > k_area ? k_area : n_colors),
    m_cur_state(state::initial),
    m_cdist(cdist),
    m_squares(height, width),
    m_initial_circles(m_squares)
{
    if (k_area % 2 != 0 && k_n_colors == 2) {
        throw std::invalid_argument("2 colors and odd area will lead to square-circle color overlap");
    } else if (k_area < 2) {
        throw std::invalid_argument("need area of at least 2");
    } else if (k_n_colors < 2) {
        throw std::invalid_argument("need at least 2 colors");
    }

    /*
     * determine the number of times each color is to be applied to a board grid
     */
    std::vector<unsigned> color_inventory;
    switch (m_cdist) {
        case color_distribution::uniform: {
            float ratio = k_area / k_n_colors;
            std::size_t mode_color_count = round(ratio);
            color_inventory = std::vector<unsigned>(k_n_colors, mode_color_count);

            if (mode_color_count > ratio) {
                for (std::size_t i = 0; i < k_n_colors * mode_color_count - k_area; i++) {
                    color_inventory[i]--;
                }
            } else {
                for (std::size_t i = 0; i < k_area - k_n_colors * mode_color_count; i++) {
                    color_inventory[i]++;
                }
            }

            break;
        }
        case color_distribution::random: {
            std::size_t rem_cells = k_area;
            std::size_t half = k_area / 2;
            std::size_t rem_colors;
            unsigned val;

            for (std::size_t i = 0; i < k_n_colors - 1; i++) {
                rem_colors = k_n_colors - i;
                val = rand_int(rem_cells / rem_colors, std::min(half, rem_cells - rem_colors + 1));

                rem_cells -= val;
                color_inventory.push_back(val);
            }

            color_inventory.push_back(rem_cells);
            break;
        }
        default:
            break;
    }

populate:
    std::vector<unsigned> square_color_inventory = color_inventory;
    std::vector<unsigned> circle_color_inventory = color_inventory;
    std::size_t rem_squares = k_area;
    std::size_t s, c;
    for (std::size_t x = 0; x < k_width; x++) {
        for (std::size_t y = 0; y < k_height; y++) {
            do {
                s = rand_int(0, k_n_colors - 1);
            } while (square_color_inventory[s] == 0);

            do {
                c = rand_int(0, k_n_colors - 1);
            } while (circle_color_inventory[c] == 0 || c == s);

            m_squares.at(y, x) = s;
            m_initial_circles.at(y, x) = c;

            square_color_inventory[s]--;
            circle_color_inventory[c]--;
            rem_squares--;

            // need a loop?
            for (std::size_t k = 0; k < k_n_colors; k++) {
                if (circle_color_inventory[k] > rem_squares - square_color_inventory[k]) {
                    goto populate;
                }
            }
        }
    }

    // next: ensure solvable

    reset();
}

gameboard::~gameboard() {
    // TODO Auto-generated destructor stub
}

// grid[0][0] = top left
//      x  y
//
// TODO what to skip:
// unsolved has no unique elements - skip that line
void gameboard::shift(shift_direction dir) {
    std::size_t num;
    std::size_t len;
    bool transpose;

    // no transpose
    if (dir == shift_direction::up || dir == shift_direction::down) {
        num = k_width;
        len = k_height;
        transpose = false;
    // transpose
    } else {
        num = k_height;
        len = k_width;
        transpose = true;
    }

    std::vector<std::size_t> unsolved;
    bool unsolved_has_dupes = true;
    unsigned temp;  // used for holding the first circle during the "trickling phase",
        // as well as for checking for unique unsolved current_circles
    for (std::size_t x = 0; x < num; x++) {
        for (std::size_t y = 0; y < len; y++) {
            if (m_current_circles.at(y, x, transpose)
                    != m_squares.at(y, x, transpose)) {
                if (unsolved.size() == 0) {
                    temp = m_current_circles.at(y, x, transpose);
                } else if (m_current_circles.at(y, x, transpose) != temp) {
                    unsolved_has_dupes = false;
                }

                unsolved.push_back(y);
            }
        }

        if (!unsolved_has_dupes && unsolved.size() > 1) {
            std::size_t k;

            if (dir == gameboard::shift_direction::up || dir == gameboard::shift_direction::left) {
                k = 0;
                temp = m_current_circles.at(unsolved[k], x, transpose);
                for ( ; k < unsolved.size() - 1; k++) {
                    m_current_circles.at(unsolved[k], x, transpose) =
                        m_current_circles.at(unsolved[k + 1], x, transpose);
                }
            } else {
                k = unsolved.size() - 1;
                temp = m_current_circles.at(unsolved[k], x, transpose);
                for ( ; k > 0; k--) {
                    m_current_circles.at(unsolved[k], x, transpose) =
                        m_current_circles.at(unsolved[k - 1], x, transpose);
                }
            }

            m_current_circles.at(unsolved[k], x, transpose) = temp;
        }

        unsolved.clear();
    }
}

void gameboard::reset() {
    m_current_circles = m_initial_circles;
}

const matrix<unsigned>& gameboard::squares() const {
    return m_squares;
}

const matrix<unsigned>& gameboard::initial_circles() const {
    return m_initial_circles;
}

const matrix<unsigned>& gameboard::current_circles() const {
    return m_current_circles;
}

std::vector<gameboard::shift_direction> gameboard::solution() {
    // temp stats
    unsigned long long num_iterations = 0;
    unsigned long long num_nodes_visited = 1;
    unsigned long long num_paths_considered = 0;

    bool solved = false;
    std::queue<std::vector<gameboard::shift_direction>> next;
    std::set<std::string> visited;
    std::vector<gameboard::shift_direction> path;
    std::string mat;
    matrix<unsigned> temp;

    visited.insert(matrix_to_string(m_initial_circles));

    do {
        // if continuing from a previous path, we first need to move the gameboard
        // into its state after that path
        if (!next.empty()) {
            path = next.front();
            for (gameboard::shift_direction dir : path) {
                shift(dir);
            }
            next.pop();
        }

        if (m_current_circles == m_squares) {
            solved = true;
        } else {
            temp = m_current_circles;
            for (gameboard::shift_direction dir : directions) {
                m_current_circles = temp;
                shift(dir);
                mat = matrix_to_string(m_current_circles);

                if (visited.count(mat) == 0) {
                    path.push_back(dir);
                    next.push(path);
                    visited.insert(mat);

                    num_paths_considered++;
                }
            }
        }

        m_current_circles = m_initial_circles;

        num_iterations++;
    } while (!solved && !next.empty());

    // temp
    num_nodes_visited = visited.size();
    std::cout << num_iterations << " iterations, " << num_nodes_visited << " nodes visited" << std::endl;

    return solved ? path : std::vector<gameboard::shift_direction>();
}

std::ostream& operator<<(std::ostream& os, const gameboard& g) {
    for (std::size_t j = 0; j < g.k_height; j++) {
        for (std::size_t i = 0; i < g.k_width; i++) {
            std::cout << g.m_squares.at(j, i) << '|' << g.m_current_circles.at(j, i) << ' ';
        }
        std::cout << std::endl << std::endl;
    }

    return os;
}

solutions::solutions(const gameboard& gameboard, unsigned depth) {
    // TODO Auto-generated constructor stub
}

solutions::~solutions() {
    // TODO Auto-generated destructor stub
}

const float solutions::difficulty() const {
    return m_difficulty;
}

const unsigned solutions::n_solns() const {
    return m_n_solns;
}

const std::vector<unsigned>& solutions::lengths() const {
    return m_lengths;
}

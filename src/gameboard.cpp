#include "gameboard.h"

// TODO remove unneeded includes
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
    m_width(width),
    m_height(height),
    m_area(width * height),
    m_n_colors(n_colors > m_area ? m_area : n_colors),
    m_cur_state(state::initial),
    m_cdist(cdist),
    m_squares(height, width),
    m_init_circles(m_squares)
{
    if (m_area % 2 != 0 && m_n_colors == 2) {
        throw std::invalid_argument("2 colors and odd area will lead to square-circle color overlap");
    } else if (m_area < 2) {
        throw std::invalid_argument("need area of at least 2");
    } else if (m_n_colors < 2) {
        throw std::invalid_argument("need at least 2 colors");
    }

    /*
     * determine the number of times each color is to be applied to a board grid
     */
    std::vector<unsigned> color_inventory;
    switch (m_cdist) {
        case color_distribution::uniform: {
            float ratio = m_area / m_n_colors;
            std::size_t mode_color_count = round(ratio);
            color_inventory = std::vector<unsigned>(m_n_colors, mode_color_count);

            if (mode_color_count > ratio) {
                for (std::size_t i = 0; i < m_n_colors * mode_color_count - m_area; i++) {
                    color_inventory[i]--;
                }
            } else {
                for (std::size_t i = 0; i < m_area - m_n_colors * mode_color_count; i++) {
                    color_inventory[i]++;
                }
            }

            break;
        }
        case color_distribution::random: {
            std::size_t rem_cells = m_area;
            std::size_t half = m_area / 2;
            std::size_t rem_colors;
            unsigned val;

            for (std::size_t i = 0; i < m_n_colors - 1; i++) {
                rem_colors = m_n_colors - i;
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
    std::size_t rem_squares = m_area;
    std::size_t s, c;
    for (std::size_t x = 0; x < m_width; x++) {
        for (std::size_t y = 0; y < m_height; y++) {
            do {
                s = rand_int(0, m_n_colors - 1);
            } while (square_color_inventory[s] == 0);

            do {
                c = rand_int(0, m_n_colors - 1);
            } while (circle_color_inventory[c] == 0 || c == s);

            m_squares.at(y, x) = s;
            m_init_circles.at(y, x) = c;

            square_color_inventory[s]--;
            circle_color_inventory[c]--;
            rem_squares--;

            // need a loop?
            for (std::size_t k = 0; k < m_n_colors; k++) {
                if (circle_color_inventory[k] > rem_squares - square_color_inventory[k]) {
                    goto populate;
                }
            }
        }
    }

    reset();

    if (!is_solvable()) {
        goto populate;
    }
}

gameboard::gameboard(const gameboard& other) :
    m_width(other.m_width),
    m_height(other.m_height),
    m_area(other.m_area),
    m_n_colors(other.m_n_colors),
    m_cur_state(state::initial),
    m_cdist(other.m_cdist),
    m_squares(other.m_squares),
    m_init_circles(other.m_init_circles)
{
    m_cur_circles = m_init_circles;
}

gameboard& gameboard::operator=(const gameboard& other) {
    if (&other != this) {
        m_width = other.m_width;
        m_height= other.m_height;
        m_area = other.m_area;
        m_n_colors = other.m_n_colors;
        m_cdist = other.m_cdist;
        m_squares = other.m_squares;
        m_init_circles = other.m_init_circles;

        m_cur_state = state::initial;
        m_cur_circles = other.m_init_circles;
    }

    return *this;
}

bool gameboard::operator==(const gameboard& other) const {
    return initial_circles() == other.initial_circles() && squares() == other.squares();
}

std::size_t gameboard::width() const {
    return m_width;
}

std::size_t gameboard::height() const {
    return m_height;
}

std::size_t gameboard::area() const {
    return m_area;
}

std::size_t gameboard::n_colors() const {
    return m_n_colors;
}

gameboard::state gameboard::current_state() const {
    return m_cur_state;
}

gameboard::color_distribution gameboard::color_dist() const {
    return m_cdist;
}

const matrix<unsigned>& gameboard::squares() const {
    return m_squares;
}

const matrix<unsigned>& gameboard::initial_circles() const {
    return m_init_circles;
}

const matrix<unsigned>& gameboard::current_circles() const {
    return m_cur_circles;
}

void gameboard::shift(shift_direction dir) {
    std::size_t num;
    std::size_t len;
    bool transpose;

    // no transpose
    if (dir == shift_direction::up || dir == shift_direction::down) {
        num = m_width;
        len = m_height;
        transpose = false;
    // transpose
    } else {
        num = m_height;
        len = m_width;
        transpose = true;
    }

    std::vector<std::size_t> unsolved;
    bool unsolved_has_dupes = true;
    unsigned temp;  // used for holding the first circle during the "trickling phase",
        // as well as for checking for unique unsolved current_circles
    for (std::size_t x = 0; x < num; x++) {
        for (std::size_t y = 0; y < len; y++) {
            if (m_cur_circles.at(y, x, transpose)
                    != m_squares.at(y, x, transpose)) {
                if (unsolved.size() == 0) {
                    temp = m_cur_circles.at(y, x, transpose);
                } else if (m_cur_circles.at(y, x, transpose) != temp) {
                    unsolved_has_dupes = false;
                }

                unsolved.push_back(y);
            }
        }

        if (!unsolved_has_dupes && unsolved.size() > 1) {
            std::size_t k;

            if (dir == gameboard::shift_direction::up || dir == gameboard::shift_direction::left) {
                k = 0;
                temp = m_cur_circles.at(unsolved[k], x, transpose);
                for ( ; k < unsolved.size() - 1; k++) {
                    m_cur_circles.at(unsolved[k], x, transpose) =
                        m_cur_circles.at(unsolved[k + 1], x, transpose);
                }
            } else {
                k = unsolved.size() - 1;
                temp = m_cur_circles.at(unsolved[k], x, transpose);
                for ( ; k > 0; k--) {
                    m_cur_circles.at(unsolved[k], x, transpose) =
                        m_cur_circles.at(unsolved[k - 1], x, transpose);
                }
            }

            m_cur_circles.at(unsolved[k], x, transpose) = temp;
        }

        unsolved.clear();
    }
}

void gameboard::reset() {
    m_cur_circles = m_init_circles;
}

bool gameboard::is_solvable() {
    bool solved = false;
    std::queue<std::vector<gameboard::shift_direction>> next;
    std::set<std::string> visited;
    std::vector<gameboard::shift_direction> path;
    std::string mat;
    matrix<unsigned> temp;

    visited.insert(matrix_to_string(m_init_circles));

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

        if (m_cur_circles == m_squares) {
            solved = true;
        } else {
            temp = m_cur_circles;
            for (gameboard::shift_direction dir : directions) {
                m_cur_circles = temp;
                shift(dir);
                mat = matrix_to_string(m_cur_circles);

                if (visited.count(mat) == 0) {
                    path.push_back(dir);
                    next.push(path);
                    visited.insert(mat);
                }
            }
        }

        m_cur_circles = m_init_circles;
    } while (!solved && !next.empty());

    return solved;
}

solution_set::solution_set(gameboard g) :
    m_difficulty(0),
    m_n_solns(0)
{
    std::queue<std::vector<gameboard::shift_direction>> next;
    std::set<std::string> visited;
    std::vector<gameboard::shift_direction> path;
    std::string mat;
    matrix<unsigned> temp;

    visited.insert(matrix_to_string(g.m_init_circles));

    do {
        // if continuing from a previous path, we first need to move the gameboard
        // into its state after that path
        if (!next.empty()) {
            path = next.front();
            for (gameboard::shift_direction dir : path) {
                g.shift(dir);
            }
            next.pop();
        }

        if (g.m_cur_circles == g.m_squares) {
            m_data.push_back(path);
        } else {
            temp = g.m_cur_circles;
            for (gameboard::shift_direction dir : directions) {
                g.m_cur_circles = temp;
                g.shift(dir);
                mat = matrix_to_string(g.m_cur_circles);

                if (visited.count(mat) == 0) {
                    path.push_back(dir);
                    next.push(path);
                    visited.insert(mat);
                }
            }
        }

        g.m_cur_circles = g.m_init_circles;
    } while (!next.empty());

    m_n_solns = m_data.size();

    for (solution& sol : m_data) {
        m_difficulty += pow(0.1, sol.size() - 1);
    }
    m_difficulty = visited.size() / m_difficulty;
}

const float solution_set::difficulty() const {
    return m_difficulty;
}

const std::size_t solution_set::n_solns() const {
    return m_n_solns;
}

const solution_set::solution& solution_set::operator[](std::size_t n) const {
    return m_data[n];
}

const std::vector<solution_set::solution> solution_set::solutions_with_length(std::size_t n) const {
    std::vector<solution> solns;
    for (const solution& sol : m_data) {
        if (m_data.size() == n) {
            solns.push_back(sol);
        }
    }
    return solns;
}

const solution_set::solution& solution_set::longest_solution() const {
    std::size_t longest = 0;

    for (std::size_t i = 1; i < m_n_solns; i++) {
        if (m_data[i].size() > m_data[longest].size()) {
            longest = i;
        }
    }

    return m_data[longest];
}

const solution_set::solution& solution_set::shortest_solution() const {
    std::size_t shortest = 0;

    for (std::size_t i = 1; i < m_n_solns; i++) {
        if (m_data[i].size() < m_data[shortest].size()) {
            shortest = i;
        }
    }

    return m_data[shortest];
}

std::ostream& operator<<(std::ostream& os, const gameboard& g) {
    for (std::size_t j = 0; j < g.height(); j++) {
        for (std::size_t i = 0; i < g.width(); i++) {
            std::cout << g.squares().at(j, i) << '|' << g.current_circles().at(j, i) << ' ';
        }
        std::cout << std::endl << std::endl;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const solution_set::solution& s) {
    for (gameboard::shift_direction d : s) {
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

    return os;
}

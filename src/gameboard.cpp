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

static int rand_int(int min, int max) {
    static bool ready = false;
    time_t t;

    if (!ready) {
        srand((unsigned) time(&t));
        ready = true;
    }

    return rand() % (max + 1 - min) + min;
}

void gameboard::print_initial() const {
	for (int j = 0; j < k_height; j++) {
		for (int i = 0; i < k_width; i++) {
			std::cout << m_squares[i][j] << '|' << m_initial_circles[i][j] << ' ';
		}
		std::cout << std::endl << std::endl;
	}
}

void gameboard::print_current() const {
	for (int j = 0; j < k_height; j++) {
		for (int i = 0; i < k_width; i++) {
			std::cout << m_squares[i][j] << '|' << m_current_circles[i][j] << ' ';
		}
		std::cout << std::endl << std::endl;
	}
}

gameboard::gameboard(std::size_t height,
		std::size_t width,
		std::size_t n_colors,
		color_distribution cdist) :
	k_height(height),
	k_width(width),
	k_area(height * width),
	k_n_colors(n_colors > k_area ? k_area : n_colors),
	m_cur_state(state::initial),
	m_cdist(cdist)
{
	if (k_area % 2 != 0 && k_n_colors == 2) {
		throw std::invalid_argument("2 colors and odd area will lead to square-circle color overlap");
	} else if (k_area < 2) {
		throw std::invalid_argument("need area of at least 2");
	} else if (k_n_colors < 2) {
		throw std::invalid_argument("need at least 2 colors");
	}

	m_squares = std::vector<std::vector<unsigned>>(width, std::vector<unsigned>(height));
	m_initial_circles = m_squares;

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
	for (std::size_t i = 0; i < k_width; i++) {
		for (std::size_t j = 0; j < k_height; j++) {
			do {
				s = rand_int(0, k_n_colors);
			} while (square_color_inventory[s] == 0);

			do {
				c = rand_int(0, k_n_colors);
			} while (circle_color_inventory[c] == 0 || c == s);

			m_squares[i][j] = s;
			m_initial_circles[i][j] = c;

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
//      c  r
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

	std::size_t cur;
	std::size_t prev;
	std::vector<std::size_t> unsolved;
	bool unsolved_has_dupes = false;
	unsigned temp;	// used for holding the first circle during the "trickling phase",
		// as well as for checking for unique unsolved circles
	for (std::size_t i = 0; i < num; i++) {
		for (std::size_t j = 0; j < len; j++) {
			if (at(cell_object::current_circle, i, j, transpose)
					!= at(cell_object::square, i, j, transpose)) {
				if (unsolved.size() == 0) {
					temp = at(cell_object::current_circle, i, j, transpose);
				} else if (at(cell_object::current_circle, i, j, transpose) != temp) {
					unsolved_has_dupes = true;
				}

				unsolved.push_back(j);
			}
		}

		if (unsolved_has_dupes && unsolved.size() > 1) {
			std::size_t k = 0;
			temp = at(cell_object::current_circle, i, unsolved[k], transpose);
			for ( ; k < unsolved.size() - 1; k++) {
				at(cell_object::current_circle, i, unsolved[k], transpose) =
					at(cell_object::current_circle, i, unsolved[k + 1], transpose);
			}
			at(cell_object::current_circle, i, unsolved[k], transpose) = temp;
		} else {
			std::cout << "skip" << std::endl;
		}

		unsolved.clear();
	}
}

void gameboard::reset() {
	m_current_circles = m_initial_circles;
}

const std::vector<std::vector<unsigned>>& gameboard::squares() const {
	return m_squares;
}

const std::vector<std::vector<unsigned>>& gameboard::circles() const {
	return m_initial_circles;
}

unsigned& gameboard::at(cell_object obj, std::size_t i,
		std::size_t j, bool transpose)
{
	std::vector<std::vector<unsigned>>* objects;

	switch (obj) {
	case cell_object::square:
		objects = &m_squares;
		break;
	case cell_object::initial_circle:
		objects = &m_initial_circles;
		break;
	case cell_object::current_circle:
		objects = &m_current_circles;
		break;
	default:
		break;
	}

	return transpose ? objects->at(j).at(i) : objects->at(i).at(j);
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

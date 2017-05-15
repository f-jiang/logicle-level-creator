/*
 * gameboard.cpp
 *
 *  Created on: May 13, 2017
 *      Author: feilan
 */

#include "gameboard.h"

#include <cmath>
#include <algorithm>
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

gameboard::gameboard(std::size_t height,
		std::size_t width,
		std::size_t n_colors,
		color_distribution cdist) :
	k_height(height),
	k_width(width),
	k_area(height * width),
	k_n_colors(n_colors > k_area ? k_area : n_colors),
	m_cur_state(state::blank),
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
	m_circles = m_squares;

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
			m_circles[i][j] = c;

			square_color_inventory[s]--;
			circle_color_inventory[c]--;
			rem_squares--;

			for (std::size_t k = 0; k < k_n_colors; k++) {
				if (circle_color_inventory[k] > rem_squares - square_color_inventory[k]) {
					goto populate;
				}
			}
		}
	}

	// next: ensure solvable
}

gameboard::~gameboard() {
	// TODO Auto-generated destructor stub
}

void gameboard::shift(shift_direction dir) {

}

void gameboard::reset() {

}

const std::vector<std::vector<unsigned>>& gameboard::squares() const {
	return m_squares;
}

const std::vector<std::vector<unsigned>>& gameboard::circles() const {
	return m_circles;
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

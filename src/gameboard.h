/*
 * gameboard.h
 *
 *  Created on: May 13, 2017
 *      Author: feilan
 */

#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

#include <cstddef>
#include <vector>

#include "matrix.h"

class gameboard {
public:
    const std::size_t k_width;
    const std::size_t k_height;
    const std::size_t k_area;
    const std::size_t k_n_colors;

    enum class state {
        stuck,
        solved,
        unsolved,
        initial
    };

    enum class shift_direction {
        up,
        down,
        left,
        right
    };

    enum class color_distribution {
        uniform,
        random,
//      non_repeating
    };

    gameboard(std::size_t, std::size_t, std::size_t, color_distribution cpop = color_distribution::uniform);
    virtual ~gameboard();

    const matrix<unsigned>& squares() const;
    const matrix<unsigned>& initial_circles() const;
    const matrix<unsigned>& current_circles() const;

    // temp
    void print_initial() const;
    void print_current() const;

    void shift(shift_direction);
    void reset();
private:
    state m_cur_state;
    color_distribution m_cdist;
    matrix<unsigned> m_squares;
    matrix<unsigned> m_initial_circles;
    matrix<unsigned> m_current_circles;
};

class solutions {
public:
    solutions(const gameboard&, unsigned depth = -1);
    virtual ~solutions();

    const float difficulty() const;
    const unsigned n_solns() const;

    const std::vector<gameboard::shift_direction>& operator[](std::size_t n) const {
        return m_data[n];
    }

    const std::vector<unsigned>& lengths() const;
private:
    float m_difficulty;
    std::size_t m_n_solns;

    std::vector<std::vector<gameboard::shift_direction>> m_data;
    std::vector<unsigned> m_lengths;
};

#endif /* GAMEBOARD_H_ */

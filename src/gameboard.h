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

    // temp - make a matrix class instead
    enum class cell_object {
        square,
        initial_circle,
        current_circle
    };

    gameboard(std::size_t, std::size_t, std::size_t, color_distribution cpop = color_distribution::uniform);
    virtual ~gameboard();

    const std::vector<std::vector<unsigned>>& squares() const;
    const std::vector<std::vector<unsigned>>& circles() const;

    // temp
    void print_initial() const;
    void print_current() const;

    void shift(shift_direction);
    void reset();
private:
    state m_cur_state;
    color_distribution m_cdist;
    std::vector<std::vector<unsigned>> m_squares;
    std::vector<std::vector<unsigned>> m_initial_circles;
    std::vector<std::vector<unsigned>> m_current_circles;

    // temp - make a matrix class instead
    unsigned& at(cell_object, std::size_t, std::size_t, bool transpose = false);
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

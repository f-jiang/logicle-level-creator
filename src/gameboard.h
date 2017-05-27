#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

#include <cstddef>
#include <vector>
#include <ostream>

#include "matrix.h"

// TODO actually set state and use reset() properly
// TODO use a shortest-path algo for is_solvable()
// TODO non-repeating color dist
// TODO save memory using reference params where possible
class gameboard {
friend class solution_set;
public:
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

    gameboard(std::size_t, std::size_t, std::size_t, color_distribution = color_distribution::uniform);
    gameboard(const gameboard&);

    gameboard& operator=(const gameboard&);
    bool operator==(const gameboard&) const;

    std::size_t width() const;
    std::size_t height() const;
    std::size_t area() const;
    std::size_t n_colors() const;
    state current_state() const;
    color_distribution color_dist() const;
    const matrix<unsigned>& squares() const;
    const matrix<unsigned>& initial_circles() const;
    const matrix<unsigned>& current_circles() const;

    void shift(shift_direction);
    void reset();

private:
    std::size_t m_width;
    std::size_t m_height;
    std::size_t m_area;
    std::size_t m_n_colors;
    state m_cur_state;
    color_distribution m_cdist;
    matrix<unsigned> m_squares;
    matrix<unsigned> m_init_circles;
    matrix<unsigned> m_cur_circles;

    bool is_solvable();
};

// TODO implement a runtime limit
// TODO save memory using reference params where possible
class solution_set {
public:
    typedef std::vector<gameboard::shift_direction> solution;

    solution_set(gameboard);

    const float difficulty() const;
    const std::size_t n_solns() const;

    const solution& operator[](std::size_t) const;

    const std::vector<solution> solutions_with_length(std::size_t) const;
    const solution& longest_solution() const;
    const solution& shortest_solution() const;
private:
    float m_difficulty;
    std::size_t m_n_solns;

    std::vector<solution> m_data;
};

std::ostream& operator<<(std::ostream&, const gameboard&);
std::ostream& operator<<(std::ostream&, const solution_set::solution&);

#endif /* GAMEBOARD_H_ */

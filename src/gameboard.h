#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

// TODO remove unneeded includes
#include <cstddef>
#include <vector>
#include <ostream>

#include "matrix.h"

// TODO actually set state and use reset() properly
// TODO use a shortest-path algo for is_solvable()
// TODO non-repeating color dist
// TODO save memory using reference params where possible
// TODO remove matrix_to_string() if it isn't needed
class gameboard {
friend class solution_set;
public:

    /*
     * The state of the gameboard according to the current circles:
     * - stuck: TBD
     * - solved: the circle and square colors match for every cell in the gameboard
     * - unsolved: TBD
     * - initial: gameboard is in its starting position, i.e. current circles
     *   match initial circles
     */
    enum class state {
        stuck,
        solved,
        unsolved,
        initial
    };

    /*
     * Represents the four possible types of moves one can make on an unsolved gameboard.
     */
    enum class shift_direction {
        up,
        down,
        left,
        right
    };

    /*
     *
     */
    enum class color_distribution {
        uniform,
        random,
//      non_repeating
    };

    static const color_distribution DEFAULT_COLOR_DISTRIBUTION = color_distribution::uniform;

    /*
     * Constructs an empty gameboard. The gameboard is guaranteed to have a solution.
     */
    // TODO appropriate for default ctor to generate a useless object, merely to
    // allow declaration of a solution_set object member inside the ctor body?
    gameboard() = default;

    /*
     * Constructs a gameboard with the specified properties. Throws an exception if the
     * given set of properties is impossible. The gameboard is guaranteed to have a solution.
     */
    gameboard(std::size_t, std::size_t, std::size_t, color_distribution = DEFAULT_COLOR_DISTRIBUTION);

    gameboard(const gameboard&);
    gameboard& operator=(const gameboard&);

    /*
     * Compares two gameboard objects, returning true if both gameboards have the same
     * dimensions, number of colors, square configuration, and circle starting configuration.
     */
    bool operator==(const gameboard&) const;

    /*
     * Compares two gameboard objects, returning false if the gameboards differ in
     * dimensions, number of colors, square configuration, and/or circle starting configuration.
     */
    bool operator!=(const gameboard&) const;

    /*
     * Accessors
     */
    std::size_t height() const;
    std::size_t width() const;
    std::size_t area() const;
    std::size_t n_colors() const;
    state current_state() const;
    color_distribution color_dist() const;
    const matrix<unsigned>& squares() const;
    const matrix<unsigned>& initial_circles() const;
    const matrix<unsigned>& current_circles() const;

    /*
     * Makes a move on the gameboard by shifting the current circles in the direction specified.
     * Circles moving beyond the gameboard edge will reappear on the opposite side. If a circle
     * is on a square of the same color, then the circle will not be shifted and the cell is considered
     * solved. Any adjacent, unsolved circles will skip over this cell and land in the nearest unsolved
     * cell in the specified shift direction.
     */
    void shift(shift_direction);

    /*
     * Sets the current circles back to the initial circles, i.e. the starting configuration.
     */
    void reset();

private:
    std::size_t m_height;
    std::size_t m_width;
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

    /*
     * Constructs an empty solution set.
     */
    // TODO appropriate for default ctor to generate a useless object, merely to
    // allow declaration of a solution_set object member inside the ctor body?
    solution_set() = default;

    /*
     * Constructs a solution set for the given gameboard.
     */
    solution_set(gameboard);

    /*
     * Accessors
     */
    const float difficulty() const;
    const std::size_t n_solutions() const;

    /*
     * Access a specific solution.
     */
    const solution& operator[](std::size_t) const;

    /*
     * Returns a vector of all solutions with the specified length. If
     * there are no such solutions, the returned vector will be empty.
     */
    const std::vector<solution> solutions_with_length(std::size_t) const;

    /*
     * Return the longest and shortest solutions. If there is only one solution,
     * both functions will return the same thing.
     */
    const solution& longest_solution() const;
    const solution& shortest_solution() const;
private:
    float m_difficulty;
    std::size_t m_n_solns;

    std::vector<solution> m_data;
};

/*
 * Inserts a gameboard or solution into the specified output stream.
 */
std::ostream& operator<<(std::ostream&, const gameboard&);
std::ostream& operator<<(std::ostream&, const solution_set::solution&);

#endif /* GAMEBOARD_H_ */

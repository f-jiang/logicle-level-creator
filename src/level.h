#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "../lib/json.hpp"
#include "json_serializable.h"
#include "gameboard.h"

class level : public json_serializable {
public:
    // TODO initializer_list, or param pack instead of vector?
    // TODO ctors from pre-existing gameboard: exceptions when too many, too few, 
    // or duplicate colors
    /*
     * Constructs a level, generating a new gameboard with the specified properties,
     * then finding and storing its solutions.
     */
    level(std::size_t,
          std::size_t,
          std::vector<unsigned>,
          gameboard::color_distribution = gameboard::color_distribution::uniform);

    /*
     * Constructs a level from a pre-existing gameboard and set of solutions,
     * then applies the given colors. No validation of the solutions is performed.
     */
    level(const gameboard&, const solution_set&, std::vector<unsigned>);

    /*
     * Constructs a level from a pre-existing gameboard, then applies the given colors
     * and finds and stores the level's solutions.
     */
    level(const gameboard&, std::vector<unsigned>);

    /*
     * Returns a json object representing the level so that it can be written to a file.
     */
    nlohmann::json as_json() const;

    /*
     * Accessors for key level properties.
     */
    std::size_t gameboard_width() const;
    std::size_t gameboard_height() const;
    float difficulty() const;
    std::size_t n_colors() const;
    std::size_t n_solutions() const;
private:
    gameboard m_gameboard;
    std::vector<unsigned> m_colors;
    solution_set m_solns;
};

#endif /* LEVEL_H_ */

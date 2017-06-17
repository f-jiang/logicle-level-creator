#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "json.hpp"
#include "json_serializable.h"
#include "gameboard.h"

class level : public json_serializable {
public:
    // TODO initializer_list instead of vector?
    // TODO exception when duplicate colours
    level(std::size_t,
          std::size_t,
          std::vector<unsigned>,
          gameboard::color_distribution = gameboard::color_distribution::uniform);
    level(const gameboard&, const solution_set&, std::vector<unsigned>);
    level(const gameboard&, std::vector<unsigned>);

    nlohmann::json as_json() const;

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

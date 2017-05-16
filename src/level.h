/*
 * level.h
 *
 *  Created on: May 13, 2017
 *      Author: feilan
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "gameboard.h"

class level {
public:
    level(std::size_t,
            std::size_t,
            const std::vector<unsigned>&,
            gameboard::color_distribution cpop = gameboard::color_distribution::uniform);
    level(const gameboard&, const std::vector<unsigned>&);
    // construct from json
    virtual ~level();

private:
    gameboard m_gameboard;
    std::vector<unsigned> m_colors;
    solutions m_solutions;
};

#endif /* LEVEL_H_ */

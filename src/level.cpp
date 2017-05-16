/*
 * level.cpp
 *
 *  Created on: May 13, 2017
 *      Author: feilan
 */

#include "level.h"

level::level(std::size_t height,
        std::size_t width,
        const std::vector<unsigned>& colors,
        gameboard::color_distribution cpop) :
    m_gameboard(height, width, colors.size(), cpop),
    m_colors(colors),
    m_solutions(m_gameboard)    // m_gameboard comes before m_solutions in level.h, so can be used here
{

}

level::level(const gameboard& gameboard,
        const std::vector<unsigned>& colors) :
    m_gameboard(gameboard),
    m_colors(colors),
    m_solutions(gameboard)
{
    // TODO Auto-generated constructor stub

}

level::~level() {
    // TODO Auto-generated destructor stub
}

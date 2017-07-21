#include "level.h"

#include <stdexcept>

level::level(std::size_t height,
        std::size_t width,
        std::vector<unsigned> colors,
        gameboard::color_distribution cpop) :
    m_gameboard(height, width, colors.size(), cpop),
    m_colors(colors),
    m_solns(m_gameboard)
{ }

level::level(std::size_t height,
        std::size_t width,
        std::vector<unsigned> colors) :
    m_gameboard(height, width, colors.size()),
    m_colors(colors),
    m_solns(m_gameboard)
{ }

level::level(const gameboard& gameboard,
        const solution_set& solutions,
        std::vector<unsigned> colors)
{
    if (colors.size() < gameboard.n_colors()) {
        throw std::invalid_argument("level: not enough colors provided for gameboard");
    }

    m_gameboard = gameboard;
    m_colors = colors;
    m_solns = solutions;
}

level::level(const gameboard& gameboard,
        std::vector<unsigned> colors)
{
    if (colors.size() < gameboard.n_colors()) {
        throw std::invalid_argument("level: not enough colors provided for gameboard");
    }

    m_gameboard = gameboard;
    m_colors = colors;
    m_solns = solution_set(gameboard);
}

nlohmann::json level::as_json() const {
    std::vector<std::vector<unsigned>> circles_json(
        m_gameboard.width(),
        std::vector<unsigned>(m_gameboard.height())
    );
    std::vector<std::vector<unsigned>> squares_json = circles_json;

    for (std::size_t i = 0; i < m_gameboard.height(); i++) {
        for (std::size_t j = 0; j < m_gameboard.width(); j++) {
            circles_json[j][i] = m_gameboard.initial_circles().at(i, j);
            squares_json[j][i] = m_gameboard.squares().at(i, j);
        }
    }

    nlohmann::json j = {
        { "area", m_gameboard.area() },
        { "heightSquares", m_gameboard.height() },
        { "widthSquares", m_gameboard.width() },
        { "difficulty", m_solns.difficulty() },
        { "longestSolution", m_solns.longest_solution().size() },
        { "shortestSolution", m_solns.shortest_solution().size() },
        { "colors", m_colors },
        { "numColors", m_colors.size() },
        { "circles", circles_json },
        { "squares", squares_json }
    };

    return j;
}

bool level::operator==(const level& other) const {
    return m_gameboard == other.m_gameboard;
}

bool level::operator!=(const level& other) const {
    return !(m_gameboard == other.m_gameboard);
}

std::size_t level::gameboard_height() const {
    return m_gameboard.height();
}

std::size_t level::gameboard_width() const {
    return m_gameboard.width();
}

float level::difficulty() const {
    return m_solns.difficulty();
}

std::size_t level::n_colors() const {
    return m_gameboard.n_colors();
}

std::size_t level::n_solutions() const {
    return m_solns.n_solutions();
}

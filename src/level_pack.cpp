#include "level_pack.h"

#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <utility>
#include <iterator>
#include <tuple>

level_pack::category::group_properties::group_properties() :
    height(0),
    width(0),
    n_levels(level_pack::category::group_properties::DEFAULT_N_LEVELS),
    color_dist(gameboard::DEFAULT_COLOR_DISTRIBUTION)
{ }

level_pack::category::group_properties::group_properties(size_t h,
        size_t w,
        std::vector<unsigned> colors,
        std::size_t n_lvls,
        gameboard::color_distribution cdist) :
    height(h),
    width(w),
    colors(colors),
    n_levels(n_lvls),
    color_dist(cdist)
{ }

level_pack::level_pack(std::string name) :
        m_name(name)
{ }

std::list<level_pack::category>::iterator level_pack::find_category(std::string name) {
    return std::find_if(m_data.begin(), m_data.end(),
        [&name] (category& cat) { return cat.name == name; }
    );
}

void level_pack::add_group(level_pack::category& cat, level_pack::category::group_properties group) {
    std::size_t old_size = cat.levels.size();
    std::size_t n_levels_to_add = group.n_levels;
    std::vector<level>::iterator group_begin;
    std::vector<level>::iterator erase_begin;
    std::ostringstream a_str;
    std::ostringstream b_str;

    do {
        for (std::size_t i = 0; i < n_levels_to_add; i++) {
            cat.levels.emplace_back(group.height, group.width, group.colors, group.color_dist);
        }

        group_begin = cat.levels.begin() + old_size;

        std::sort(group_begin, cat.levels.end(),
            [&a_str, &b_str] (const level& a, const level& b) {
                a_str.str(std::string());
                b_str.str(std::string());

                a_str << a.m_gameboard;
                b_str << b.m_gameboard;
                return a_str.str() < b_str.str();
            }
        );

        erase_begin = std::unique(group_begin, cat.levels.end());

        n_levels_to_add = std::distance(erase_begin, cat.levels.end());
        cat.levels.erase(erase_begin, cat.levels.end());
    } while (n_levels_to_add != 0);

    std::sort(group_begin, cat.levels.end(),
        [] (const level& a, const level& b) {
            return a.difficulty() < b.difficulty();
        }
    );
}

void level_pack::add_category(std::string name, level_pack::category::group_properties group) {
    if (find_category(name) != m_data.end()) {
        std::ostringstream osstr;
        osstr << "level_pack: there already exists a category with name ";
        osstr << name;
        throw std::invalid_argument(osstr.str());
    }

    std::list<category>::iterator cat = m_data.insert(m_data.end(), { name });
    add_group(*cat, group);
}

void level_pack::add_category(std::string name, std::vector<level_pack::category::group_properties> groups) {
    if (find_category(name) != m_data.end()) {
        std::ostringstream osstr;
        osstr << "level_pack: there already exists a category with name ";
        osstr << name;
        throw std::invalid_argument(osstr.str());
    }

    std::list<category>::iterator cat = m_data.insert(m_data.end(), { name });

    for (category::group_properties& g : groups) {
        add_group(*cat, g);
    }
}

void level_pack::remove_category(std::string name) {
    std::list<category>::iterator it = find_category(name);
    std::ostringstream osstr;

    if (m_data.size() == 0) {
        throw std::invalid_argument("level_pack: nothing to remove from empty level_pack");
    }

    if (it == m_data.end()) {    // if no category with |name| was found
        osstr << "level_pack: couldn't find a level category with name ";
        osstr << name;
        throw std::invalid_argument(osstr.str());
    }

    m_data.erase(it);
}

void level_pack::rename_category(std::string cur_name, std::string new_name) {
    std::list<category>::iterator it = find_category(cur_name);
    std::ostringstream osstr;

    if (m_data.size() == 0) {
        throw std::invalid_argument("level_pack: nothing to rename in empty level_pack");
    }

    if (it == m_data.end()) {    // if no category with |cur_name| was found
        osstr << "level_pack: couldn't find a level category with name ";
        osstr << cur_name;
        throw std::invalid_argument(osstr.str());
    }

    if (find_category(new_name) != m_data.end()) {   // there already exists a category called |new_name|
        osstr << "level_pack: there already exists a category with name ";
        osstr << new_name;
        throw std::invalid_argument(osstr.str());
    }

    it->name = new_name;
}

void level_pack::reorder_category(std::string name, std::size_t new_pos) {
    std::list<category>::iterator it = find_category(name);
    std::ostringstream osstr;

    if (m_data.size() == 0) {
        throw std::invalid_argument("level_pack: nothing to reorder in empty level_pack");
    }

    if (it == m_data.end()) {    // if no category with |name| was found
        osstr << "level_pack: couldn't find a level category with name ";
        osstr << name;
        throw std::invalid_argument(osstr.str());
    }

    if (new_pos >= m_data.size()) {
        osstr << "level_pack: level category ";
        osstr << name;
        osstr << " couldn't be relocated to out-of-bounds index ";
        osstr << new_pos;
        throw std::out_of_range(osstr.str());
    }

    std::list<category>::iterator dest = m_data.begin();
    std::advance(dest, new_pos + 1);
    m_data.splice(dest, m_data, it);
}

const std::list<level_pack::category>& level_pack::data() const {
    return m_data;
}

nlohmann::json level_pack::as_json() const {
    nlohmann::json j = {
        { "categories", nlohmann::json::object() },
        { "categoryOrder", nlohmann::json::array() },
    };

    for (const category& cat : m_data) {
        j["categories"][cat.name] = nlohmann::json::array();
        j["categoryOrder"].push_back(cat.name);

        for (const level& lvl : cat.levels) {
            j["categories"][cat.name].push_back(lvl.as_json());
        }
    }

    return j;
}

const std::string& level_pack::name() const {
    return m_name;
}

void level_pack::name(std::string name) {
    m_name = name;
}

#include "level_pack.h"

#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <utility>
#include <iterator>
#include <tuple>

std::list<level_pack::category>::iterator level_pack::find_category(std::string name) {
    return std::find_if(m_data.begin(), m_data.end(),
        [&name] (category& cat) { return cat.name == name; }
    );
}

void level_pack::add_group(level_pack::category& cat, level_pack::category::group_properties group) {
    std::size_t old_size = cat.levels.size();

    for (std::size_t i = 0; i < group.n_levels; i++) {
        cat.levels.push_back(level(group.width, group.height, group.colors, group.color_dist));
    }

    std::sort(cat.levels.begin() + old_size - 1, cat.levels.end(),
        [] (const level& a, const level& b) {
            return a.difficulty() < b.difficulty();
        }
    );
}

void level_pack::add_category(std::string name, level_pack::category::group_properties group) {
    if (find_category(name) != m_data.end()) {
        std::ostringstream osstr;
        osstr << "there already exists a category with name ";
        osstr << name;
        throw std::invalid_argument(osstr.str());
    } else {
        std::list<category>::iterator cat = m_data.insert(m_data.end(), { name });
        add_group(*cat, group);
    }
}

void level_pack::add_category(std::string name, std::vector<level_pack::category::group_properties> groups) {
    if (find_category(name) != m_data.end()) {
        std::ostringstream osstr;
        osstr << "there already exists a category with name ";
        osstr << name;
        throw std::invalid_argument(osstr.str());
    } else {
        std::list<category>::iterator cat = m_data.insert(m_data.end(), { name });

        for (category::group_properties& g : groups) {
            add_group(*cat, g);
        }
    }
}

void level_pack::remove_category(std::string name) {
    std::list<category>::iterator it = find_category(name);
    std::ostringstream osstr;

    if (m_data.size() == 0) {
        throw std::invalid_argument("nothing to remove from empty level_pack");
    } else if (it == m_data.end()) {    // if no category with |name| was found
        osstr << "couldn't find a level category with name ";
        osstr << name;
        throw std::invalid_argument(osstr.str());
    } else {    // found
        m_data.erase(it);
    }
}

void level_pack::rename_category(std::string cur_name, std::string new_name) {
    std::list<category>::iterator it = find_category(cur_name);
    std::ostringstream osstr;

    if (m_data.size() == 0) {
        throw std::invalid_argument("nothing to rename in empty level_pack");
    } else if (it == m_data.end()) {    // if no category with |cur_name| was found
        osstr << "couldn't find a level category with name ";
        osstr << cur_name;
        throw std::invalid_argument(osstr.str());
    } else if (find_category(new_name) != m_data.end()) {   // there already exists a category called |new_name|
        osstr << "there already exists a category with name ";
        osstr << new_name;
        throw std::invalid_argument(osstr.str());
    } else {    // found
        it->name = new_name;
    }
}

void level_pack::reorder_category(std::string name, std::size_t new_pos) {
    std::list<category>::iterator it = find_category(name);
    std::ostringstream osstr;

    if (m_data.size() == 0) {
        throw std::invalid_argument("nothing to reorder in empty level_pack");
    } else if (it == m_data.end()) {    // if no category with |name| was found
        osstr << "couldn't find a level category with name ";
        osstr << name;
        throw std::invalid_argument(osstr.str());
    } else if (new_pos >= m_data.size()) {
        osstr << "level category ";
        osstr << name;
        osstr << " couldn't be relocated to out-of-bounds index ";
        osstr << new_pos;
        throw std::out_of_range(osstr.str());
    } else {    // found
        std::list<category>::iterator dest = m_data.begin();
        std::advance(dest, new_pos);
        std::swap(*it, *dest);  // TODO do the |level| objects within |category::data| need move ctors?
    }
}

std::list<level_pack::category>& level_pack::data() {
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

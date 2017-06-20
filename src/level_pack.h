#ifndef LEVEL_PACK_H_
#define LEVEL_PACK_H_

#include <list>
#include <vector>

#include "../lib/json.hpp"
#include "level.h"

#include "json_serializable.h"

// TODO check for and remove duplicates levels when creating a category
class level_pack : public json_serializable {
private:
    struct category {
        // TODO enable optional params for group_properties when adding categories
        struct group_properties {
            std::size_t height;
            std::size_t width;
            std::vector<unsigned> colors;
            std::size_t n_levels;
            gameboard::color_distribution color_dist;
        };

        std::string name;
        std::vector<level> levels;
    };

    std::list<category> m_data;

    std::list<category>::iterator find_category(std::string);
    void add_group(category&, category::group_properties);

public:
    /*
     * Constructs an empty level pack.
     */
    level_pack() = default;

    /**
     * Adds a category to the level pack. Each category can be further organized into groups which contain
     * any number of levels with matching properties.
     *
     * The "group" struct format is as follows: { height, width, colors, [num_levels = 1], [color_distribution = uniform] }
     * - std::size_t: height of the gameboard
     * - std::size_t: width of the gameboard
     * - vector<unsigned>: colors to use for the gameboard
     * - std::size_t: number of levels to create for this group; optional, with a default value of 1
     * - gameboard::color_distribution: color distribution to use
     */
    void add_category(std::string, category::group_properties);

    /*
     * Same as above, but for multiple groups within one category.
     */
    // TODO use something else to allow for multiple arguments, maybe parameter packs
    void add_category(std::string, std::vector<category::group_properties>);

    /*
     * Removes the category with the given name.
     */
    void remove_category(std::string);

    /*
     * Updates the name of the category called |cur_name|, if it is present.
     */
    void rename_category(std::string, std::string);

    /*
     * Changes the position of the category called |name| within the list of all categories.
     * This will affect their order when the level pack is written to a .json file.
     */
    void reorder_category(std::string, std::size_t);

    /*
     * Returns the internal list of categories. Use this if you wish to manually adjust the level pack's contents.
     */
    std::list<category>& data();

    /*
     * Returns a json object representing the level pack so that it can be written to a file.
     */
    nlohmann::json as_json() const;
};

#endif /* LEVEL_PACK_H_ */

#ifndef LEVEL_PACK_TEST_H_
#define LEVEL_PACK_TEST_H_

#include "../lib/catch.hpp"
#include "../src/level_pack.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <vector>

#include <iostream>

bool is_category_valid(const level_pack::category& cat, std::string& name, level_pack::category::group_properties& group) {
    return cat.name == name
        && cat.levels.size() == group.n_levels
        && std::all_of(cat.levels.begin(), cat.levels.end(),
               [&group] (const level& lvl) {
                   return lvl.gameboard_height() == group.height
                       && lvl.gameboard_width() == group.width
                       && lvl.n_colors() == group.colors.size();
               }
           );
}

bool is_category_valid(const level_pack::category& cat,
        std::string& name,
        std::vector<level_pack::category::group_properties>& groups)
{
    std::size_t n_levels = std::accumulate(groups.begin(), groups.end(), 0,
        [] (std::size_t a, level_pack::category::group_properties& b) {
            return a + b.n_levels;
        }
    );

    if (cat.name != name || cat.levels.size() != n_levels) {
        return false;
    }

    std::vector<level>::const_iterator group_start = cat.levels.cbegin();
    for (level_pack::category::group_properties& group : groups) {
        if (!std::all_of(group_start, group_start + group.n_levels,
               [&group] (const level& lvl) {
                   return lvl.gameboard_height() == group.height
                       && lvl.gameboard_width() == group.width
                       && lvl.n_colors() == group.colors.size();
               }
           ))
        {
            return false;
        }

        group_start += group.n_levels;
    }

    return true;
}

bool do_categories_follow_order(const level_pack& levels, std::vector<std::string> categories) {
    std::size_t i = 0;
    return levels.data().size() == categories.size()
        && std::all_of(levels.data().cbegin(), levels.data().cend(),
               [&] (const level_pack::category& cat) {
                   return cat.name == categories.at(i++);
               }
           );
}

TEST_CASE("adding level categories", "[level_pack][level]") {
    level_pack levels;
    std::list<level_pack::category>::const_iterator cat;

    SECTION("level category contents match parameters given") {
        std::string test_group_name = "test";
        level_pack::category::group_properties test_group = {
            2, 2, { 0, 1, 2, 3 }, 20,
            gameboard::color_distribution::uniform
        };
        levels.add_category(test_group_name, test_group);
        cat = levels.data().begin();
        REQUIRE(is_category_valid(*cat, test_group_name, test_group));

        std::string multi_test_group_name = "multi-group test";
        std::vector<level_pack::category::group_properties> multi_test_group = {
            { 3, 3, { 0, 1, 3, 4 }, 12, gameboard::color_distribution::random },
            { 3, 4, { 0, 1, }, 1, gameboard::color_distribution::uniform },
            { 2, 3, { 0, 1, 3, 4 }, 5 },
            { 2, 4, { 0, 1, 3 } }
        };
        levels.add_category(multi_test_group_name, multi_test_group);
        REQUIRE(is_category_valid(*++cat, multi_test_group_name, multi_test_group));
    }

    SECTION("exception thrown when trying to add a category with an already-used name") {
        levels.add_category("test", { 2, 2, { 1, 2 } });
        REQUIRE_THROWS(levels.add_category("test", { 2, 2, { 1, 2 } }));
    }

//    SECTION("level pack is free of duplicates") {
//    }

    SECTION("ordering of categories is preserved") {
        levels.add_category("test1", { 2, 2, { 1, 2 } });
        levels.add_category("test2", { 2, 2, { 1, 2 } });
        levels.add_category("test3", { 2, 2, { 1, 2 } });
        levels.add_category("test4", { 2, 2, { 1, 2 } });
        REQUIRE(do_categories_follow_order(levels, { "test1", "test2", "test3", "test4" }));
    }
}

TEST_CASE("removing level categories", "[level_pack][level]") {
    level_pack levels;

    SECTION("level pack is successfully removed") {
        levels.add_category("test", { 2, 2, { 1, 2 } });
        levels.remove_category("test");
        REQUIRE(levels.data().size() == 0);
    }

    SECTION("trying to remove something from an empty level pack should cause an exception") {
        REQUIRE_THROWS(levels.remove_category("test"));
    }

    SECTION("trying to remove a non-existent category should cause an exception") {
        levels.add_category("test", { 2, 2, { 1, 2 } });
        REQUIRE_THROWS(levels.remove_category("foo"));
    }

    SECTION("ordering of categories is preserved") {
        levels.add_category("test1", { 2, 2, { 1, 2 } });
        levels.add_category("test2", { 2, 2, { 1, 2 } });
        levels.add_category("test3", { 2, 2, { 1, 2 } });
        levels.add_category("test4", { 2, 2, { 1, 2 } });
        levels.remove_category("test3");
        std::cout << std::endl;
        REQUIRE(do_categories_follow_order(levels, { "test1", "test2", "test4" }));
    }
}

TEST_CASE("renaming level categories", "[level_pack][level]") {
    level_pack levels;
    levels.add_category("test", { 2, 2, { 1, 2 } });

    SECTION("category is successfully renamed") {
        levels.rename_category("test", "test2");
        REQUIRE(levels.data().front().name == "test2");
        REQUIRE(levels.data().size() == 1);
    }

    SECTION("renaming a non-existent level pack should cause an exception") {
        REQUIRE_THROWS(levels.rename_category("asdf", "fdsa"));
    }

    SECTION("renaming an existent level pack to a name that already exists should cause an exception") {
        levels.add_category("test2", { 3, 2, { 1, 2 } });
        REQUIRE_THROWS(levels.rename_category("test2", "test"));
    }

    SECTION("ordering of categories is preserved") {
        levels.add_category("test2", { 2, 2, { 1, 2 } });
        levels.add_category("test3", { 2, 2, { 1, 2 } });
        levels.add_category("test4", { 2, 2, { 1, 2 } });
        levels.rename_category("test", "test1");
        std::cout << std::endl;
        REQUIRE(do_categories_follow_order(levels, { "test1", "test2", "test3", "test4" }));
    }
}

TEST_CASE("reordering level categories", "[level_pack][level]") {
    level_pack levels;

    SECTION("category is reordered successfully") {
        levels.add_category("test1", { 2, 2, { 1, 2 } });
        levels.add_category("test2", { 2, 2, { 1, 2 } });
        levels.add_category("test3", { 2, 2, { 1, 2 } });
        levels.add_category("test4", { 2, 2, { 1, 2 } });
        levels.reorder_category("test1", 2);
        REQUIRE(do_categories_follow_order(levels, { "test2", "test3", "test1", "test4" }));
    }

    SECTION("trying to reorder a non-existent category should cause an exception") {
        levels.add_category("test1", { 2, 2, { 1, 2 } });
        levels.add_category("test2", { 2, 2, { 1, 2 } });
        levels.add_category("test3", { 2, 2, { 1, 2 } });
        REQUIRE_THROWS(levels.reorder_category("test5", 2));
    }

    SECTION("trying to reorder an empty level pack should cause an exception") {
        REQUIRE_THROWS(levels.reorder_category("test1", 2));
    }

    SECTION("trying to reorder a category to an out-of-bounds index should cause an exception") {
        levels.add_category("test1", { 2, 2, { 1, 2 } });
        levels.add_category("test2", { 2, 2, { 1, 2 } });
        levels.add_category("test3", { 2, 2, { 1, 2 } });
        REQUIRE_THROWS(levels.reorder_category("test1", 10));
    }
}

#endif /* LEVEL_PACK_TEST_H_ */

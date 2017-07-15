#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <getopt.h>

#include "../lib/json.hpp"

#include "level_pack.h"

static std::string PROGRAM_NAME;

static int indent = 4;

static void display_mesg(std::string mesg, bool verbose) {
    std::cout << PROGRAM_NAME << ": " << mesg << std::endl;
}

static void usage() {
    display_mesg("usage: " + PROGRAM_NAME + " [-h] [-v] [-i indent-level] PARAM_FILES...", false);
}

static void parse_options(int argc, char* argv[]) {
    int c;

    while ((c = getopt(argc, argv, "vhi:")) != -1) {
        switch (c) {
        case 'v':
            break;
        case 'i':
            indent = std::atoi(optarg);
            break;
        case 'h':
            usage();
            exit(0);
        default:
            usage();
            exit(1);
        }
    }
}

static bool does_file_exist(std::string path) {
    std::ifstream file(path);
    return file.good();
}

level_pack::category::group_properties parse_group(const nlohmann::json& j) {
    level_pack::category::group_properties group;

    // TODO eventually use a helper fn (that throws) for searching multiple names at a time
    group.height = j.at("gameboard-height");
    group.width = j.at("gameboard-width");

    for (const std::string& color : j.at("colors")) {
        group.colors.push_back(std::stoul(color, nullptr, 16));
    }

    try {
        std::string color_dist = j.at("color-dist");
        if (color_dist == "uniform") {
            group.color_dist = gameboard::color_distribution::uniform;
        } else if (color_dist == "random") {
            group.color_dist = gameboard::color_distribution::random;
        } else {
            throw std::invalid_argument("invalid color distribution '" + color_dist + "'");
        }
    // the colour distribution value is not a string as it ought to be
    } catch (const std::domain_error& e) {
        throw;
    // the colour distribution value is a string but does not name a valid colour distribution type
    } catch (const std::invalid_argument& e) {
        throw;
    } catch (...) {
        display_mesg("no color distribution specified; using default value instead", true);
    }

    try {
        group.n_levels = j.at("num-levels");
    // the level count is not an int as it ought to be
    } catch (const std::domain_error& e) {
        throw;
    } catch (...) {
        display_mesg("no level count specified for group; assuming one level", true);
    }

    return group;
}

int main(int argc, char* argv[]) {
    PROGRAM_NAME = std::string(argv[0]);
    parse_options(argc, argv);

    for ( ; optind < argc; optind++) {
        if (!does_file_exist(argv[optind])) {
            display_mesg("no file found at " + std::string(argv[optind]), false);
            continue;
        }

        std::ifstream fin(argv[optind]);
        std::ofstream fout;
        nlohmann::json params;

        try {
            params << fin;

            std::string name = params.value("name", "");
            level_pack levels(name);
            fout.open(name + ".json");

            for (const nlohmann::json& cat : params.at("categories")) {
                std::vector<level_pack::category::group_properties> groups;

                for (const nlohmann::json& group : cat.at("groups")) {
                    groups.push_back(parse_group(group));
                }

                levels.add_category(cat.at("name"), groups);
            }

            fout << levels.as_json().dump(indent);
        } catch (const std::exception& e) {
            display_mesg(std::string(e.what()), false);
        }

        fin.close();
        fout.close();
    }

    return 0;
}


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <getopt.h>

#include "../lib/json.hpp"

#include "level_pack.h"

static std::string PROGRAM_NAME;

static int indent = 4;
static bool print_verbose_output = false;

static void display_mesg(std::string mesg, bool verbose) {
    if (!verbose || (verbose && print_verbose_output)) {
        std::cout << PROGRAM_NAME << ": " << mesg << std::endl;
    }
}

static void usage() {
    display_mesg("usage: " + PROGRAM_NAME + " [-h] [-v] [-i indent-level] PARAM_FILES...", false);
}

static void parse_options(int argc, char* argv[]) {
    int c;

    while ((c = getopt(argc, argv, "vhi:")) != -1) {
        switch (c) {
        case 'v':
            print_verbose_output = true;
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

nlohmann::json::const_reference json_at(const nlohmann::json& j, std::string key) {
    return j.at(key);
}

template<typename... Args>
nlohmann::json::const_reference json_at(const nlohmann::json& j, std::string key, Args... args) {
    try {
        return j.at(key);
    } catch (...)  {
        return json_at(j, args...);
    }
}


level_pack::category::group_properties parse_group(const nlohmann::json& j) {
    level_pack::category::group_properties group;

    try {
        group.height = json_at(j, "gameboard-height", "height");
    } catch (const std::domain_error& e) {
        throw std::domain_error("gameboard height: " + std::string(e.what()));
    }

    try {
        group.width = json_at(j, "gameboard-width", "width");
    } catch (const std::domain_error& e) {
        throw std::domain_error("gameboard width: " + std::string(e.what()));
    }

    try {
        for (const std::string& color : json_at(j, "colors", "colours")) {
            group.colors.push_back(std::stoul(color, nullptr, 16));
        }
    } catch (const std::domain_error& e) {
        throw std::domain_error("colors: " + std::string(e.what()));
    }

    try {
        std::string color_dist = json_at(j, "color-dist", "colour-dist", "color-distribution", "colour-distribution", "cdist");
        if (color_dist == "uniform") {
            group.color_dist = gameboard::color_distribution::uniform;
        } else if (color_dist == "random") {
            group.color_dist = gameboard::color_distribution::random;
        } else {
            throw std::invalid_argument("color distribution: invalid value '" + color_dist + "'");
        }
    // the colour distribution value is not a string as it ought to be
    } catch (const std::domain_error& e) {
        throw std::domain_error("color distribution: " + std::string(e.what()));
    // the colour distribution value is a string but does not name a valid colour distribution type
    } catch (const std::invalid_argument& e) {
        throw;
    } catch (...) {
        display_mesg("color distribution: no value specified; using default instead", true);
    }

    try {
        group.n_levels = json_at(j, "num-levels", "n-levels", "level-count");
    // the level count is not an int as it ought to be
    } catch (const std::domain_error& e) {
        throw std::domain_error("level count: " + std::string(e.what()));
    } catch (...) {
        display_mesg("level count: no value specified; using '1' instead", true);
    }

    return group;
}

int main(int argc, char* argv[]) {
    PROGRAM_NAME = std::string(argv[0]);
    parse_options(argc, argv);

    if (optind == argc) {
        display_mesg("to create level packs, please provide at least one JSON parameter file", false);
        usage();
        exit(1);
    }

    for ( ; optind < argc; optind++) {
        if (!does_file_exist(argv[optind])) {
            display_mesg("no parameter file found at '" + std::string(argv[optind]) + "'", false);
            continue;
        }

        display_mesg("creating level pack from parameter file '" + std::string(argv[optind]) + "'", false);

        std::ifstream fin(argv[optind]);
        std::ofstream fout;
        nlohmann::json params;

        try {
            params << fin;

            std::string name = params.value("name", "");
            level_pack levels(name);

            for (const nlohmann::json& cat : params.at("categories")) {
                std::vector<level_pack::category::group_properties> groups;

                for (const nlohmann::json& group : cat.at("groups")) {
                    groups.push_back(parse_group(group));
                }

                levels.add_category(cat.at("name"), groups);
            }

            fout.open(name + ".json");
            fout << levels.as_json().dump(indent);
        } catch (const std::exception& e) {
            display_mesg(std::string(e.what()), true);
            display_mesg("failed to create level pack from parameter file '" + std::string(argv[optind]) + "'", false);
        }

        fin.close();
        fout.close();
    }

    return 0;
}


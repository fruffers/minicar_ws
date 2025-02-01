#include <cstdio>
#include <fstream>
#include "../include/nlohmann/json.hpp"
#include <vector>
// #include <nlohmann/json.hpp>

struct Cone {
    double x, y;
    std::string colour;
};

std::vector<Cone> parseTrackLayout(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open JSON file: " + filename);
    }

    nlohmann::json json_data;
    file >> json_data;

    std::vector<Cone> cones;

    // Parse blue cones
    for (const auto& point : json_data["blue"]) {
        cones.push_back({point[0], point[1], "blue"});
    }

    // Parse yellow cones
    for (const auto& point : json_data["yellow"]) {
        cones.push_back({point[0], point[1], "yellow"});
    }

    // Parse orange cones (if needed)
    for (const auto& point : json_data["orange"]) {
        cones.push_back({point[0], point[1], "orange"});
    }

    return cones;
}

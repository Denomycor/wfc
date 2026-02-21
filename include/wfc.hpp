#pragma once
#include "array2d.hpp"
#include <vector>
#include <unordered_map>

enum class wfc_directions_t {
    UP, DOWN, LEFT, RIGHT
};

using wfc_state_t = array2d<std::vector<int>>;
using wfc_rule_t = std::unordered_map<int, std::vector<int>>;
using wfc_rules_t = std::unordered_map<wfc_directions_t,wfc_rule_t>;

void print_wfc_state(const wfc_state_t& state);


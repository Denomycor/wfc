#include <unordered_set>
#include <vector>

enum wfc_rule_type {
    block, allow
};

class wfc_rule {
private:
    int id;
    wfc_rule_type type;
    std::unordered_set<int> up_rules, down_rules, left_rules, right_rules;

};

using wfc_state_t = std::vector<std::vector<std::unordered_set<int>>>;

class wfc {
private:
    int size_x, size_y;
};


#include "wfc.hpp"


int main(int argc, char** argv){
    std::vector<int> ids;
    ids.emplace_back(1);
    ids.emplace_back(2);
    ids.emplace_back(3);
    ids.emplace_back(4);

    wfc_state_t state(5, 5, ids);
    print_wfc_state(state);
}


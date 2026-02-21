#include "wfc.hpp"
#include <iostream>
#include <queue>
#include <utility>
#include <stdlib.h>
#include <vector>



void print_wfc_state(const wfc_state_t& state){
    for(int y=0; y < state.get_height(); y++){
        for(int x=0; x < state.get_width(); x++){
            for(auto& e: state.elem(x, y)){
                std::cout << e << '.';
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

// Create the wfc state data structure
wfc_state_t wfc_init(int width, int height, const std::vector<int>& ids){
    return wfc_state_t(width, height, ids);
}

// pick the coords of the element with the lowest entropy
std::pair<int, int> wfc_pick(const wfc_state_t& state){
    int x=0, y=0;
    size_t len=state.elem(0, 0).size();
    for(int h=0; h < state.get_height(); h++){
        for(int w=0; w < state.get_width(); w++){
            if(len == 1) break;
            if(state.elem(w, h).size() < len){
                x=w;
                y=h;
                len=state.elem(w, h).size();
            }
        }
    }
    return {x,y};
}

// Randomly collapse the possible ids of a coordinate to one
void wfc_collapse(int x, int y, wfc_state_t& state){
    auto& vec = state.elem(x, y);
    int n = rand() % vec.size();
    int e = vec[n];
    vec.clear();
    vec.push_back(e);
}

bool update_tile(int x, int y, wfc_state_t& state, const wfc_rule_t& rule, const std::vector<int>& possible_ids){
    std::vector<int> acc;
    for(int id : possible_ids){

    }
}

void wfc_propagate(int x, int y, wfc_state_t& state, const wfc_rules_t& rules){
    std::queue<std::pair<int, int>> queue;
    queue.emplace(std::pair<int, int>(x, y));
    while(!queue.empty()){
        auto& curr = queue.front();

        if(curr.second-1 >= 0){
            if(update_tile(curr.first, curr.second-1, state, rules.at(wfc_directions_t::UP), state.elem(curr.first, curr.second)))
                queue.emplace(std::pair<int, int>(curr.first, curr.second-1));
        }
        
        if(curr.second+1 < state.get_height()){
            if(update_tile(curr.first, curr.second+1, state, rules.at(wfc_directions_t::DOWN), state.elem(curr.first, curr.second)))
                queue.emplace(std::pair<int, int>(curr.first, curr.second+1));
        }

        if(curr.first-1 >= 0){
            if(update_tile(curr.first-1, curr.second, state, rules.at(wfc_directions_t::LEFT), state.elem(curr.first, curr.second)))
                queue.emplace(std::pair<int, int>(curr.first-1, curr.second));
        }

        if(curr.first+1 < state.get_width()){
            if(update_tile(curr.first+1, curr.second-1, state, rules.at(wfc_directions_t::RIGHT), state.elem(curr.first, curr.second)))
                queue.emplace(std::pair<int, int>(curr.first+1, curr.second));
        }
        queue.pop();
    }

}


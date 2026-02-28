#include "wave.hpp"
#include <cfloat>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <vector>


Wave::Wave(std::size_t width, std::size_t height, const std::map<int, double>& id_weight)
:state(width, height), entropy(id_weight)
{
    for(auto& tile : state){
        for(auto [k,_] : id_weight){
            tile[k] = true;
        }
    }
}

void Wave::print() const {
    for(std::size_t y=0; y<state.get_height(); y++){
        for(std::size_t x=0; x<state.get_width(); x++){
            for(auto[k,b] : state.get(x,y)){
                if(b) std::cout << k << '.';
            }
            std::cout << ' ';
        }
        std::cout << std::endl;
    }
}

void Entropy::print() const {
    for(auto&[k,v] : id_weight){
        std::cout << k << ':' << v << ' ';
    }
    std::cout << std::endl;
    for(auto&[k,v] : id_log){
        std::cout << k << ':' << v << ' ';
    }
    std::cout << std::endl;
}

Entropy::Entropy(const std::map<int, double>& id_weight)
: id_weight(id_weight), id_log()
{
    precompute_log();
}

void Entropy::precompute_log(){
    for(auto&[k, w] : id_weight){
        id_log[k] = log(w);
    }
}

Entropy& Wave::get_entropy(){
    return entropy;
}

AdjacencyConstraints& Wave::get_constraints(){
    return constr;
}

float Entropy::get_cell_entropy(const std::map<int, bool>& tiles) const {
    double w_sum = 0;
    double sum_sum = 0;
    for(auto[t,b] : tiles){
        if(b){
            w_sum += id_weight.at(t);
            sum_sum += id_weight.at(t) * id_log.at(t);
        }
    }
    return log(w_sum) - (sum_sum/w_sum);
}

void normalized_weight_map(std::map<int, double>& out, const std::map<int, double>& weights, const std::map<int, bool>& tiles) {
    double w_sum = 0;
    for(auto[t,b] : tiles){
        if(b){
            out[t] = weights.at(t);
            w_sum += weights.at(t);
        }
    }
    for(auto[t,w] : out){
        out[t] = w/w_sum;
    }
}

void Wave::collapse_cell(std::size_t x, std::size_t y){
    std::map<int, double> normalized;
    normalized_weight_map(normalized, entropy.id_weight, state.get(x, y));

    auto r = rand() / (double) RAND_MAX;
    double acc = 0;
    int selected_id = normalized.begin()->first;
    for(auto[t,w] : normalized){
        acc += w;
        if(r <= acc){
            selected_id = t;
            break;
        }
    }
    for(auto[t,b] : state.get(x,y)){
        if(t != selected_id){
            state.get(x,y)[t] = false;
        }else{
            assert(b);
        }
    }
}

std::pair<int, int> Wave::select_cell() const {
    std::vector<std::pair<std::size_t, std::size_t>> out{{0,0}};
    double entr = DBL_MAX;

    for(std::size_t x = 0; x < state.get_width(); x++){
        for(std::size_t y = 0; y < state.get_height(); y++){
            double e = entropy.get_cell_entropy(state.get(x,y));
            if(e > 0) {
                if(e < entr){
                    out.clear();
                    out.emplace_back(x,y);   
                    entr = e;
                // comparison of doubles is ok
                } else if (e == entr) {
                    out.emplace_back(x,y);
                }
            }
        }
    }

    if(entr == DBL_MAX) 
        return {-1,-1};
    else
        return out[rand() % out.size()];
}

bool update_cell_state(std::map<int, bool>& cell, const std::map<int, std::vector<int>>& constraints, const std::map<int, bool>& neighboor) {
    bool updated = false;
    for(auto[tile, enabled] : cell){
        // skip tiles already excluded
        if(!enabled) continue;

        bool found = false;

        for(auto[n_tile, n_enabled] : neighboor){
            // skip tiles already excluded
            if(!n_enabled) continue;

            const auto& vec = constraints.at(n_tile);
            if(vec_has(vec, tile)){
                found = true;
                break;
            }
        }
        updated = updated || !found;
        cell[tile] = found;
    }
    return updated;
}

void Wave::propagate_constraints(std::size_t x, std::size_t y) {
    std::queue<std::pair<std::size_t, std::size_t>> queue;
    queue.push({x,y});

    while(!queue.empty()){
        auto current = queue.front();

        //Up
        if(current.second > 0){
            if(update_cell_state(state.get(current.first, current.second-1), constr.constraints[Directions::UP], state.get(current.first, current.second))){
                queue.push({current.first, current.second-1});
            }
        }
        //Down
        if(current.second < state.get_height()-1){
            if(update_cell_state(state.get(current.first, current.second+1), constr.constraints[Directions::DOWN], state.get(current.first, current.second))){
                queue.push({current.first, current.second+1});
            }
        }
        //Left
        if(current.first > 0){
            if(update_cell_state(state.get(current.first-1, current.second), constr.constraints[Directions::LEFT], state.get(current.first, current.second))){
                queue.push({current.first-1, current.second});
            }
        }
        //Right
        if(current.first < state.get_width()-1){
            if(update_cell_state(state.get(current.first+1, current.second), constr.constraints[Directions::RIGHT], state.get(current.first, current.second))){
                queue.push({current.first+1, current.second});
            }
        }
        queue.pop();
    }
}

AdjacencyConstraints::AdjacencyConstraints()
: constraints()
{
    constraints[Directions::UP] = std::map<int, std::vector<int>>();
    constraints[Directions::DOWN] = std::map<int, std::vector<int>>();
    constraints[Directions::LEFT] = std::map<int, std::vector<int>>();
    constraints[Directions::RIGHT] = std::map<int, std::vector<int>>();
}

void Wave::run(){
    while(true){
        auto selected = select_cell();
        if(selected == std::make_pair(-1,-1)){
            break;
        }
        collapse_cell(selected.first, selected.second);
        propagate_constraints(selected.first, selected.second);
    }
}

void AdjacencyConstraints::add(int id, const std::vector<int>& up,
            const std::vector<int>& down,
            const std::vector<int>& left,
            const std::vector<int>& right)
{
    constraints[Directions::UP][id] = up;
    constraints[Directions::DOWN][id] = down;
    constraints[Directions::LEFT][id] = left;
    constraints[Directions::RIGHT][id] = right;
}


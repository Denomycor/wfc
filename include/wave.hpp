#pragma once
#include "array2d.hpp"
#include <cstddef>
#include <map>
#include <vector>

enum class Directions{
    UP, DOWN, LEFT, RIGHT
};

/*
 * The Entropy helper, stores values of weights normalizes and precomputes log(pi)
 */
class Entropy {
private:
    void precompute_log();

public:
    std::map<int, double> id_weight;
    std::map<int, double> id_log;

    Entropy(const std::map<int, double>& id_weight);

    void print() const;

    float get_cell_entropy(const std::map<int, bool>& tiles) const;

};


struct AdjacencyConstraints {
    std::map<Directions, std::map<int, std::vector<int>>> constraints;

    AdjacencyConstraints();

    void add(int id, const std::vector<int>& up,
                const std::vector<int>& down,
                const std::vector<int>& left,
                const std::vector<int>& right);
};

/*
 * The WFC state, the wave function stores all superpositions for all tiles
 */
class Wave {
private:
    Array2D<std::map<int, bool>> state;
    Entropy entropy;
    AdjacencyConstraints constr;

public:
    Wave(std::size_t width, std::size_t height, const std::map<int, double>& id_weight);

    Entropy& get_entropy();
    AdjacencyConstraints& get_constraints();

    void print() const;

    void collapse_cell(std::size_t x, std::size_t y);

    std::pair<int, int> select_cell() const;

    void propagate_constraints(std::size_t x, std::size_t y);

    void run();

};

template<typename T>
bool vec_has(const std::vector<T>& vec, const T& val){
    for(const auto& v : vec){
        if(v == val){
            return true;
        }
    }
    return false;
}

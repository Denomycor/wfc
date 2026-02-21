#pragma once
#include <vector>


template<typename T>
class array2d {
private:
    int width, height;
    std::vector<T> buffer;

public:
    array2d(int p_width, int p_heigth)
    : width(p_heigth), height(p_heigth), buffer(p_width * p_heigth)
    {}    

    array2d(int p_width, int p_heigth, const T& init_value)
    : width(p_heigth), height(p_heigth), buffer(p_width * p_heigth, init_value)
    {}    

    T& elem(int x, int y) {
        return buffer[y * width + x];
    }

    const T& elem(int x, int y) const {
        return buffer[y * width + x];
    }

    template<typename U>
    void set(int x, int y, U&& value) {
        buffer[y * width + x] = std::forward<U>(value);
    }

    auto begin() {
        return buffer.begin();
    }

    auto end() {
        return buffer.end();
    }

    int get_width() const {
        return width;
    }

    int get_height() const {
        return height;
    }
};


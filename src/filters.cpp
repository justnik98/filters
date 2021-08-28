//
// Created by justnik on 28.08.2021.
//

#include <cmath>
#include "filters.hpp"

double MedianFilter::filter(double new_val) {
    if (data.size() < size) {
        data.push_back(new_val);
    } else {
        data[ind++ % size] = new_val;
    }
    auto v = data;
    auto m = v.begin() + v.size() / 2;
    std::nth_element(v.begin(), m, v.end());
    return v[v.size() / 2];
}

MedianFilter::MedianFilter(size_t size) : size(size) {
    data.reserve(size);
}

double RunningAvg::filter(double new_val) {
    if (data.size() < size) {
        data.push_back(new_val);
    } else {
        data[ind++ % size] = new_val;
    }
    double mean = 0;
    for (auto &&v: data) {
        mean += v;
    }
    return mean / static_cast<double>(data.size());
}

RunningAvg::RunningAvg(size_t size) : size(size) {
    data.reserve(size);
}

double ExpSmooth::filter(double new_val) {
    if (std::isnan(last_val)) {
        return new_val;
    }
    return new_val * coef + (1 - coef) * last_val;
}

ExpSmooth::ExpSmooth(double coef) : coef(coef) {}

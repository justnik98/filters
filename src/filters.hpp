//
// Created by justnik on 28.08.2021.
//

#ifndef FILTERS_FILTERS_HPP
#define FILTERS_FILTERS_HPP

#include <vector>

class IFilter {
public:
    virtual double filter(double new_val) = 0;
};

class MedianFilter : public IFilter {
public:
    double filter(double new_val) final;
};

class RunningAvg : public IFilter {
public:
    double filter(double new_val) final;
};

class ExpSmooth : public IFilter {
public:
    double filter(double new_val) final;
};

#endif //FILTERS_FILTERS_HPP

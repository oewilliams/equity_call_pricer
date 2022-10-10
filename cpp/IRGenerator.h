#pragma once

#include <vector>
#include <numeric>
#include "rng.h"

class IRGenerator
{
public:
    const double mean;
    const double vol;
    const double start;

    IRGenerator(
        const double mean,
        const double vol,
        const double start,
        std::shared_ptr<RandomGenerator>& rng
        )
    : mean(mean), vol(vol), start(start), rng(rng) {

    }

    std::vector<double> get_path(const unsigned int num) {
        auto steps = rng->normal(num, this->mean, this->vol);
        steps[0] += this->start;
        std::partial_sum(steps.begin(), steps.end(), steps.begin());

        return steps;
    }
private:
    std::shared_ptr<RandomGenerator>& rng;
};


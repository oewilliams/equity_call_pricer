#pragma once

#include <vector>
#include <numeric>
#include "rng.h"

class FXGenerator
{
public:
    const double drift;
    const double vol;
    const double start;

    FXGenerator(
        const double drift,
        const double vol,
        const double start,
        std::shared_ptr<RandomGenerator>& rng
        )
    : drift(drift), vol(vol), start(start), rng(rng) {

    }

    std::vector<double> get_path(const unsigned int num) {
        auto steps = rng->log_normal(num, this->drift, this->vol);
        steps[0] *= this->start;

        for (size_t i=1 ; i<steps.size() ; ++i) {
            steps[i] *= steps[i-1];
        }

        return steps;
    }
private:
    std::shared_ptr<RandomGenerator>& rng;
};
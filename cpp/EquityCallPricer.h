#pragma once

#include <vector>
#include <math.h>
#include <memory>

#include "IRGenerator.h"
#include "FXGenerator.h"
#include "rng.h"

class EquityCallPricer {

public:
    const double strike;
    const double start_val;
    const double vol;
    const unsigned int points_per_path;

    EquityCallPricer(
        const double strike,
        const double start_val,
        const double vol,
        const int points_per_path,
        std::shared_ptr<RandomGenerator>& rng,
        std::unique_ptr<IRGenerator>& ir_generator,
        std::unique_ptr<FXGenerator>& fx_generator
    ) : 
        strike(strike),
        start_val(start_val),
        vol(vol),
        points_per_path(points_per_path),
        rng(rng),
        ir_generator(ir_generator),
        fx_generator(fx_generator)
    {}

    std::vector<double> simulate_value(const unsigned int paths) {

        std::vector<double> simulated_value_path(this->points_per_path, 0.0);

        for(uint i=0 ; i<paths ; ++i){
            const auto value_path = this->simulate_value_single_path();

            for(size_t v=0 ; v<this->points_per_path ; ++v) {
                simulated_value_path[v] += (value_path[v] - simulated_value_path[v] ) / double(i + 2);
            }
        }

        return simulated_value_path;
    }

    std::vector<double> simulate_value_single_path() {
        auto stock_path = this->generate_stock_path();
        const auto fx_path = fx_generator->get_path(this->points_per_path);

        auto value_path = this->value_on_path(stock_path);

        auto fx_val = &fx_path[0];
        for (auto &v : value_path) {
            v *= *(fx_val++);
        }

        return value_path;
    }

private:
    std::shared_ptr<RandomGenerator>& rng;
    std::unique_ptr<IRGenerator>& ir_generator;
    std::unique_ptr<FXGenerator>& fx_generator;

    std::vector<double> generate_stock_path() {
        auto stock_path = rng->normal(this->points_per_path, 0.0, this->vol);
        const auto ir_path = ir_generator->get_path(this->points_per_path);
        
        auto ir_val = &ir_path[0];
        for (auto &s : stock_path) {
            s = exp(s + *(ir_val++));
        } 

        stock_path[0] *= this->start_val;

        for (size_t i=1 ; i<stock_path.size() ; ++i) {
            stock_path[i] *= stock_path[i-1];
        } 

        return stock_path;
    }

    std::vector<double> value_on_path(const std::vector<double> &stock_path) {
        std::vector<double> value_path;
        value_path.reserve(stock_path.size());

        for (const auto s: stock_path) {
            const double value = (s > this->strike) ? s : 0.0;
            value_path.push_back(value);
        }

        return value_path;
    }
};
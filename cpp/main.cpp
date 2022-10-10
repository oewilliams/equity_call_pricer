#include <memory>
#include <iostream>

#include "rng.h"
#include "IRGenerator.h"
#include "FXGenerator.h"
#include "EquityCallPricer.h"

int main(int argc, char *argv[]) {

    const unsigned int seed = 342326;

    int paths = 1000000;
    int price_points = 25;
    double strike = 80.0;

    // IR params
    double ir_start = 0.000001;
    double ir_mean = 0.000002;
    double ir_vol = 0.000001;

    // FX params
    double fx_start = 1.01;
    double fx_drift = 0.0;
    double fx_vol = 0.01;

    // Equity params
    double security_start = 100.0;
    double security_vol = 0.02;

    std::shared_ptr<RandomGenerator> rng = std::make_shared<RandomGenerator>(seed);
    std::unique_ptr<IRGenerator> ir_generator = std::make_unique<IRGenerator>(ir_mean, ir_vol, ir_start, rng);
    std::unique_ptr<FXGenerator> fx_generator = std::make_unique<FXGenerator>(fx_drift, fx_vol, fx_start, rng);

    EquityCallPricer equity_pricer{strike, security_start, security_vol, price_points, rng, ir_generator, fx_generator};

    const auto simulated_path_value = equity_pricer.simulate_value(paths);
    
    for(const auto x : simulated_path_value) {
        std::cout << x << '\n';
    }
    std::cout << std::endl;
    
}
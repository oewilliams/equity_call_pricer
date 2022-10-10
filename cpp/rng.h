#pragma once

#include <random>
#include <vector>
#include <math.h>

class RandomGenerator {
private:
	std::mt19937_64 mt;
	std::uniform_real_distribution<double> u;
public:
	RandomGenerator(const unsigned int s) : mt(s) {}
	
    double uniform() {
		return u(mt);
	}

    std::vector<double> normal(const unsigned int num) {
        
		size_t gens = num + (num & 1);
		std::vector<double> rands(gens,0.0);
		while (gens--) {
			rands[gens] = u(mt);
		}
		
		std::vector<double> pathVec;
		pathVec.reserve(num);
		auto x = &rands[0];
		do {
			const auto coef = sqrt(-2*log(*x));
			const auto next = *(++x);
			pathVec.push_back(coef * cos(next));
			pathVec.push_back(coef * sin(next));

		}while(++x < &rands.back());

        if (num & 1) {
            pathVec.pop_back();
        }

		return pathVec;
	};

    std::vector<double> normal(
        const unsigned int num,
        const double mean,
        const double var
    ) {
        
        auto path = this->normal(num);
        for (auto &x : path) {
            x = mean + (x * var);
        }

        return path;
    }

    std::vector<double> log_normal(
        const unsigned int num,
        const double drift,
        const double var
    ) {
        auto path = this->normal(num);
        for (auto &x : path) {
            x = exp(drift + (x * var));
        }

        return path;
    }
};
mod random_generator;
mod ir_generator;
mod fx_generator;
mod equity_call_pricer;

use equity_call_pricer::equity_call_pricer::EquityCallPricer;
use fx_generator::fx_generator::FXGenerator;
use ir_generator::ir_generator::IRGenerator;
use rand_xoshiro::rand_core::SeedableRng;
use rand_xoshiro::Xoshiro256Plus;
use random_generator::random_generator::RandomGenerator;

fn main() {
    let seed: u64 = 342326;

    let paths: usize = 1_000_000;
    let price_points: usize = 25;
    let strike: f64 = 80.0;

    // IR params
    let ir_start: f64 = 0.000001;
    let ir_mean: f64 = 0.000002;
    let ir_vol: f64 = 0.000001;

    // FX params
    let fx_start: f64 = 1.01;
    let fx_drift: f64 = 0.0;
    let fx_vol: f64 = 0.01;

    // Equity params
    let security_start: f64 = 100.0;
    let security_vol: f64 = 0.02;

    let mut equity_pricer: EquityCallPricer = EquityCallPricer{
        strike: strike,
        start: security_start,
        vol: security_vol,
        points: price_points,
        rng: RandomGenerator{rng: Xoshiro256Plus::seed_from_u64(seed)},
        ir_generator: IRGenerator{
            mean: ir_mean,
            vol: ir_vol,
            start: ir_start,
        },
        fx_generator: FXGenerator{
            drift: fx_drift,
            vol: fx_vol,
            start: fx_start,
        }
    };
    
    let simulated_value = equity_pricer.simulate_value_path(paths);

    for v in simulated_value {
        println!("{}",v);
    }
    
}

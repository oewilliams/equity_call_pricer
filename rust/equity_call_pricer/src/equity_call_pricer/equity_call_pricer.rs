use crate::{
    random_generator::random_generator::RandomGenerator,
    ir_generator::ir_generator::IRGenerator,
    fx_generator::fx_generator::FXGenerator
};


pub struct EquityCallPricer {
    pub strike: f64,
    pub start: f64,
    pub vol: f64,
    pub points: usize,
    pub rng: RandomGenerator,
    pub ir_generator: IRGenerator,
    pub fx_generator: FXGenerator
}

impl EquityCallPricer {

    pub fn simulate_value_path(&mut self, paths: usize) -> Vec<f64> {
        let mut simulated_value_path: Vec<f64> = vec![0.0; self.points];

        for i in 0..paths {
            let value_path = self.simulate_value_single_path();

            for v in 0..self.points {
                simulated_value_path[v] += (value_path[v] - simulated_value_path[v] ) / (i + 2) as f64;
            }
        }

        simulated_value_path
    }

    pub fn simulate_value_single_path(&mut self) -> Vec<f64> {
        let stock_path = self.generate_stock_path();
        let fx_path = self.fx_generator.get_path(self.points, &mut self.rng);

        let value_path = self.value_on_path(&stock_path);

        value_path.iter().zip(fx_path.iter()).map(|(v, f)| v * f).collect()
    }

    fn generate_stock_path(&mut self) -> Vec<f64> {
        let mut stock_path: Vec<f64> = self.rng.normal(self.points, 0.0, self.vol);
        let ir_path = self.ir_generator.get_path(self.points, &mut self.rng);

        stock_path = stock_path
            .iter()
            .zip(ir_path.iter())
            .map(|(s, r)| f64::exp(*s + r))
            .collect::<Vec<f64>>();

        stock_path
            .iter_mut()
            .fold(self.start, |acc, v| {
            *v *= acc;
            *v
        });

        stock_path
    }

    fn value_on_path(&self, stock_path: &Vec<f64>) -> Vec<f64> {
        stock_path.iter().map(|v| if *v > self.strike {*v} else {0.0}).collect()
    }
}
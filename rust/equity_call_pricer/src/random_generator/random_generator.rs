use rand::Rng;

pub struct RandomGenerator {
    pub rng: rand_xoshiro::Xoshiro256Plus
}

impl RandomGenerator {
    pub fn standard_normal(&mut self, num: usize) -> Vec<f64> {
        let gens: usize = num + (num & 1);

        let rands: Vec<f64> = (0..gens).map(|_| self.rng.gen::<f64>()).collect();

        let mut path_vec: Vec<f64> = Vec::with_capacity(gens);

        for i in (0..gens).step_by(2) {
            let coef: f64 = f64::sqrt(-2.0 * f64::ln(rands[i]));
            path_vec.push(coef * f64::cos(rands[i+1]));
            path_vec.push(coef * f64::sin(rands[i+1]));
        }

        if num & 1 == 1 {
            path_vec.pop();
        }
        
        path_vec
    }

    pub fn normal(&mut self, num: usize, mean: f64, vol: f64) -> Vec<f64>{
        self.standard_normal(num).iter().map(|x| mean + x*vol).collect()
    }

    pub fn log_normal(&mut self, num: usize, drift: f64, vol: f64) -> Vec<f64> {
        self.standard_normal(num).iter().map(|x| f64::exp(drift + x*vol)).collect()
    }
}
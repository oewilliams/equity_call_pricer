use crate::random_generator::random_generator::RandomGenerator;


pub struct FXGenerator {
    pub drift: f64,
    pub vol: f64,
    pub start: f64,
}

impl FXGenerator {
    pub fn get_path(&self, num: usize, rng: &mut RandomGenerator) -> Vec<f64> {
        let mut steps: Vec<f64> = rng.log_normal(num, self.drift, self.vol);
        steps.iter_mut().fold(self.start, |acc, x| {
            *x *= acc;
            *x
        });
        steps
    }
}
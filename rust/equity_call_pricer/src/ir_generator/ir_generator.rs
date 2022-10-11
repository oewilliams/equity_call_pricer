use crate::random_generator::random_generator::RandomGenerator;


pub struct IRGenerator {
    pub mean: f64,
    pub vol: f64,
    pub start: f64,
}

impl IRGenerator {
    pub fn get_path(&self, num: usize, rng: &mut RandomGenerator) -> Vec<f64> {
        let mut steps: Vec<f64> = rng.normal(num, self.mean, self.vol);
        steps.iter_mut().fold(self.start, |acc, x| {
            *x += acc;
            *x
        });
        steps
    }
}
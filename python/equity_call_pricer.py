
from fx_generator import FXGenerator
from ir_generator import InterestRateGenerator

import numpy as np

class EquityCallPricer:

    def __init__(
        self,
        strike: float,
        start_val: float,
        vol: float,
        points_per_path: int,
        ir_generator: InterestRateGenerator,
        fx_generator: FXGenerator
    ) -> None:
        self.strike = strike
        self.start = start_val
        self.vol = vol
        self.points_per_path = points_per_path
        self.ir_generator = ir_generator
        self.fx_generator = fx_generator

        return

    def simulate_value(self, paths: int) -> np.array:
        value_paths = [self.simulate_value_single_path() for _ in range(paths)]
        simulated_value_path = np.mean(value_paths, axis=0)
        return simulated_value_path

    def simulate_value_single_path(self) -> np.array:
        stock_path = self._generate_stock_path()
        fx_path = self.fx_generator.get_path(self.points_per_path)

        raw_value_path = self._value_on_path(stock_path)
        ccy_value_path = raw_value_path * fx_path

        return ccy_value_path

    def _generate_stock_path(self) -> np.array:
        """
        Generate a GBM path with stochastic IR, as provided by the classes IR generator.
        Recall that under the risk free measure a stock in the Black-Scholes model has drift
        equal to the interest rate of a risk free asset.
        """
        bm_path = np.random.normal(0.0, self.vol, self.points_per_path)
        ir_path = self.ir_generator.get_path(self.points_per_path)
        
        bm_path += ir_path

        stock_path = np.exp(bm_path)
        stock_path[0] *= self.start
        stock_path = stock_path.cumprod()

        return stock_path
    
    def _value_on_path(self, stock_path: np.array) -> np.array:
        call_value = np.maximum(stock_path - self.strike, np.zeros(self.points_per_path))
        return call_value
    

    


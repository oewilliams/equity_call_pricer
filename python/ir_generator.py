from typing import Optional
import numpy as np

class InterestRateGenerator:
    """
    Simulate interest rates using a gaussian random walk (negative ir is allowed now) given a mean and vol.
    Generation of paths is done in blocks of a given size. When requesting IR data we exhaust the block, then 
    generate a new one.
    """

    def __init__(self, mean: float, vol: float, start: Optional[float]):
        self.mean = mean
        self.vol = vol
        self.start = start if start else mean
        return

    def get_path(self, steps: int):
        return self._simulate(steps, self.start)
    
    def _simulate(self, size: int, start: float) -> np.array:
        steps = np.random.normal(self.mean, self.vol, size)
        steps[0] += start
        return steps.cumsum()
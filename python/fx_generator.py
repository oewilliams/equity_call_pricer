import numpy as np

class FXGenerator:
    """
    Simulate some FX rate using a geometric BM given a mean and vol.

    We assume that some exchange rate X from ccy A to B is given by

    dX = X(mu*dt + sigma*dW)

    for mean mu, vol sigma. 

    Generation of paths is done in blocks of a given size. When requesting IR data we exhaust the block, then 
    generate a new one.
    """

    def __init__(self, drift: float, vol: float, start: float = 0.0):
        self.drift = drift
        self.vol = vol
        self.start = start
        return
        
    def get_path(self, steps: int):
        return self._simulate(steps, self.start)

    def _simulate(self, size: int, start: float) -> np.array:
        steps = np.exp(np.random.normal(self.drift, self.vol, size))
        steps[0] *= start
        return steps.cumprod()


from equity_call_pricer import EquityCallPricer
from fx_generator import FXGenerator
from ir_generator import InterestRateGenerator


def main():
    paths = 1000000
    price_points = 25
    strike = 80.0

    # IR params
    ir_start = 0.000001
    ir_mean = 0.000002
    ir_vol = 0.000001

    # FX params
    fx_start = 1.01
    fx_drift = 0.0
    fx_vol = 0.01

    # Equity params
    security_start = 100.0
    security_vol = 0.02

    ir_generator = InterestRateGenerator(ir_mean, ir_vol, ir_start)
    fx_generator = FXGenerator(fx_drift, fx_vol, fx_start)
    equity_pricer = EquityCallPricer(
        strike=strike,
        start_val=security_start,
        vol=security_vol,
        points_per_path=price_points,
        ir_generator=ir_generator,
        fx_generator=fx_generator,
    )

    simulated_value_path = equity_pricer.simulate_value(paths)

    print(simulated_value_path)



if __name__ == "__main__":
    main()
from typing import Callable, Tuple, List, Optional
import numpy as np
import matplotlib.pyplot as plt

# Function to compute the risk-neutral probability
def risk_neutral_probability(U: float, D: float, R: float) -> float:
    return (R - D) / (U - D)

# Function to compute stock price at node (n, i)
def stock_price_binomial(S0: float, U: float, D: float, n: int, i: int) -> float:
    return S0 * (1 + U)**i * (1 + D)**(n - i)

# Function to compute the binomial CRR option price
def CRR_pv(S0: float, K: float, U: float, D: float, R: float, N: float, Payoff: Callable[[float, float], float]) -> float:
    q: float = risk_neutral_probability(U, D, R)  # Risk-neutral probability

    n: int = int(N)
    price: List[float] = [0] * (n + 1)

    # Calculate the payoff at maturity
    for i in range(n + 1):
        price[i] = Payoff(S0 * (1 + U)**i * (1 + D)**(n - i), K)

    # Step backwards through the tree to calculate the option price
    for n in range(n - 1, -1, -1):
        for i in range(n + 1):
            price[i] = (q * price[i + 1] + (1 - q) * price[i]) / (1 + R)

    return price[0]

# Function to compute delta (sensitivity to stock price changes)
def CRR_delta(S0: float, K: float, U: float, D: float, R: float, N: int, epsilon: float, Payoff: Callable[[float, float], float]) -> float:
    PV_S0pp: float = CRR_pv(S0 * (1. + epsilon), K, U, D, R, N, Payoff)  # Price with S0 + epsilon
    PV_S0mm: float = CRR_pv(S0 * (1. - epsilon), K, U, D, R, N, Payoff)  # Price with S0 - epsilon
    return (PV_S0pp - PV_S0mm) / (S0 * 2. * epsilon)


def sigma_to_UD(sigma: float, T: float, N: float, nu: float = 0.0) -> Tuple[float, float]:
    dT: float = T / N
    U: float = np.exp(nu * dT + sigma * np.sqrt(T / N)) - 1.
    D: float = np.exp(nu * dT + -sigma * np.sqrt(T / N)) - 1.
    return U, D

# Function to compute vega (sensitivity to volatility changes)
def CRR_vega(S0: float, K: float, sigma: float, T: float, R: float, N: float, epsilon: float, Payoff: Callable[[float, float], float], SigmaToUD: Callable) -> float:
    # Bumping sigma by epsilon% of the current volatility
    sigma_pp: float = sigma * (1 + epsilon)  # Increase volatility by epsilon% (1 bps if epsilon = 0.01)
    sigma_mm: float = sigma * (1 - epsilon)

    U_pp, D_pp = SigmaToUD(K, sigma_pp)
    PV_pp: float = CRR_pv(S0, K, U_pp, D_pp, R, N, Payoff)

    U_mm, D_mm = SigmaToUD(K, sigma_mm)
    PV_mm: float = CRR_pv(S0, K, U_mm, D_mm, R, N, Payoff)

    return (PV_pp - PV_mm) / (sigma * 2. * epsilon)  # Divide by the actual volatility bump size


# Payoff function for European Call Option
def european_call_payoff(z: float, K: float) -> float:
    return max(z - K, 0.0)


def digital_call_payoff(z: float, K: float) -> float:
    return float(z > K)


def european_put_payoff(z: float, K: float) -> float:
    return max(-z + K, 0.0)

# Double Knock-Out Option Payoff
def knockout_european_call_payoff(z: float, K: float, U: float) -> float:
    if z >= U:
        return 0.0  # Knocked out
    return max(z - K, 0.0)  # Standard European call option payoff if no knock-out


# Function to plot option PV, delta, and vega
def plotter(
    K_range: np.ndarray, pvs: List[float],
    deltas: Optional[List[float]] = None, vegas: Optional[List[float]] = None
):
    fig, ax1 = plt.subplots(figsize=(10, 6))

    # Plot PV (Present Value)
    ax1.plot(K_range, pvs, label="PV (Option Price)", color='orange')
    ax1.set_xlabel('Strike Price (K)')
    ax1.set_ylabel('Present Value (PV)', color='orange')
    ax1.tick_params(axis='y', labelcolor='orange')

    # Create a twin axis to plot delta and vega
    if bool(deltas) or bool(vegas):
        ax2 = ax1.twinx()
        if bool(deltas):
            ax2.plot(K_range, deltas, label='Delta', linestyle='--', color='red')
        if bool(vegas):
            ax2.plot(K_range, vegas, label='Vega', linestyle=':', color='blue')
        ax2.set_ylabel('Greeks (Delta, Vega)', color='blue')
        ax2.tick_params(axis='y', labelcolor='blue')

    # Add title and legend
    fig.tight_layout()
    plt.title('Option PV, Delta, and Vega vs Strike Price (K)')
    fig.legend(loc="upper left", bbox_to_anchor=(0.1, 0.9), bbox_transform=ax1.transAxes)
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    # Parameters
    S0: float = 100.  # Initial stock price
    sigma: float = 0.1  # Annual volatility (10%)
    N: float = 250.  # Number of steps (250 days in a year)
    R: float = 0.  # Risk-free rate (0%)
    T: float = 1.  # Time to expiry in years
    epsilon: float = 0.01  # Small perturbation for delta and vega calculation

    # Range of strike prices K from 50 to 150
    K_range: np.ndarray = np.concatenate([np.arange(50, 95, 5), np.arange(95, 105.00001, 1.), np.arange(105, 150.00001, 5)])

    # Lists to store option prices, deltas, and vegas
    option_prices: List[float] = []
    deltas: List[float] = []
    vegas: List[float] = []

    # Calculate option prices, delta, and vega for each strike price K
    payoff_op: Callable[[float, float], float] #= digital_call_payoff  # european_put_payoff, european_call_payoff
    payoff_op = digital_call_payoff
    # payoff_op = lambda z, K, U=S0*1.2: knockout_european_call_payoff(z=z, K=K, U=U)
    getUD_op = lambda K, volatility, t=float(T), n=float(N): sigma_to_UD(sigma=volatility, T=t, N=n, nu=-1.0/(n * t/n) * np.log(float(S0)/float(K)))

    for K in K_range:
        U, D = getUD_op(K, sigma)

        option_price = CRR_pv(S0, K, U, D, R, N, payoff_op)
        option_prices.append(option_price)

        delta = CRR_delta(S0, K, U, D, R, N, epsilon, payoff_op)
        deltas.append(delta)
        vega = CRR_vega(S0, K, sigma, T, R, N, epsilon, payoff_op, getUD_op)
        vegas.append(vega)

    # Call the plotter function to render the plot
    plotter(K_range, option_prices, deltas, vegas)

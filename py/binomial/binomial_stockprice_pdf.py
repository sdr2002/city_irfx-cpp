import numpy as np
import matplotlib.pyplot as plt
import math

# Function to compute the risk-neutral probability
def risk_neutral_prob(U, D, R):
    return (R - D) / (U - D)

# Function to compute stock price at node (n, i)
def stock_price(S0, U, D, n, i):
    return S0 * (1 + U)**i * (1 + D)**(n - i)

# Function to compute binomial coefficient (n choose i)
def binomial_coefficient(n, i):
    if i < 0 or i > n:
        return 0.0
    result = 1
    for k in range(1, i + 1):
        result *= (n - i + k) / k
    return result

# Function to compute the PDF
def compute_pdf(N, q):
    pdf = []
    for i in range(N + 1):
        prob = binomial_coefficient(N, i) * (q ** i) * ((1 - q) ** (N - i))
        pdf.append(prob)
    return np.array(pdf)

# Function to plot stock prices and PDFs
def plotter(stock_prices_list, pdf_list):
    fig, ax1 = plt.subplots(figsize=(10, 6))

    # Plot the stock prices for both D values
    ax1.plot(stock_prices_list[0], label="S(i), D=-0.03", color='blue', marker='o')
    ax1.plot(stock_prices_list[1], label="S(i), D=-0.01", color='orange', marker='o')

    ax1.set_xlabel('Node i (Number of upward moves)')
    ax1.set_ylabel('Stock Price S(N, i)')
    ax1.legend(loc='upper left')

    # Create a second y-axis for PDF
    ax2 = ax1.twinx()
    ax2.plot(pdf_list[0], label="PDF, D=-0.03", color='gray', linestyle='--', marker='o')
    ax2.plot(pdf_list[1], label="PDF, D=-0.01", color='yellow', linestyle='--', marker='o')
    ax2.set_ylabel('PDF (Probability Distribution)')
    ax2.legend(loc='upper right')

    # Add grid and title
    plt.title('Stock Price S(N, i) and PDF for different Downward moves D')
    plt.grid(True)

    # Show plot
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    # Parameters from the image
    S0 = 100  # Initial stock price
    U = 0.05  # Upward move
    D_values = [-0.03, -0.01]  # Downward moves for two cases
    R = 0  # Risk-free rate
    N = 100  # Number of steps

    # Stock prices and PDFs for different values of D
    stock_prices_list = []
    pdf_list = []

    for D in D_values:
        q = risk_neutral_prob(U, D, R)
        stock_prices = [stock_price(S0, U, D, N, i) for i in range(N + 1)]
        pdf = compute_pdf(N, q)

        stock_prices_list.append(stock_prices)
        pdf_list.append(pdf)

    # Call the plotter function to render the plot
    plotter(stock_prices_list, pdf_list)

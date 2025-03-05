# pragma once
# include "general_agent.hpp"
# include "matrix.hpp"

// Invests only in bonds
class RiskHater : public Agent {
    public:
        RiskHater(unsigned int m, unsigned int n);
        std::vector<double> initial_position();
        std::vector<double> update_position();
};

// Invests equally in all assets
class NaiveInvestor : public Agent {
    public:
        NaiveInvestor(unsigned int m, unsigned int n);
        std::vector<double> initial_position();
        std::vector<double> update_position();
};

// Invests in assets based on Markowitz portfolio theory
// Invest in bonds for first lookback periods and then use Markowitz portfolio theory
// Will need to add a lot of extra variables to store the covariance matrix and expected returns
class MarkowitzSavvy : public Agent {
    private:
        unsigned int lookback;
        double target_return;
    public:
    MarkowitzSavvy(unsigned int m, unsigned int n, unsigned int l, double r);
        std::vector<double> initial_position();
        std::vector<double> update_position();
};

// Functions useful for the MarkowitzSavvy class
// Convert to returns matrix
std::vector<std::vector<double>> returns_matrix(std::vector<std::vector<double>>& prices, unsigned int M, unsigned int current_period, unsigned int lookback,
    unsigned int num_assets);

// Calculate the mean of returns matrix
std::vector<double> mean_returns(std::vector<std::vector<double>>& returns);

// Calculate the covariance matrix of returns matrix
std::vector<std::vector<double>> covariance_returns(std::vector<std::vector<double>>& returns);